#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arithmetic.h"

void OR_BINT(BINT* ptrX, BINT* ptrY, BINT** pptrZ) {
    int min_len = MINIMUM(ptrX->wordlen, ptrY->wordlen);
    int max_len = MAXIMUM(ptrX->wordlen, ptrY->wordlen);
    for (int i = 0; i < min_len; i++) {
        (*pptrZ)->val[i] = ptrX->val[i] | ptrY->val[i];
    }
    for (int i = min_len; i < max_len; i++) {
        if(ptrX->wordlen > ptrY->wordlen)
            (*pptrZ)->val[i] = ptrX->val[i];
        else
            (*pptrZ)->val[i] = ptrY->val[i];
    }
    (*pptrZ)->wordlen = max_len; // The result size will be size of the smaller operand
    (*pptrZ)->sign = ptrX->sign && ptrY->sign; // Negative if both operands are negative
}

void add_carry(WORD x, WORD y, WORD k, WORD* ptrQ, WORD* ptrR) {
    WORD sum = x + y;
	*ptrQ = 0x00;
	if(sum < x)
		*ptrQ = 0x01;
	sum += k;
	*ptrR = sum;
	if(sum < k)
		*ptrQ += 0x01;
}
void add_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "add_core_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "add_core_xyz");
    if (!compare_abs_bint(*pptrX,*pptrY)) {
        add_core_xyz(pptrY, pptrX, pptrZ);
        return;
    }  

    init_bint(pptrZ, (*pptrX)->wordlen + 1);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "add_core_xyz");

    WORD res = 0x00;
    WORD carry = 0x00;
    int k = 0x00;

    for(int i = 0; i < (*pptrY)->wordlen; i++) {
        add_carry((*pptrX)->val[i], (*pptrY)->val[i], k, &carry, &res);
        (*pptrZ)->val[i] = res;
        // printf("-After: X[%d] + Y[%d] + k = %x  + %x + %x = %x * W + %x, Z[%d]: %x\n",i, i,ptrX->val[i], ptrY->val[i], k, carry, res,i,ptrZ->val[i]);
        k = carry;
    }
    for (int i = (*pptrY)->wordlen; i < (*pptrX)->wordlen; i++) {
        add_carry((*pptrX)->val[i],(WORD)0, k, &carry, &res);
        (*pptrZ)->val[i] = res;
        k = carry;
    }
    (*pptrZ)->val[(*pptrX)->wordlen] = k;
    refineBINT(*pptrZ);
}

void ADD(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "ADD");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "ADD");
    if (!compare_abs_bint(*pptrX, *pptrY)) {
        ADD(pptrY, pptrX, pptrZ);
        swapBINT(pptrX,pptrY);
        return;
    }    
    int n = (*pptrX)->wordlen;

    init_bint(pptrZ, n+1);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "ADD");
    
    if ((*pptrX)->sign == (*pptrY)->sign) {
        // If signs are the same, add the numbers
        add_core_xyz(pptrX, pptrY, pptrZ);
        if ((*pptrX)->sign) {
            // If both numbers are negative, then result is negative
            (*pptrZ)->sign = true;
        }
    } else {
        // If signs are different, subtract the numbers
        sub_core_xyz(pptrX, pptrY, pptrZ);
        if ((*pptrX)->sign) {
            // If X is negative and Y is positive, then result is negative
            (*pptrZ)->sign = true;
        }
    }
}

void sub_borrow(WORD x, WORD y, WORD* ptrQ, WORD* ptrR) {
    WORD tmp = (WORD)x - *ptrQ;
    *ptrQ = 0x00;
    if (x < tmp)
        *ptrQ = 0x01;
    if (tmp < y)
        *ptrQ += 0x01;
    tmp = (WORD)tmp-y;
    *ptrR = tmp;
}
void sub_core_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "sub_core_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "sub_core_xyz");
    int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;
    bool xGeqy = compare_abs_bint(*pptrX, *pptrY);
    if (!xGeqy) {
        sub_core_xyz(pptrY, pptrX, pptrZ);
        swapBINT(pptrX,pptrY);
        return;
    }  

    init_bint(pptrZ, MAXIMUM(n,m));
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "sub_core_xyz");
    
    WORD res = 0x00;
    WORD borrow = 0x00;

    matchSize(*pptrX,*pptrY);
    for(int i = 0; i < m; i++) {
        sub_borrow((*pptrX)->val[i], (*pptrY)->val[i], &borrow, &res);
        (*pptrZ)->val[i] = res;
    }
    for(int i = m; i < n; i++) {
        sub_borrow((*pptrX)->val[i], (WORD)0, &borrow, &res);
        (*pptrZ)->val[i] = res;
    }
    refineBINT(*pptrX); refineBINT(*pptrY);
    if(!xGeqy) swapBINT(pptrX,pptrY);
}
void SUB(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "SUB");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "SUB");
    int n = (*pptrX)->wordlen;
    int m = (*pptrY)->wordlen;
    bool sgnX = (*pptrX)->sign;
    bool sgnY = (*pptrY)->sign;
    
    init_bint(pptrZ, MAXIMUM(n,m));
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "SUB");


    bool xGeqy = compare_bint(*pptrX, *pptrY);
    if ((*pptrY)->sign == false && xGeqy) {
        sub_core_xyz(pptrX,pptrY,pptrZ);
    } else if ((*pptrX)->sign == false && !xGeqy) {
        sub_core_xyz(pptrY,pptrX,pptrZ);
        (*pptrZ)->sign = true;
    } else if ((*pptrX)->sign == true && xGeqy) {
        (*pptrX)->sign = false;
        (*pptrY)->sign = false;
        sub_core_xyz(pptrY,pptrX,pptrZ);
    } else if ((*pptrY)->sign == true && !xGeqy) {
        (*pptrX)->sign = false;
        (*pptrY)->sign = false;
        sub_core_xyz(pptrX,pptrY,pptrZ);
        (*pptrZ)->sign = true;
    } else if ((*pptrX)->sign == false && (*pptrY)->sign == true) {
        (*pptrY)->sign = false;
        ADD(pptrX,pptrY,pptrZ);
    } else {
        (*pptrX)->sign = false;
        ADD(pptrX,pptrY,pptrZ);
        (*pptrZ)->sign = true;
    }

    (*pptrX)->sign = sgnX;
    (*pptrY)->sign = sgnY;
}


void mul_xyz(WORD valX, WORD valY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_xyz");

    const int half_w = WORD_BITLEN / 2; // if w=32, half_w = 16 = 2^4
	const WORD MASK = (WORD_ONE << half_w) - 1;
	
    // Split the WORDs into halves
	WORD X0 = valX & MASK;
	WORD X1 = valX >> half_w;
	WORD Y0 = valY & MASK;
	WORD Y1 = valY >> half_w;

    // Cross multiplication
	WORD T0 = X0 * Y1;
	WORD T1 = X1 * Y0;
	T0 = T0 + T1;
	T1 = T0 < T1; // overflow

    // Direct multiplication
	WORD Z0 = X0 * Y0;
	WORD Z1 = X1 * Y1;
	
    // Adjust for overflows
	WORD T = Z0;
	Z0 += (T0 << half_w);
	Z1 += (T1 << half_w) + (T0 >> half_w) + (Z0 < T);
	
    // Set results
	(*pptrZ)->val[0] = Z0;
	(*pptrZ)->val[1] = Z1;
}
void mul_core_TxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "mul_core_TxtBk_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "mul_core_TxtBk_xyz");
    int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;

    init_bint(pptrZ, n+m);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_core_TxtBk_xyz");

    BINT* ptrWordMul = NULL;
    BINT* ptrTemp = NULL;
    init_bint(&ptrTemp, m+n);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            init_bint(&ptrWordMul, 2);
            mul_xyz((*pptrX)->val[i], (*pptrY)->val[j], &ptrWordMul);
            left_shift_word(&ptrWordMul, (i+j));
            add_core_xyz(pptrZ, &ptrWordMul , &ptrTemp);
            copyBINT(pptrZ,&ptrTemp);
        }
    }
    delete_bint(&ptrWordMul);
    delete_bint(&ptrTemp);
    if((*pptrX)->sign != (*pptrY)->sign)
        (*pptrZ)->sign = true;
}

void MUL_Core_ImpTxtBk_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "mul_core_ImpTxtBk_test");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "mul_core_ImpTxtBk_test");
    matchSize(*pptrX, *pptrY);
    makeEven(*pptrX); makeEven(*pptrY);
    int n = (*pptrX)->wordlen; int m = (*pptrX)->wordlen;

    init_bint(pptrZ, n+m);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "mul_core_ImpTxtBk_test");

    int p = n / 2; int q = n / 2;

    BINT* ptrT = NULL;
    init_bint(&ptrT, n+m);
    BINT* ptrT0 = NULL;
    init_bint(&ptrT0, 2*p);
    BINT* ptrT1 = NULL;
    init_bint(&ptrT1, 2*p+1);
    BINT* ptrTmp0 = NULL;
    init_bint(&ptrTmp0, 2*p);
    BINT* ptrTmp1 = NULL;
    init_bint(&ptrTmp1, 2*p+1);
    BINT* ptrTmpZ = NULL;
    init_bint(&ptrTmpZ, (*pptrZ)->wordlen);

    for(int j = 0; j < 2 * q; j++) {
        for(int k = 0; k < p; k++) {
            resetBINT(ptrTmp0); resetBINT(ptrTmp1);
            mul_xyz((*pptrX)->val[2*k], (*pptrY)->val[j], &ptrTmp0);
            mul_xyz((*pptrX)->val[2*k+1], (*pptrY)->val[j], &ptrTmp1);
            if (!k) {
                copyBINT(&ptrT0, &ptrTmp0);
                copyBINT(&ptrT1, &ptrTmp1);
            } else {
                left_shift_word(&ptrTmp0, 2*k);
                refine_BINT_word(ptrTmp0, 2*k);
                OR_BINT(ptrTmp0, ptrT0, &ptrT0);
                left_shift_word(&ptrTmp1, 2*k);
                refine_BINT_word(ptrTmp1, 2*k);
                OR_BINT(ptrTmp1, ptrT1, &ptrT1);
            }
        }
        left_shift_word(&ptrT1, 1);

        ADD(&ptrT1, &ptrT0, &ptrT);
        
        left_shift_word(&ptrT, j);

        copyBINT(&ptrTmpZ, pptrZ);
        ADD(&ptrTmpZ, &ptrT, pptrZ);
    }
    // Cleanup
    delete_bint(&ptrT);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrTmp0); delete_bint(&ptrTmp1);
    delete_bint(&ptrTmpZ);
    refineBINT(*pptrX); refineBINT(*pptrY); refineBINT(*pptrZ);
    if((*pptrX)->sign != (*pptrY)->sign)
        (*pptrZ)->sign = true;
}

void MUL_Core_Krtsb_xyz(BINT** pptrX, BINT** pptrY, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "MUL_Core_Krtsb_xyz");
    CHECK_PTR_AND_DEREF(pptrY, "pptrY", "MUL_Core_Krtsb_xyz");
    int n = (*pptrX)->wordlen; int m = (*pptrY)->wordlen;
    if (FLAG >= MINIMUM(n,m)) {
        BINT* tmpTxtBk_X = NULL; BINT* tmpTxtBk_Y = NULL;
        copyBINT(&tmpTxtBk_X, pptrX); copyBINT(&tmpTxtBk_Y, pptrY);
        MUL_Core_ImpTxtBk_xyz(&tmpTxtBk_X,&tmpTxtBk_Y,pptrZ);
        delete_bint(&tmpTxtBk_X); delete_bint(&tmpTxtBk_Y);
        return;
    }
    init_bint(pptrZ, n+m);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "MUL_Core_Krtsb_xyz");
    int l = (MAXIMUM(n,m) + 1) >> 1;

    BINT* ptrX0 = NULL; BINT* ptrX1 = NULL;
    BINT* ptrY0 = NULL; BINT* ptrY1 = NULL;
    BINT* ptrT0 = NULL; BINT* ptrT1 = NULL;
    BINT* ptrShiftT1 = NULL;
    BINT* ptrS0 = NULL; BINT* ptrS1 = NULL;
    BINT* ptrS = NULL;
    init_bint(&ptrS, 2*l);

    BINT* ptrR = NULL;
    BINT* ptrTmpR = NULL;
    init_bint(&ptrR, (*pptrZ)->wordlen);
    BINT* ptrTmpST1 = NULL;
    BINT* ptrTmpST0 = NULL;

    matchSize(*pptrX, *pptrY);
    copyBINT(&ptrX1, pptrX); right_shift_word(&ptrX1, l);
    copyBINT(&ptrX0, pptrX); reduction(&ptrX0, l * WORD_BITLEN);
    
    copyBINT(&ptrY1, pptrY); right_shift_word(&ptrY1, l);
    copyBINT(&ptrY0, pptrY); reduction(&ptrY0, l * WORD_BITLEN);
    
    MUL_Core_Krtsb_xyz(&ptrX1, &ptrY1, &ptrT1);
    MUL_Core_Krtsb_xyz(&ptrX0, &ptrY0, &ptrT0);
    
    copyBINT(&ptrShiftT1, &ptrT1);
    left_shift_word(&ptrShiftT1, 2*l);
    ADD(&ptrShiftT1, &ptrT0, &ptrR);
    
    SUB(&ptrX0, &ptrX1, &ptrS1);
    SUB(&ptrY1, &ptrY0, &ptrS0);
    
    bool sgn_S = ((ptrS0)->sign) ^ ((ptrS1)->sign);
    ptrS0->sign = false; ptrS1->sign = false;
    MUL_Core_Krtsb_xyz(&ptrS1, &ptrS0, &ptrS);
    ptrS->sign = sgn_S;
    
    ADD(&ptrS,&ptrT1,&ptrTmpST1);
    copyBINT(&ptrS,&ptrTmpST1);
    
    copyBINT(&ptrTmpST0, &ptrS);
    ADD(&ptrTmpST0,&ptrT0,&ptrS);
    
    left_shift_word(&ptrS, l);
    
    ADD(&ptrR, &ptrS, pptrZ);
    
    delete_bint(&ptrX0); delete_bint(&ptrX1);
    delete_bint(&ptrY0); delete_bint(&ptrY1);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrShiftT1);
    delete_bint(&ptrS0); delete_bint(&ptrS1);
    delete_bint(&ptrS);
    delete_bint(&ptrR); delete_bint(&ptrTmpR);
    delete_bint(&ptrTmpST0); delete_bint(&ptrTmpST1);
    refineBINT(*pptrX); refineBINT(*pptrY); refineBINT(*pptrZ);
}

void squ_core(WORD valX, BINT** pptrZ) {
    const int half_w = WORD_BITLEN / 2; // if w=32, half_w = 16 = 2^4
	const WORD MASK = (WORD_ONE << half_w) - 1;

	BINT* C = NULL;
    init_bint(&C,2);
    BINT* T = NULL;
    init_bint(&T,2);
    BINT* Temp = NULL;
    init_bint(&Temp,2);
    // Split the WORDs into halves
	WORD X0 = valX & MASK;
	WORD X1 = valX >> half_w;
	

    // Cross multiplication
	WORD C1 = X1 * X1;
	WORD C0 = X0 * X0;
    C->val[0] = C0;
    C->val[1] = C1;
	T->val[0] = X0 * X1;
    left_shift_bit(&T,half_w + 1); 
    ADD(&C,&T,&Temp);
    
    copyBINT(pptrZ,&Temp);
    delete_bint(&C);
    delete_bint(&T);
    delete_bint(&Temp);
}

void SQU_TxtBk_xz(BINT** pptrX, BINT** pptrZ) {
    BINT* C1 = NULL;
    init_bint(&C1, 1);
    BINT* C2 = NULL;
    init_bint(&C2, 1);
    BINT* T1 = NULL;
    BINT* T2=NULL;
    init_bint(&T2, 2);
    BINT* Temp = NULL;
    BINT* Temp2 = NULL;
    for(int j=0; j < (*pptrX)->wordlen; j++) {
        squ_core((*pptrX)->val[j],&T1);
        left_shift_word(&T1,(2*j));
        add_core_xyz(&T1,&C1,&Temp);
        copyBINT(&C1,&Temp);
        for (int i = j+1; i < (*pptrX)->wordlen; i++) {
            mul_xyz((*pptrX)->val[j],(*pptrX)->val[i],&T2);
            left_shift_word(&T2,(i+j));
            add_core_xyz(&C2,&T2,&Temp2);
            copyBINT(&C2,&Temp2);
            init_bint(&T2,2);
        }
    }
    left_shift_bit(&C2,1);
    add_core_xyz(&C1,&C2,pptrZ);
    delete_bint(&C1);
    delete_bint(&C2);
    delete_bint(&T1);
    delete_bint(&T2);
    delete_bint(&Temp);
    delete_bint(&Temp2);
}

void SQU_Krtsb_xz(BINT** pptrX, BINT** pptrZ) {
    CHECK_PTR_AND_DEREF(pptrX, "pptrX", "SQU_Krtsb_xz");
    int n = (*pptrX)->wordlen;
    static int lenZ = -1; // Declare lenZ as a static variable
    if (lenZ == -1) {
        lenZ = 2 * n;
        init_bint(pptrZ, lenZ);
        CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "SQU_Krtsb_xz");
    }
    if (FLAG >= n) {
        BINT* tmpTxtBk_X = NULL; copyBINT(&tmpTxtBk_X, pptrX);
        SQU_TxtBk_xz(&tmpTxtBk_X, pptrZ);
        delete_bint(&tmpTxtBk_X);
        return;
    }
    init_bint(pptrZ, 2*n);
    CHECK_PTR_AND_DEREF(pptrZ, "pptrZ", "SQU_Krtsb_xz");
    int l = (n + 1) >> 1;

    BINT* ptrX0 = NULL; BINT* ptrX1 = NULL;
    BINT* ptrT0 = NULL; BINT* ptrT1 = NULL;
    BINT* ptrShiftT1 = NULL;
    BINT* ptrR = NULL;
    BINT* ptrS = NULL;
    init_bint(&ptrS, 2*l);

    copyBINT(&ptrX1, pptrX); right_shift_word(&ptrX1, l);
    copyBINT(&ptrX0, pptrX); reduction(&ptrX0, l * WORD_BITLEN);

    SQU_Krtsb_xz(&ptrX1, &ptrT1);
    SQU_Krtsb_xz(&ptrX0, &ptrT0);

    copyBINT(&ptrShiftT1, &ptrT1);
    left_shift_word(&ptrShiftT1, 2*l);
    ADD(&ptrShiftT1, &ptrT0, &ptrR);

    MUL_Core_Krtsb_xyz(&ptrX1, &ptrX0, &ptrS);
    left_shift_word(&ptrS, l);
    left_shift_bit(&ptrS, 1);

    ADD(&ptrR, &ptrS, pptrZ);

    delete_bint(&ptrX0); delete_bint(&ptrX1);
    delete_bint(&ptrT0); delete_bint(&ptrT1);
    delete_bint(&ptrShiftT1);
    delete_bint(&ptrS);
    delete_bint(&ptrR);
}

void DIV_Binary_Long(BINT** pptrDividend, BINT** pptrDivisor, BINT** pptrQ, BINT** pptrR) {
    if((*pptrDivisor)->wordlen == 0 || ((*pptrDivisor)->wordlen == 1 && (*pptrDivisor)->val[0] == 0) ) {
        fprintf(stderr, "Division by zero error.\n");
        exit(1);
    }
    if (!(*pptrDividend)->sign && compare_bint(*pptrDivisor, *pptrDividend)) {
        init_bint(pptrQ, 1);
        copyBINT(pptrR, pptrDividend);
        return;
    }
    int n = (*pptrDividend)->wordlen;
    init_bint(pptrQ, 1);
    (*pptrQ)-> sign = (*pptrDividend)->sign ^ (*pptrDivisor)->sign;
    init_bint(pptrR, 1);
    
    BINT* ptrTmpSub = NULL;
    init_bint(&ptrTmpSub, 1);
    BINT* ptrTmpAdd = NULL;
    init_bint(&ptrTmpAdd, 1);

    matchSize(*pptrDividend,*pptrDivisor);
    for(int i = n * WORD_BITLEN - 1; i >= 0 ; i--) {
        left_shift_bit(pptrR, 1);  // R <- 2R
        (*pptrR)->val[0] ^= GET_BIT(*pptrDividend, i); // R <- R + x_i
        
        matchSize(*pptrR,*pptrDivisor);
        if(compare_bint(*pptrR, *pptrDivisor)) {   // R >= Y
            SUB(pptrR,pptrDivisor,&ptrTmpSub);
            copyBINT(pptrR,&ptrTmpSub);

            init_bint(&ptrTmpAdd, 1);
            ptrTmpAdd->val[0] = 0x01;
            left_shift_bit(&ptrTmpAdd, i);
            matchSize(*pptrQ, ptrTmpAdd);
            for(int j = 0; j < ptrTmpAdd->wordlen; j++) {
                (*pptrQ)->val[j] ^= ptrTmpAdd->val[j];
            }
        }
    }
    refineBINT(*pptrQ);
    refineBINT(*pptrR);

    delete_bint(&ptrTmpAdd);
    delete_bint(&ptrTmpSub);
}

void EEA(BINT** ptrX, BINT** ptrY, BINT** ptrS, BINT** ptrT, BINT** ptrR) {
    BINT *r1 = NULL, *r2 = NULL;
    BINT *s1 = NULL, *s2 = NULL;
    BINT *t1 = NULL, *t2 = NULL;   
    BINT *q = NULL;
    
    init_bint(&s1, 1);
    s1->val[0] = 0x01;
    init_bint(&t2, 1);
    s1->val[0] = 0x01;
    
    init_bint(&s2, 1);
    init_bint(&t1, 1);
    copyBINT(&r1, ptrX);
    copyBINT(&r2, ptrY); 

    while ((r2)->wordlen != 1 || (r2)->val[0] != 0) {
        BINT* temp = NULL;
        DIV_Binary_Long(&r1,&r2,&q, ptrR);

        copyBINT(&r1, &r2);
        copyBINT(&r2, ptrR);

        MUL_Core_ImpTxtBk_xyz(&q, &s2, &temp);

        SUB(&s1, &temp, ptrS);
        
        copyBINT(&s1, &s2);
        copyBINT(&s2, ptrS);
        
        MUL_Core_ImpTxtBk_xyz(&q, &t2, &temp);
        SUB(&t1, &temp, ptrT);

        copyBINT(&t1, &t2);
        copyBINT(&t2, ptrT);
        
        delete_bint(&temp);
        
        refineBINT(r2);
    }
    
    copyBINT(ptrR, &r1);
    copyBINT(ptrS, &s1);
    copyBINT(ptrT, &t1);    
    
    delete_bint(&q);
    delete_bint(&r1);
    delete_bint(&r2);
    delete_bint(&s1);
    delete_bint(&s2);
    delete_bint(&t1);
    delete_bint(&t2);
}
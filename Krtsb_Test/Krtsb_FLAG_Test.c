int main() {
    srand((unsigned int)time(NULL));

    clock_t start1, end1;
    clock_t start2, end2;
    double cpu_time_used1;
    double cpu_time_used2;
    int t = 10000;

    BINT* ptrX = NULL;
    BINT* ptrY = NULL;
    BINT* ptrZ = NULL;
    BINT* ptrTmpX = NULL;
    BINT* ptrTmpY = NULL;
    BINT* ptrTmpZ = NULL;
    BINT* ptrTTmpX = NULL;
    BINT* ptrTTmpY = NULL;
    BINT* ptrTTmpZ = NULL;
    /**
     * if WORD_BITLEN = 32,
     * 0x010 -> ( 1 * 16 =  16) -> (16 *  32 =   512-bit)
     * 0x020 -> ( 2 * 16 =  32) -> (32 *  32 =  1024-bit)
     * 0x040 -> ( 4 * 16 =  64) -> (32 *  64 =  2048-bit)
     * 0x060 -> ( 6 * 16 =  96) -> (32 *  96 =  3072-bit)
     * 0x0f0 -> (15 * 16 = 240) -> (32 * 240 =  7680-bit)
     * 
     * 0x1e0 -> ( 1 * 256 + 14 * 16 = 480) -> (32 * 480 = 15360-bit)
    */
    
    int idx = 0;
    while(idx < t) {
/*************************** Random Input **************************************/
        // int len1 = (rand() % 0x020) + 0x010; //  512 ~ 1024 bits
        // int len2 = (rand() % 0x020) + 0x010; //  512 ~ 1024 bits
        // int len1 = (rand() % 0x040) + 0x020; // 1024 ~ 2048 bits
        // int len2 = (rand() % 0x040) + 0x020; // 1024 ~ 2048 bits
        // int len1 = (rand() % 0x060) + 0x040; // 2048 ~ 3072 bits
        // int len2 = (rand() % 0x060) + 0x040; // 2048 ~ 3072 bits
        // int len1 = (rand() % 0x0f0) + 0x060; // 3072 ~ 7680 bits
        // int len2 = (rand() % 0x0f0) + 0x060; // 3072 ~ 7680 bits
        // int len1 = (rand() % 0x1e0) + 0x0f0; // 7680 ~ 15360 bits
        // int len2 = (rand() % 0x1e0) + 0x0f0; // 7680 ~ 15360 bits
        
        // int len1 = (rand() % 0x08) + 0x3;
        // int len2 = (rand() % 0x08) + 0x3;
        
        int len1 = 0x01a;
        int len2 = 0x01a;
        
        // RANDOM_BINT(&ptrX, false, len1);
        // RANDOM_BINT(&ptrY, false, len2);
        
        // int sgn1 = rand() % 0x02;
        // int sgn2 = rand() % 0x02;
        RANDOM_BINT(&ptrX, false, len1);
        RANDOM_BINT(&ptrY, false, len2);
     
        copyBINT(&ptrTmpX, &ptrX);      
        copyBINT(&ptrTmpY, &ptrY);
        copyBINT(&ptrTTmpX, &ptrX);      
        copyBINT(&ptrTTmpY, &ptrY);

        start1 = clock();
        MUL_Core_ImpTxtBk_xyz(&ptrX,&ptrY,&ptrZ);
        end1 = clock();
        cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;

        start2 = clock();
        Krtsb_FLAG_Test(&ptrTmpX,&ptrTmpY,&ptrTmpZ, 10);
        end2 = clock();
        cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
        delete_bint(&ptrX);
        delete_bint(&ptrY);
        delete_bint(&ptrZ);
        delete_bint(&ptrTmpX);
        delete_bint(&ptrTmpY);
        delete_bint(&ptrTmpZ);
        printf("%.6f\n", cpu_time_used1-cpu_time_used2);
        idx++;
    }
}
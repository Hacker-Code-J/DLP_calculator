#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NO_ELEMENT -1

typedef struct {
    int index;
    int value;
} Pair;

void show_list(Pair *L, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d, ", L[i].value);
    }
    printf("\n");
}

int bin_search(Pair *list, int size, int target) {
    if (size < 1) {
        return NO_ELEMENT;
    }

    int mid = (size - 1) >> 1;

    if (list[mid].value == target) {
        return list[mid].index;
    }

    if (list[mid].value < target) {
        return bin_search(list + mid + 1, size - mid - 1, target);
    } else {
        return bin_search(list, mid, target);
    }
}

void dlp_bsgs(int p, int g, int h) {
    int m = 1 + (int)floor(sqrt(p - 1));
    printf("m=%d = 0x%x\n", m, m);
    
    Pair *L = malloc(m * sizeof(Pair));
    int t = 1;

    for (int j = 0; j < m; j++) {
        L[j].index = j;
        L[j].value = t;
        t = (t * g) % p;
    }

    printf("L_b=\n");
    show_list(L, m);

    // 값에 기반한 L 정렬
    for (int i = 0; i < m - 1; i++) {
        for (int j = 0; j < m - i - 1; j++) {
            if (L[j].value > L[j + 1].value) {
                Pair temp = L[j];
                L[j] = L[j + 1];
                L[j + 1] = temp;
            }
        }
    }

    printf("정렬된 L_b=\n");
    show_list(L, m);

    int inv_g = g; // TODO: 확장 유클리드 알고리즘을 구현하여 모듈러 역수 찾기
    int w = (int)pow(inv_g, m) % p;
    t = h;
    char *root = "UNKNOWN";

    Pair *L2 = malloc(m * sizeof(Pair));

    for (int j = 0; j < m; j++) {
        L2[j].index = j;
        L2[j].value = t;
        int i = bin_search(L, m, t);

        if (i != NO_ELEMENT) {
            printf("L_g=\n");
            show_list(L2, m);
            printf("(i, g^i) = (%d, %d)\n", i, (int)pow(g, i) % p);
            printf("(j, h(g^(-m))^j) = (%d, %d)\n", j, t);
            root = "FOUND"; // 실제 계산으로 업데이트 필요
            break;
        }
        
        t = (t * w) % p;
    }

    printf("k=%s\n", root);
    printf("%d\n", (int)pow(g, atoi(root)) % p == h);

    free(L);
    free(L2);
}

int main() {
    // 문제: g^x = h, GF(p) 상에서
    srand(time(NULL)); // 시드 초기화
    int p = rand() % 1076 + 2; // 2^20 이하의 임의의 소수
    int g = rand() % (p - 1) + 1; // p에 대한 임의의 원시근
    int k = rand() % (p - 2) + 2; // 임의의 개인 키
    int h = (int)pow(g, k) % p; // 공개 키 계산

    printf("[DLP] %d^%d = %d, GF(%d)\n", g, k, h, p);
    dlp_bsgs(p, g, h);

    return 0;
}
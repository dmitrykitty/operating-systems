#include <stdio.h> 
#include <stdlib.h>

#define MAX_BITS 32

void find_set_lsb_msb(int* lsb, int* msb, int num) {
    int n = 0; 
    while (n < MAX_BITS) {
        //or if(num & (1 << n))
        if(((num >> n) & 1) == 1) {
            *lsb = n; 
            break;
        }
        n++;
    }

    while(n < MAX_BITS) {
        if(((num >> n) & 1) == 1) {
            *msb = n; 
        }
        n++;
    }
}



int main(void) {
    int n, lsb = -1, msb = -1; 

    int in = scanf("%d", &n);
    if(in != 1) {
        printf("1 arg!");
        abort();
    }

    find_set_lsb_msb(&lsb, &msb, n);

    if(lsb == -1) {
        printf("NO");
    } else {
        printf("%d %d", msb, lsb);
    }
    return 0; 
}
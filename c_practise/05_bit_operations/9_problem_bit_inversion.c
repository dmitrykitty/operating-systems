#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void invert_bit(unsigned char* arr, int idx, int sz) {
    if(idx < 0 || idx >= sz * 8) {
        return;
    }

    int num_idx = idx / 8; 
    int bit_idx = idx % 8; 

    arr[num_idx] ^= (1 << bit_idx);
}

int main(void) {
    int sz, idx; 
    unsigned char* arr; 

    int in = scanf("%d", &sz); 
    if(in != 1) {
        printf("1 arg"); 
        abort();
    }

    if (sz <= 0) {
        printf("Invalid size\n");
        abort();
    }

    arr = (unsigned char*)(malloc(sz)); //each number not more then 1 byte!
    if(arr == NULL) {
        perror("malloc");
        abort(); 
    }

    for(int i = 0; i < sz; ++i) {
        in = scanf("%hhu", &arr[i]); 
        if(in != 1) {
            printf("1 arg"); 
            free(arr);
            abort();
        }
    }

    in = scanf("%d", &idx); 
    if(in != 1) {
        printf("1 arg"); 
        free(arr);
        abort();
    }

    invert_bit(arr, idx, sz);

    for(int i = 0; i < sz; ++i) {
        printf("%hhu ", arr[i]);
    }

    free(arr);

    return 0; 
}
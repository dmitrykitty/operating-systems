#include <stdio.h>
#include <stdlib.h>

void print_converted_to_fs(unsigned x){
    int i = 0;
    unsigned fac = 1;
    size_t sz;  

    while(fac * (i + 1) <= x){
        i++; 
        fac *= i; 
        
    }

    unsigned* arr = (unsigned*)malloc(sizeof(unsigned) * i); 
    if(arr == NULL){
        perror("malloc");
    }

    sz = i; 

    for(int j = 0; j < sz; j++){
        unsigned cur_coef = 0; 
        while(fac * (cur_coef + 1) <= x){
            cur_coef++;
        }
        arr[j] = cur_coef; 
        if(cur_coef != 0){
            x %= (fac * cur_coef); 
        }
        fac /= i; 
        i--; 
    }

    for(int j = 0; j < sz; j++){
        printf("%u.", arr[j]);
    }
}

int main(){
    unsigned x; 

    short ic = scanf("%u", &x); 
    if(ic != 1){
        perror("scanf"); 
    }

    print_converted_to_fs(x); 
}
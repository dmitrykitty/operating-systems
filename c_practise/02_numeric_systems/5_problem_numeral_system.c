#include <stdio.h>
#include <stdlib.h>

void print_converted(unsigned x, unsigned y){
    size_t sz = 1; 
    unsigned y_check = y; 
    while(y_check < x){
        sz++; 
        y_check *= y; 
    }

    unsigned* arr = (unsigned*)malloc(sz * sizeof(unsigned)); 
    if(arr == NULL){
        perror("malloc"); 
        abort();
    }

    for(int i = sz - 1; i >= 0; i--){
        arr[i] = x % y; 
        x /= y; 
    }

    for(int i = 0; i < sz; i++){
        printf("%u", arr[i]);
    }
}


int main(){
    unsigned int x, y; 
    int ic = scanf("%u%u", &x, &y); 
    print_converted(x, y); 
}
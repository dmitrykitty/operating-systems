#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 64

int fill_fib_and_get_last(int* arr, int sz, int target){
    int idx = 2;
    arr[0] = 1; arr[1] = 2; 
    
    while(idx < MAX_SIZE){
        int next = arr[idx - 2] + arr[idx - 1];

        if(next > target){
            break;
        }
        arr[idx] = next;
        idx++;
    } 
    return idx - 1; 
}

void print_fs(int* arr, int last, int target) {
    for(last; last >= 0; last--){
        if(target >= arr[last]){
            printf("1");
            target -= arr[last];
        } else {
            printf("0");
        }
    }
}

int main(){
    int x, max_index;
    int arr[MAX_SIZE]; 

    int ic = scanf("%d", &x); 
    if(ic != 1){
        printf("scanf: 1 arg required");
        abort();
    }
    
    if(x == 1){
        printf("1");
        return 0;
    }

    max_index = fill_fib_and_get_last(arr, MAX_SIZE, x);
    print_fs(arr, max_index, x);
}
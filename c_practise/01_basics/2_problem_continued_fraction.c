#include <stdio.h>
#include <assert.h>

void cf(int x, int y){
    assert(y != 0); 
    while(y != 0){
        printf("%d ", x / y); 
        int tmp = x % y; 
        x = y; 
        y = tmp; 
    }
}

int main(){
    int x, y; 

    int count = scanf("%d%d", &x, &y); 
    if(count != 2 || y == 0){
        perror("Usage: <int a> <int b> where b != 0"); 
    }

    cf(x, y); 
}
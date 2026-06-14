#include <stdio.h>
#include <stdlib.h>

int is_prime(int n){
    for(int i = 2; i * i <= n; i++){
        if((n % i) == 0){
            return 0;
        }
    }
    return 1;
}


int n_pr(int n){
    int counter = 0, num = 2;
    
    while(1){
        if(is_prime(num)){
            counter++;
        }
        if(counter == n) break;
        num++;
    }
    return num;
}


int main(){
    int n; 
    int i = scanf("%d", &n); 

    if(i != 1){
        printf("1 arg only");
        abort();
    }

    int res = n_pr(n);
    printf("%d", res);
}
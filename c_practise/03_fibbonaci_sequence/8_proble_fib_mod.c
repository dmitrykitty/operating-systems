#include <stdio.h>

int fib_pow_x_mod_m(int x, int m){
    int first = 0, second = 1, i, new; 

    for(i = 0; i < x; i++){
        printf("%d: %d\n", i, first);
        new = (first + second) % m; 
        first = second; 
        second = new;
    }
    return first;
}

int main(){
    int x, m, res;

    int ic= scanf("%d%d", &x, &m);
    if(ic != 2){
        perror("scanf");
        return 1; 
    }

    res = fib_pow_x_mod_m(x, m);
    printf("%d", res);
}
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long llu;

llu find_pisano_period(llu m){
    llu first = 0, second = 1, n = 0, new; 

    while(1){
        new = (first + second) % m; 
        first = second; 
        second = new; 
        n++; 

        if(first == 0 && second == 1){
            break;
        }
    }

    return n; 
}

llu find_x_fib_by_mod(llu x, llu m){
    if(x == 0){
        return 0;
    }

    llu zero = 0, first = 1, new; 

    for(int i = 1; i < x; i++){
        new = (zero + first) % m;
        zero = first; 
        first = new;
    }

    return first; 
};

int main(){
    llu x, m; 

    int ic = scanf("%llu%llu", &x, &m);

    if(ic != 2){
        printf("scanf: 2 arguments requiared");
        abort();
    }

    llu pisano_period = find_pisano_period(m);
    llu fib_num_x = find_x_fib_by_mod(x % pisano_period, m);
    printf("%llu %llu", fib_num_x, pisano_period);
}
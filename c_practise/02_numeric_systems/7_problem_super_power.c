#include <stdio.h>

int super_power_mod(int a, int b, int n){
    int pow = 1; 
    while(b > 1){
        pow *= a; 
        b--;
    }
    
    int res = 1;
    int mult = a % n; 

    while(pow > 0){
        if(pow & 1u){
            res = (res * mult) % n; 
        }
        pow >>= 1; 
        mult = (mult * mult) % n; 
    }
    return res; 
}

int main(){
    int a, b, n, res; 

    int ic = scanf("%d%d%d", &a, &b, &n);
    if(ic != 3){
        perror("scanf"); 
        return 1; 
    }

    res = super_power_mod(a, b, n);
    printf("%d\n", res);
}
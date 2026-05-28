#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef long long ll; 


ll e_abs(ll num){
    return num > 0 ? num : -num; 
}

#if 0
ll e_mod(ll a, ll b){
    ll q = a % b; 
    if(a < 0){
        q+= e_abs(b); 
    }
    return q; 
}
#endif 

void extended_gcd(ll a, ll b){
    //a = a * 1 + b * 0
    //b = a * 0 + b * 1
    //gcd(a, b) = a * u + b * v = r[n-1]

    //r[k] = r[k-2] - q[k] * r[k-1]
    //u[k] = u[k-2] - q[k] * u[k-1]
    //v[k] = v[k-2] - q[k] * v[k-1]

    ll v_old = 0, u_old = 1, v = 1, u = 0, q, r, u_temp, v_temp;
    ll sign_a = a >= 0 ? 1 : -1; 
    ll sign_b = b >= 0 ? 1 : -1; 
    a = e_abs(a);
    b = e_abs(b);
    
    while(b != 0){
        q = a / b; 
        r = a % b; 

        a = b; 
        b = r;

        u_temp = u_old - q * u; 
        u_old = u;
        u = u_temp;

        v_temp = v_old - q * v; 
        v_old = v;
        v = v_temp;
    }

    printf("%lld %lld %lld\n", u_old * sign_a, v_old * sign_b, a);
}

int main() {
    ll a, b;
  
    int ic = scanf("%lld %lld", &a, &b);
    if(ic != 2){
        perror("scanf");
        return 1;
    }
    extended_gcd(a, b);
}
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef long long ll; 

ll gcd_rec(ll x, ll y) {
    ll q;
    if (y > x)
        return gcd_rec(y, x);
    assert (y != 0);
    q = x % y;
    if (q == 0)
        return y;
    return gcd_rec(y, q);
}

ll iabs(ll num){
    return num >= 0 ? num : -num; 
}

ll gcd(ll x, ll y){
    assert (y != 0);

    while(y != 0){
        ll tmp = x % y; 
        x = y; 
        y = tmp;  
    }
    return x; 
}

ll gcd_loop(ll x, ll y) {
    x = iabs(x);
    y = iabs(y);
    return x > y ? gcd(x, y) : gcd(y, x); 
}

int main() {
    ll x = 0, y = 0, rec, loop;
    int res;
  
    res = scanf("%llu %llu", &x, &y);
    assert(res == 2);
    loop = gcd_loop(x, y);
    printf("%llu\n", loop);
    return 0;
}
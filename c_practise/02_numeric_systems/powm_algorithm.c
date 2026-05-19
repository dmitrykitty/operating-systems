unsigned pow_mod(unsigned n, unsigned k, unsigned m){
    unsigned mult = n % m; 
    unsigned prod = 1; 
    while(k > 0){
        if((k % 2) == 1){
            prod = (prod * mult) % m; 
            k--; 
        }
        mult = (mult * mult) % m; 
        k /= 2; 
    }
    return prod; 
}

int main(){

}
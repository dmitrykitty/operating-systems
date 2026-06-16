struct sieve_t {
    int n;
    char *s;
};

void fill_sieve(struct sieve_t *sv){
    sv->s[0] = 1; sv->s[1] = 1; 

    for(int i = 2; i * i < sv->n; i++){
        if(sv->s[i] == 1) continue;

        for(int j = i * i; j < sv->n; j+= i){
            sv->s[j] = 1; 
        }
    }
}

int nth_prime(struct sieve_t *sv, int N){
    int counter = 0;
    
    for(int i = 2; i < sv->n; ++i){
        if(sv->s[i] == 0){
            counter++;
        }

        if(counter == N){
            return i;
        }
    }

    return 0;
}
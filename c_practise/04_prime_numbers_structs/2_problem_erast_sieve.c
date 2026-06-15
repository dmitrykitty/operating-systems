#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int sz; 
    char* sieve;
} Sieve;

void process_sieve(Sieve* s){
    for(int i = 2; (long long)i * i < s->sz; ++i){
        if(s->sieve[i] == 1) continue;

        for(int j = i * i; j < s->sz; j += i){
            s->sieve[j] = 1;
        }
    }
}

int count_prime_numbers(Sieve* s){
    int res = 0;
    for(int i = 2; i < s->sz; ++i){
        if(s->sieve[i] == 0){
            res++;
        }
    }
    return res;
}

int main(){
    int n, res = 0; 

    int i_res = scanf("%d", &n);
    if(i_res != 1){
        printf("1 arg!");
        abort();
    }

    char* arr = calloc(n + 1, sizeof(char)); 
    if(arr == NULL){
        perror("calloc");
        abort();
    }

    Sieve s = {n + 1, arr};

    process_sieve(&s);
    res = count_prime_numbers(&s);

    printf("%d", res);

    free(arr);
}

#include <stdio.h> 
#include <stdlib.h>

#define CUR_INDEX(i) ((i) / 8)
#define CUR_BIT(i) ((i) % 8)


struct sieve_t {
    int n; 
    unsigned char* s; 
}; 

int is_bit_turn_on(struct sieve_t* S, int i) {
    return S->s[CUR_INDEX(i)] & (1 << CUR_BIT(i));
}

void turn_on_nth_bit(struct sieve_t* S, int i) {
    S->s[CUR_INDEX(i)] |= (1 << CUR_BIT(i));
}

void fill_sieve(struct sieve_t* sv) {
    long bits_amount = (long)sv->n * 8; 

    turn_on_nth_bit(sv, 0); 
    turn_on_nth_bit(sv, 1);

    for(int i = 2; (long)i * i < bits_amount; ++i) {
        if(!is_bit_turn_on(sv, i)) {
            for(long j = (long)i * i; j < bits_amount; j += i) {
                turn_on_nth_bit(sv, j);
            }
        }
    }
}

int get_nth_prime(struct sieve_t *sv, unsigned n) {
    if (n == 0) {
        return -1;
    }

    long bits_amount = (long)sv->n * 8; 
    int count = 0;

    for(int i = 0; i < bits_amount; ++i) {
        if(!is_bit_turn_on(sv, i)) {
            ++count;

            if(count == n) {
                return i; 
            }
        }

        
    }
    return -1;
}



int main(void) {
    struct sieve_t S; 

    int in = scanf("%d", &S.n); 
    if(in != 1) {
        printf("1 arg");
        abort();
    }

    S.s = calloc(S.n, sizeof(char)); 
    if(S.s == NULL) {
        perror("calloc");
        abort();
    }

    fill_sieve(&S); 

    int num_idx;
    in = scanf("%d", &num_idx); 
    if(in != 1) {
        printf("1 arg");
        free(S.s);
        abort();
    }

    int res = get_nth_prime(&S, num_idx);
    printf("%d", res);
    free(S.s);
}
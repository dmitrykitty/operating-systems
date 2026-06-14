#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x00;
    int x01;
    int x10;
    int x11;
} Matrix;

typedef long long LL;

/*
|a00 a01| x |b00 b01| = |(a00 * b00 + a01 * b10)  (a00 * b01 + a01 * b11)|
|a10 a11|   |b10 b11|   |(a10 * b00 + a11 * b10)  (a10 * b01 + a11 * b11)|
*/
void AxB(Matrix* A, Matrix* B, int m){
    int res00 = ((LL)A->x00 * B->x00 + (LL)A->x01 * B->x10) % m;
    int res01 = ((LL)A->x00 * B->x01 + (LL)A->x01 * B->x11) % m;
    int res10 = ((LL)A->x10 * B->x00 + (LL)A->x11 * B->x10) % m; 
    int res11 = ((LL)A->x10 * B->x01 + (LL)A->x11 * B->x11) % m; 

    A->x00 = res00;
    A->x01 = res01;
    A->x10 = res10;
    A->x11 = res11;
}

void matrix_pow_n_mod_m(Matrix* A, int n, int m){
    /*
    1 0
    0 1
    */
    Matrix res = {1, 0, 0, 1};

    while (n != 0){
        if((n & 1) == 1){
            AxB(&res, A, m);
        }
        n >>= 1; 
        AxB(A, A, m);
    }

    printf("%d %d\n%d %d", res.x00, res.x01, res.x10, res.x11);
}


int main(){
    int x00, x01, x10, x11, n, m; 

    int io_res = scanf("%d%d%d%d%d%d", &x00, &x01, &x10, &x11, &n, &m);

    if(io_res != 6){
        printf("6 args needed!");
        abort();
    }

    Matrix A = {x00, x01, x10, x11};
    matrix_pow_n_mod_m(&A, n, m);
}
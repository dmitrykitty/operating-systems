#include <stdio.h>
#include <stdlib.h>

void read_input(int* pa, int* pb); 

int main(){
    int a, b, p, q; 
    read_input(&a, &b); 

    p = a / b; 
    q = a % b; 

    printf("p=%d, q=%d\n", p, q); 
 
    return 0; 
}

void read_input(int* pa, int* pb){
    printf("Input a and b: "); 
    int scaned = scanf("%d%d", pa, pb);

    if(scaned != 2 || *pb == 0){
        perror("usage: <int> <int> where b != 0");
        abort(); 
    } 
}
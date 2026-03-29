#include <stdio.h>
#include <unistd.h>

int main(){
    printf("Początek\n");
    printf("Proces %d działa\n", getpid());
    printf("fork\n");
    fork();
    //two processes executed in parallel
    printf("Proces %d działa\n", getpid());
    sleep(5);
    printf("Kończenie działania procesu %d\n", getpid());

    return 0;
}
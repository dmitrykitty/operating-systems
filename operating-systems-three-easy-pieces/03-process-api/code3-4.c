#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(){
    printf("parent process(%d) started\n", getpid()); 

    int rc = fork(); //creating child process 

    if(rc < 0){
        perror("fork");
    }else if(rc > 0){
        printf("this process(%d) is parent of process(%d)\n", getpid(), rc);
        wait(NULL);  
    } else {
        printf("it's child process(%d)\n", getpid()); 
        close(STDOUT_FILENO); 

        int out_file = open("out.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644); 
        if(out_file < 0){
            perror("opening/creating file"); 
            _exit(1);
        }
        char* myargs[] = {"wc", "code3-4.c", NULL}; 
        execvp(myargs[0], myargs); 

        fprintf(stderr, "something went wrong\n"); 
    }
    return 0;
}
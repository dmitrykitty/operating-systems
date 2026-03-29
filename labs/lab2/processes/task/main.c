#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "Args missed. Correct format: program <catalog_name>");
        return 1;
    }
    
    char* catalog_name = argv[1]; 
    int cn_len = strlen(catalog_name); 
    struct dirent* entity; 
    DIR* entity_stream; 

    entity_stream = opendir(catalog_name); 

    if(entity_stream == NULL){
        fprintf(stderr, "This catalog '%s' does not exist", catalog_name); 
    }

    while((entity = readdir(entity_stream)) != NULL){
        char* entity_name = entity -> d_name; 
        int en_len = strlen(entity_name);
        if(strcmp(".", entity_name) == 0 || strcmp("..", entity_name) == 0) {
            continue;
        } 
        if(en_len < 4 || (strcmp(".txt", entity_name + en_len - 4) != 0)) {
            continue;
        }

        //EOF == \0
        char full_path[cn_len + en_len + 2]; 

        struct stat st;
        if (stat(full_path, &st) == -1) {
            perror(full_path);
            continue;
        }
        if (S_ISDIR(st.st_mode)) {
            continue;
        }

        sprintf(full_path, "%s/%s", catalog_name, entity_name); 

        //creating new process
        //pid < 0 - error
        //pid == 0 - inside child process
        //pid > 0 - inside parent process
        pid_t pid = fork();
        if(pid < 0){
            fprintf(stderr, "Error during opening new process occured"); 
        }
        if(pid == 0){
            printf("Process(%d) is working with file '%s'", getpid(), full_path); 
            change_eol(full_path); 
        }
    }

    closedir(entity_stream);
    return 0;
}

void change_eol(char* file_path){
    int in = open(file_path, O_RDONLY);
    if(in < 0){
        fprintf(stderr, "Error during opening '%s' occured.", file_path); 
        return; 
    }

    unsigned char b1;
    unsigned char b2;
    while(read(in, &b1, 1) == 1){
        if(b1 == '\r'){
            if(read(in, &b2, 1) == 1){
                if(b2 == '\n'){
                    unsigned char lf = '\n';
                    if(write(out, &lf, 1) != 1){
                        fprintf(stderr, "Error during writing to '%s' occured.", ); 
                        close(in);
                        close(out); 
                        return; 
                    }
                } else {
                    //b1 = \r, b2 = some other byte
                    if((write(out, &b1, 1) != 1) && (write(out, &b2, 1) != 1))
                        fprintf(stderr, "Error during writing to '%s' occured.", ); 
                        close(in);
                        close(out); 
                        return; 
                }
            } else {
                //file ends with \r
                if(write(out, &b1, 1) != 1){
                    fprintf(stderr, "Error during writing to '%s' occured.", ); 
                    close(in);
                    close(out); 
                    return; 
                }
                break;
            }
        } else {
            if(write(out, &b1, 1) != 1){
                fprintf(stderr, "Error during writing to '%s' occured.", ); 
                close(in);
                close(out); 
                return; 
            }
        }
    }
}


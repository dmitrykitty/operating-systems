#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PATH_SIZE 4096
void change_eol(const char* file_path);

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "Args missed. Correct format: program <catalog_name>\n");
        return 1;
    }
    
    char* catalog_name = argv[1]; 
    int cn_len = strlen(catalog_name); 
    struct dirent* entity; 
    DIR* entity_stream; 

    entity_stream = opendir(catalog_name); 

    if(entity_stream == NULL){
        fprintf(stderr, "This catalog '%s' does not exist\n", catalog_name);
        return 1; 
    }

    while((entity = readdir(entity_stream)) != NULL){
        char* entity_name = entity -> d_name; 
        printf("Current entity working on: %s\n", entity_name); 
        int en_len = strlen(entity_name);
        if(strcmp(".", entity_name) == 0 || 
            (strcmp("..", entity_name) == 0) || 
            entity -> d_type != DT_REG ||
            en_len < 4 || 
            (strcmp(".txt", entity_name + en_len - 4) != 0)
        ) {
            printf("%s is not .txt file\n", entity_name);
            continue;
        }

        //EOF == \0
        char full_path[PATH_SIZE]; 
        snprintf(full_path, PATH_SIZE, "%s/%s", catalog_name, entity_name); 

        //creating new process
        //pid < 0 - error
        //pid == 0 - inside child process
        //pid > 0 - inside parent process
        pid_t pid = fork();
        if(pid < 0){
            fprintf(stderr, "Error during opening new process occured\n"); 
        }
        if(pid == 0){
            printf("Process(%d) is working with file '%s'\n", getpid(), full_path); 
            change_eol(full_path); 
            _exit(0);
        }
    }

    closedir(entity_stream);
    while (wait(NULL) > 0) {}
    return 0;
}

void change_eol(const char* file_path){
    int in = open(file_path, O_RDONLY);
    if(in < 0){
        fprintf(stderr, "Error during opening '%s' occured.\n", file_path); 
        return; 
    }

    char temp_path[PATH_SIZE];
    snprintf(temp_path, PATH_SIZE, "%s.tmp", file_path);
    int out = open(temp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        fprintf(stderr, "Error during opening or creating '%s' occured.\n", temp_path); 
        close(in);
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
                        fprintf(stderr, "Error during writing to '%s' occured.\n", temp_path); 
                        close(in);
                        close(out); 
                        return; 
                    }
                } else {
                    //b1 = \r, b2 = some other byte
                    if((write(out, &b1, 1) != 1) || (write(out, &b2, 1) != 1)){
                        fprintf(stderr, "Error during writing to '%s' occured.\n", temp_path); 
                        close(in);
                        close(out); 
                        return; 
                    }
                }
            } else {
                //file ends with \r
                if(write(out, &b1, 1) != 1){
                    fprintf(stderr, "Error during writing to '%s' occured.\n", temp_path); 
                    close(in);
                    close(out); 
                    return; 
                }
                break;
            }
        } else {
            if(write(out, &b1, 1) != 1){
                fprintf(stderr, "Error during writing to '%s' occured.\n", temp_path); 
                close(in);
                close(out); 
                return; 
            }
        }
    }

    close(in);
    close(out);

    if (rename(temp_path, file_path) != 0) {
        fprintf(stderr, "Error during renaming '%s' to '%s' occured.\n", temp_path, file_path); 
        return;
    }
}


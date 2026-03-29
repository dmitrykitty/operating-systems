#include <stdio.h>
#include <string.h>
#include <dirent.h>

int main(){

    // Pointer to a structure describing one directory entry
    struct dirent* file; 

    // Pointer representing an opened directory stream
    DIR* path; 

    // Open the current directory (".")
    path = opendir("."); 

    // Check if opening the directory succeeded
    if (path == NULL) {
        perror("opendir");
        return 1;
    }

    // Read directory entries one by one until readdir() returns NULL
    while((file = readdir(path)) != NULL){
        // d_name contains the name of the current entry
        puts(file -> d_name); 
        
        //to checl if it is .txt
        char* entry_name = file -> d_name;
        int len = strlen(entry_name);


        if(entry_name >= 4 && strcmp(".txt", entry_name + len - 4)){
            puts("It's .txt!");
        }
    }

    closedir(path);
    return 0;
}
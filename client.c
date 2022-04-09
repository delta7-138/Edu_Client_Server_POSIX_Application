#include <stdio.h>
#include <unistd.h> // used for fork and execvp calls
#include <sys/wait.h> //for waitpid call
#include <stdlib.h>
#include <string.h>
#include <signal.h> //used for WUNTRACED or WCONTINED in waitpid call 
#define MAX_SIZE 1000 //max command size
#define MAX_ARGS 10

int main(){
    char *command = (char *)malloc(sizeof(char) * MAX_SIZE);
    while(1){
        printf(">> "); 
        fgets(command , MAX_SIZE , stdin);
        command[strcspn(command, "\n")] = 0; //removing newline character from the command to make tokenization easier
        if(strcmp(command , "exit")==0){
            exit(1); 
        }
        //parsing command
        char *token = strtok(command , " ");
        char *argList[MAX_ARGS];
        argList[0] = token; 
        int i = 1; 
        while(token!=NULL){
            token = strtok(NULL , " ");
            argList[i++] = token; //arguments being passed. 
        }
        int size = i-1;

        
    }
    return 0;
}
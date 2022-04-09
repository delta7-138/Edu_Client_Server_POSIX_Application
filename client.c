#include <stdio.h>
#include <unistd.h> // used for fork and execvp calls
#include <sys/wait.h> //for waitpid call
#include <stdlib.h>
#include <string.h>
#include <signal.h> //used for WUNTRACED or WCONTINED in waitpid call 
#define MAX_SIZE 1000 //max command size

int main(){
    char *command = (char *)malloc(sizeof(char) * MAX_SIZE);
    while(1){
        printf(">> "); 
        fgets(command , MAX_SIZE , stdin);
        command[strcspn(command, "\n")] = 0; //removing newline character from the command to make tokenization easier
        if(strcmp(command , "exit")==0){
            return 0; 
        }
        int x = fork(); 
        if(x==0){
            command_parse_and_execute(); 
        }else{
            int status; 
            int w = waitpid(x, &status, WUNTRACED | WCONTINUED); 
            /*wait for child process to exit or get killed 
            WCONTINUED returns status of continued child process
            WUNTRACED returns status of stopped child process 
            status variable to hold the status reurned
            x is pid child process
            */
        }
    }
    return 0;
}
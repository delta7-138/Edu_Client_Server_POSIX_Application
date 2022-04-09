#include <stdio.h>
#include <unistd.h> 
#include <sys/wait.h> 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#define MAX_LINE_SIZE 100
#define PARSE_SUCCESS 0
#define PARSE_FAILURE 1

int MIN_COURSES = 0 , MAX_COURSES = 0; 
int MIN_TEACHERS = 0, MAX_TEACHERS = 0;

int parse_and_update(char buffer[MAX_LINE_SIZE]){
    char *token = strtok(buffer , " ");
    char *argList[2];
    argList[0] = token; 
    int i = 1; 
    while(token!=NULL){
        token = strtok(NULL , " ");
        argList[i++] = token; //arguments being passed. 
    }
    if(strcmp(argList[0] , "MIN_COURSES")==0){
        MIN_COURSES = atoi(argList[1]);
        printf("Reading... MIN_COURSES : %d\n" , MIN_COURSES);
        return PARSE_SUCCESS;
    }else if(strcmp(argList[0], "MAX_COURSES")==0){
        MAX_COURSES = atoi(argList[1]);
        printf("Reading... MAX_COURSES : %d\n" , MAX_COURSES);
        return PARSE_SUCCESS;
    }else if(strcmp(argList[0] , "MIN_TEACHERS")==0){
        MIN_TEACHERS = atoi(argList[1]);
        printf("Reading... MIN_TEACHERS : %d\n" , MIN_TEACHERS);
        return PARSE_SUCCESS;
    }else if(strcmp(argList[0] , "MAX_TEACHERS")==0){
        MAX_TEACHERS = atoi(argList[1]);
        printf("Reading... MAX_TEACHERS : %d\n" , MAX_TEACHERS);
        return PARSE_SUCCESS;
    }
    return PARSE_FAILURE; 
}

int main(){
    /*
    Reads from configuration file
    parses the configuration 
    proceeds to listen to the message queue
    */
   FILE *fd = fopen("config.txt" , "r");
   if(fd==NULL){
       perror("Provide a config.txt file!");
       exit(1);
   }
   char buffer[MAX_LINE_SIZE];
   while(fgets(buffer , MAX_LINE_SIZE , fd)){
       int res = parse_and_update(buffer); 
       if(res){
           perror("Parsing error in config.txt!");
           exit(1);
       }
   }
   printf("config.txt read successfully!...\n");
   printf("Listening on message queue...\n");
   while(1){
       
   }
   return 0;
}
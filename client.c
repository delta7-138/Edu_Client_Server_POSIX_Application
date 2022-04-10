#include "server.h"
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

        mqd_t qd_rx; 

        if ((qd_rx = mq_open (RX_QUEUE_NAME, O_WRONLY)) == -1) {
            perror ("Msq Tx: mq_open (qd_rx)");
            exit (1);
        }
        req_msg_t out_msg;
        strcpy(out_msg.msg_type, argList[0]);   // strcpy(destPtr, srcPtr)
	    sprintf(out_msg.msg_val , "%s %s" , argList[0] , argList[1]);

        if (mq_send (qd_rx, (char *) &out_msg, sizeof(out_msg), 0) == -1) {
            perror ("Unable to send message!");
            continue;
        } 
    }
    return 0;
}
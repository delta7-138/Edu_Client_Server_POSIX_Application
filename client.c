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

        mqd_t qd_tx; 
        mqd_t qd_rx; 

        if ((qd_tx = mq_open (SERV_QUEUE_NAME, O_WRONLY)) == -1) {
            perror ("Msq Tx: mq_open (qd_rx)");
            exit (1);
        }

        int pid = getpid(); 
        char client_queue_name[32]; 
        sprintf(client_queue_name , "/CLIENT_%d" , pid);

        struct mq_attr attr; 
        attr.mq_flags = 0;
        attr.mq_maxmsg = MAX_MESSAGES;
        attr.mq_msgsize = MAX_MSG_SIZE;
        attr.mq_curmsgs = 0;
        if ((qd_rx = mq_open (client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS,
                           &attr)) == -1) {
            perror ("Error in opening listening queue");
            exit (1);
        }

        req_msg_t out_msg;
        req_msg_t in_msg;

        sprintf(out_msg.msg_type, "%d" , pid);   // strcpy(destPtr, srcPtr)
	    sprintf(out_msg.msg_val , "%s %s", argList[0] , argList[1]);

        if (mq_send (qd_tx, (char *) &out_msg, sizeof(out_msg), 0) == -1) {
            perror ("Unable to send message!");
            continue;
        } 

        if(mq_receive (qd_rx,(char *) &in_msg, MAX_MSG_SIZE, NULL) == -1) {
            perror ("Error in receiving message");
            exit (1);
        }

        printf("%s\n" , in_msg.msg_val);
    }
    return 0;
}
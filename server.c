#include "server.h"
#include "data_structure.h"

req_msg_t rx_msg; //global message struct to store command sent by client queue



int MIN_COURSES = 0 , MAX_COURSES = 0; 
int MIN_TEACHERS = 0, MAX_TEACHERS = 0;
sem_t bin_sem; 

int curr_teachers = 0, curr_courses = 0; 
int status = 0; 

void *report_generator(void *msg){
    sem_wait(&bin_sem); 
    printList((struct TeacherNode *)msg , curr_teachers); 
    sem_post(&bin_sem);  
    sleep(10); 
    pthread_exit("Thread exit");
}



int parse_and_update(char buffer[MAX_LINE_SIZE] , struct TeacherNode *tList){
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
    }else if(strcmp(argList[0] , "ADD_COURSE")==0){
        printf("%s\n" , argList[0]);
        if(curr_courses==MAX_COURSES){
            return MAX_LIMIT_REACHED;
        }
        if(curr_teachers==0){
            return NO_TEACHER_ERROR; 
        }

        int res = addCourseNode(argList[1] , tList , curr_teachers , 0 , 0); 
        if(res==0){
            curr_courses++; 
            return PARSE_SUCCESS;
        }
        return PARSE_FAILURE;
    }else if(strcmp(argList[0] , "DEL_COURSE")==0){
        if(curr_courses<MIN_COURSES){
            return MIN_LIMIT_NOT_REACHED; 
        }
        if(curr_teachers==0){
            return NO_TEACHER_ERROR; 
        }

        int res = deleteCourseNode(argList[1] , tList,  curr_teachers); 
        if(res<0){
            return DELETE_ERROR; 
        }
        curr_courses--; 
        return PARSE_SUCCESS; 
    }else if(strcmp(argList[0] , "ADD_TEACHER")==0){
        if(curr_teachers==MAX_TEACHERS){
            return MAX_LIMIT_REACHED; 
        }
        struct TeacherNode node = addTeacherNode(argList[1]); 
        tList[curr_teachers++] = node; 
        return PARSE_SUCCESS; 
    }else if(strcmp(argList[0] , "DEL_TEACHER")==0){
        if(curr_teachers<MIN_TEACHERS){
            return MIN_LIMIT_NOT_REACHED; 
        }
        return PARSE_SUCCESS; 
        int res = deleteTeacherNode(argList[1] , tList , &curr_teachers); 
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
       int res = parse_and_update(buffer , NULL); 
       if(res){
           perror("Parsing error in config.txt!");
           exit(1);
       }
    }
    printf("config.txt read successfully!...\n");
    int res_sem = sem_init(&bin_sem , 0 , 1); 
    if(res_sem!=0){
        perror("error in creating semaphore"); 
        exit(0); 
    }

    struct TeacherNode *teacherList = (struct TeacherNode *)malloc(sizeof(struct TeacherNode) * MAX_TEACHERS);
    mqd_t qd_rx; 
    mqd_t qd_tx; 
    struct mq_attr attr; 

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    if ((qd_rx = mq_open (SERV_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS,
                           &attr)) == -1) {
        perror ("Error in opening listening queue");
        exit (1);
    }
    
    pthread_t report_thread; 
    void *thread_status; 

    req_msg_t in_msg;
    req_msg_t res_msg; 
    
    printf("Listening on message queue...\n");
    while(1){
        pthread_create(&report_thread , NULL , report_generator , teacherList); 
        status = pthread_join(report_thread , &thread_status);

        if(mq_receive (qd_rx,(char *) &in_msg, MAX_MSG_SIZE, NULL) == -1) {
            perror ("Error in receiving message");
            exit (1);
        }

        printf("Received message type : %s\n" , in_msg.msg_type);
        printf("Received message val  : %s\n" , in_msg.msg_val); 

        
        sem_wait(&bin_sem); 
        int res = parse_and_update(in_msg.msg_val , teacherList); 
        sem_post(&bin_sem);
        if(res==PARSE_SUCCESS){
            strcpy(res_msg.msg_val,  "SUCCESS");
        }else{
            strcpy(res_msg.msg_val , "FAILURE");
        }
        status = res; 
        sprintf(res_msg.msg_type , "%d" , status);
        

        char client_queue_name[1000]; 
        sprintf(client_queue_name , "/CLIENT_%s" , in_msg.msg_type);
        printf("%s\n" , client_queue_name); 
        if ((qd_tx = mq_open (client_queue_name,  O_WRONLY)) == -1) {
            perror ("Error in opening sending queue");
            exit (1);
        }

        if (mq_send (qd_tx, (char *) &res_msg, sizeof(res_msg), 0) == -1) {
            perror ("Unable to send message!");
            continue;
        } 
    }
    return 0;
}
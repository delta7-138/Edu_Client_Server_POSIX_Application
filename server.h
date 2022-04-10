#ifndef __SERVER_H_
#define __SERVER_H_

#include <stdio.h>
#include <unistd.h> 
#include <sys/wait.h> 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_LINE_SIZE 100
#define PARSE_SUCCESS 0
#define PARSE_FAILURE 1
#define MAX_LIMIT_REACHED 2
#define MIN_LIMIT_NOT_REACHED 3
#define DUPLICATE_ERROR 4
#define DELETE_ERROR 5
#define NO_TEACHER_ERROR 6

#define IS_PRESENT 1
#define IS_NOT_PRESENT 0

#define MSG_TYPE_LEN 16
#define MSG_VAL_LEN  16


typedef struct{
char msg_type[MSG_TYPE_LEN];
char msg_val[MSG_VAL_LEN];
} req_msg_t;



#define SERV_QUEUE_NAME   "/server_queue_222"
#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 3
#define MAX_MSG_SIZE sizeof(req_msg_t)
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE * MAX_MESSAGES) 

#endif
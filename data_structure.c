#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structure.h"


void printList(struct TeacherNode *teacherList , int size){
    for(int i = 0; i<size; i++){
        struct TeacherNode tch = teacherList[i]; 
        if(tch.delflag==1){
            continue; 
        }
        printf("%s\n" , tch.val); 
        
        struct CourseNode *head = tch.head; 
        while(head!=NULL){
            printf("%s\n" , head->val); 
            head = head->next; 
        }
    }
    printf("\n\n"); 
}

int findTeacherNode(char val[TEACHER_NAME] , struct TeacherNode *tList , int size){
    int index = -1; 
    for(int i = 0; i<size; i++){
        struct TeacherNode tmp = tList[i]; 
        if(strcmp(tmp.val , val)==0 && tmp.delflag!=1){
            index = i; 
            break;
        }
    }
    return index; 
}   

struct TeacherNode addTeacherNode(char val[TEACHER_NAME]){
    struct TeacherNode node; 
    strcpy(node.val , val);
    node.head = NULL; 
    node.course_num = 0;
    node.delflag = 0; //0 means not deleted 
    return node; 
}

int addCourseNode(char val[COURSE_NAME] , struct TeacherNode *teacherList , int size , int flag , int ignore){
    int mincount = 10000 , index = -1;
    //printf("%d" , (teacherList[0]).course_num) ;
    for(int i = 0; i<size; i++){
        if(flag && ignore==i){
            continue; 
        }
        struct TeacherNode tmp = teacherList[i]; 
        if(tmp.delflag==1){
            continue; 
        }
        if(mincount>tmp.course_num){
            mincount = tmp.course_num; 
            index = i; 
        }
    }
    struct CourseNode *newNode = (struct CourseNode *)malloc(sizeof(struct CourseNode));  
    newNode->next = teacherList[index].head; 
    strcpy(newNode->val , val);
    teacherList[index].head = newNode; 
    teacherList[index].course_num++; 
    return 0;
}

int deleteTeacherNode(char val[COURSE_NAME] , struct TeacherNode *teacherList , int *size){
    int index = findTeacherNode(val , teacherList , *size); 
    if(index==-1){
        return index; 
    }
    struct TeacherNode delNode = teacherList[index]; 
    struct CourseNode *head = delNode.head; 
    while(head!=NULL){
        addCourseNode(head->val , teacherList , *size , 1 , index); 
        head = head->next; 
    }
    teacherList[index].delflag = 1; 
}

int deleteCourseNode(char val[COURSE_NAME] , struct TeacherNode *teacherList , int size){
    int flag = -1; 
    for(int i = 0; i<size; i++){
        struct TeacherNode tmp = teacherList[i]; 
        if(tmp.delflag==1){
            continue; 
        }

        struct CourseNode *head = tmp.head; 
        struct CourseNode *prev = NULL; 
        while(head!=NULL){
            if(strcmp(head->val , val)==0){
                flag = 0; 
                if(prev==NULL){
                    teacherList[i].head = head->next; 
                }else{
                    prev->next = head->next; 
                    free(head); 
                }
            }
            prev = head; 
            head = head->next; 
        }
    }
    return flag; 
}

// int main(){
//     struct TeacherNode *teacherList = (struct TeacherNode *)malloc(sizeof(struct TeacherNode) * 100); 
//     teacherList[0] = addTeacherNode("t1"); 
//     teacherList[1] = addTeacherNode("t2"); 
//     teacherList[2] = addTeacherNode("t3");
//     teacherList[3] = addTeacherNode("t4"); 
     
//     addCourseNode("c1" , teacherList , 4 , 0 , 0);
//     addCourseNode("c2" , teacherList , 4 , 0 , 0);
//     addCourseNode("c3" , teacherList , 4 , 0 , 0) ;
//     addCourseNode("c4" , teacherList , 4 , 0 , 0);
//     addCourseNode("c5" , teacherList , 4 , 0 , 0);
//     addCourseNode("c6" , teacherList , 4 , 0 , 0);

//     printList(teacherList , 4); 
//     int size = 4; 
//     deleteTeacherNode("t1" , teacherList , &size); 

//     printList(teacherList , size); 

//     deleteTeacherNode("t2" , teacherList , &size); 
//     printList(teacherList , size); 

//     deleteCourseNode("c1" , teacherList , size); 
//     printList(teacherList , size); 
//     return 0; 
// }
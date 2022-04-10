#ifndef __DS_H_
#define __DS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TEACHER_NAME 10
#define COURSE_NAME 10

struct CourseNode{
    char val[COURSE_NAME]; 
    struct CourseNode *next; //Linked List 
}; 

struct TeacherNode{
    char val[TEACHER_NAME];
    int course_num; 
    struct CourseNode *head;
    int delflag;  
}; 

int deleteCourseNode(char val[COURSE_NAME] , struct TeacherNode *teacherList , int size);
int deleteTeacherNode(char val[COURSE_NAME] , struct TeacherNode *teacherList , int *size);
int addCourseNode(char val[COURSE_NAME] , struct TeacherNode *teacherList , int size , int flag , int ignore); 
struct TeacherNode addTeacherNode(char val[TEACHER_NAME]); 
int findTeacherNode(char val[TEACHER_NAME] , struct TeacherNode *tList , int size); 
void printList(struct TeacherNode *teacherList , int size); 
int isCoursePresent(char val[COURSE_NAME] , struct TeacherNode *tList , int size);

#endif
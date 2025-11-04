#ifndef STUDENT_H
#define STUDENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"

struct Student {
    int roll;
    char name[50];
    int age;
    float marks;
};

void addStudent();
void displayStudents();
void searchStudent();
void modifyStudent();
void deleteStudent();

#endif
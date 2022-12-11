#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h> // qsort �Լ��� ����ϱ� ���� ������.
#include "utils.h"

extern char* tmpFile; // ������ ���� ���� ������Ʈ �� �� ������Ʈ ������ ��� ������ ����.
extern char* scheduleTable[4]; // ������ ������ ��� ��Ƶ�.
extern char* fileTable[3]; // �� �������� �����ϴ� ���� �迭
extern char* wdayMap[7];

// Ȱ���ð��밡 �����ϴ� ������ ����ü. (���� ������, �Ͻ� ������)
// ���������� �� ����ü�� ���� �������� �� �־���� today.txt�� ����.
typedef struct {
    char startTime[20];
    char endTime[20];
    char activity[50];
} TimeSchedule;

// �ؾ��� ���� ������ ����ü.
typedef struct {
    int month;
    int day;
    char activity[50];
    char takenTime[10];
} ToDo;

int insertTimeSchedule(struct tm* date, TimeSchedule schedules[50], FILE* fp, int i);
int insertToDo(struct tm* date, ToDo todoList[50], FILE* toDo);
void transformTime(char strTime[10], int intTime[2]);
int cmpfunc(const void* first, const void* second);
int checkDuplicated(TimeSchedule schedules[50], int size);
int appendToDo(TimeSchedule schedules[50], ToDo todoList[50], int sSize, int todoSize);
void displayToday(struct tm* date, TimeSchedule schedules[50], int size);
void makeToday();
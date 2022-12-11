#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h> // qsort 함수를 사용하기 위해 가져옴.
#include "utils.h"

extern char* tmpFile; // 스케줄 파일 내용 업데이트 할 때 업데이트 사항을 잠시 저장할 파일.
extern char* scheduleTable[4]; // 스케줄 종류를 모두 모아둠.
extern char* fileTable[3]; // 각 스케줄을 저장하는 파일 배열
extern char* wdayMap[7];

// 활동시간대가 존재하는 스케줄 구조체. (고정 스케줄, 일시 스케줄)
// 최종적으로 이 구조체에 오늘 스케줄을 다 넣어놓고 today.txt에 저장.
typedef struct {
    char startTime[20];
    char endTime[20];
    char activity[50];
} TimeSchedule;

// 해야할 일을 저장할 구조체.
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
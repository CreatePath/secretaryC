#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "read.h"

extern char* tmpFile;
extern char* inputTable[3][4]; // 순서대로 고정 스케줄, 일시 스케줄, 해야할 일에서 받아야하는 입력값
extern char* scheduleTable[4]; // 순서대로 오늘 스케줄, 고정 스케줄, 일시 스케줄, 해야할 일
extern char* wdayMap[7];

void clearBuffer();
void getTime(int timeArr[2]);
void getDate(int dateArr[2]);
void displayWDay();
void getWDay(int wdayArr[]);
void getScheduleInput(int scheduleKind, char answer[3][30], int i);
int findSchedule(char wantToChange[3][30], char* fileName, char date[30], int scheduleNum);
int askWhereChange(char* fileName, int scheduleKind, char wantToChange[3][30], char* kind);
void updateFile(char* fileName, int scheduleNum, char change[3][30], char* kind);
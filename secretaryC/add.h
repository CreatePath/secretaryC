#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "utils.h"

extern char* tmpFile;
extern char* inputTable[3][4]; // ������� ���� ������, �Ͻ� ������, �ؾ��� �Ͽ��� �޾ƾ��ϴ� �Է°�

void writeFile(char* fileName, char answer[3][30]);
void addSchedule(char* fileName, int scheduleKind);
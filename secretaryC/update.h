#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "read.h"
#include "utils.h"

extern char* inputTable[3][4]; // ������� ���� ������, �Ͻ� ������, �ؾ��� �Ͽ��� �޾ƾ��ϴ� �Է°�

void updateSchedule(char* fileName, int scheduleKind);
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "read.h"
#include "utils.h"

extern char* inputTable[3][4]; // 순서대로 고정 스케줄, 일시 스케줄, 해야할 일에서 받아야하는 입력값

void updateSchedule(char* fileName, int scheduleKind);
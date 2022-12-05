#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "utils.h"
#include "read.h"

// ������ �����ϴ� �Լ�
void deleteSchedule(char* fileName, int scheduleKind) {
	int scheduleNum = 0; // ���� ����
	char wantToDelete[3][30] = { 0 };
	char* kind = "����";

	// ����ڰ� �Է��� ������ ���Ͽ� �ִ��� �˻�
	scheduleNum = askWhereChange(fileName, scheduleKind, wantToDelete, kind);

	if (scheduleNum == -1) {
		printf("������ �������� �����̽��ϴ�.\n");
		return;
	}

	updateFile(fileName, scheduleNum, wantToDelete, kind);
}
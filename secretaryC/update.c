#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "read.h"
#include "utils.h"

extern char* inputTable[3][4]; // ������� ���� ������, �Ͻ� ������, �ؾ��� �Ͽ��� �޾ƾ��ϴ� �Է°�

// ������ ������ �� ����ڰ� ��� ������ ���� �Է¹޴� �Լ�. ���������� ���� ������Ʈ �Լ��� ����.
void updateSchedule(char* fileName, int scheduleKind) {
	char wantToChange[3][30] = { 0 }; // �����ϰ� ���� ������ ���� �迭
	char change[3][30] = { 0 }; // ���� ������ ���� �迭
	int scheduleNum = 0; // ������ ������ ����.
	char ok = 0;
	char startTime[20] = { 0 }, endTime[20] = { 0 }; // Ȱ�� �ð��� �Է½� ���� �ð��� ���� �ð��� ������ �迭��.
	char* kind = "����"; // ���� �Ǵ� ����. ���⼭�� �����ϴ� ��Ȳ�̹Ƿ� ����.

	// � ����(��¥) �� ��° ������ �ٲܰ��� ����.
	scheduleNum = askWhereChange(fileName, scheduleKind, wantToChange, kind); 

	if (scheduleNum == -1) { // -1�� �޾Ҵٸ� ����â���� ����.
		printf("������ �������� �����̽��ϴ�.\n");
		return;
	}

	strcpy(change[0], wantToChange[0]); // ����(��¥)�� ���ٲٴ� ����
	for (int i = 1; i < 3; ++i) {
		printf("%s�� �����Ͻðھ��?(y/n)>>>", inputTable[scheduleKind][i]);
		scanf(" %c", &ok);

		clearBuffer(); // �Է� ���� ����

		if (ok == 'n' || ok == 'N') {
			strcpy(change[i], wantToChange[i]);
			continue;
		}

		getScheduleInput(scheduleKind, change, i);
	}

	updateFile(fileName, scheduleNum, change, kind); // �������� ����
}
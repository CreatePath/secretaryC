#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "add.h"
#include "read.h"
#include "update.h"
#include "delete.h"
#include "utils.h"
#include "today.h"

char* tmpFile = "writetmp.txt"; // ������ ���� ���� ������Ʈ �� �� ������Ʈ ������ ��� ������ ����.
char* inputTable[3][4] = { // ������� ���� ������, �Ͻ� ������, �ؾ��� �Ͽ��� �޾ƾ��ϴ� �Է°�
		{"����", "Ȱ�� �ð���", "�� ��"},
		{"��¥", "Ȱ�� �ð���", "�� ��"},
		{"��¥", "�� ��", "����ҿ�ð�"}
};
char* scheduleTable[4] = { "���� ������", "���� ������", "�Ͻ� ������", "�ؾ��� ��" }; // ������ ������ ��� ��Ƶ�.
char* fileTable[4] = { "today.txt", "fixed.txt", "temporary.txt", "ToDo.txt" }; // �� �������� �����ϴ� ���� �迭
char* wdayMap[7] = { "��", "��", "ȭ", "��", "��", "��", "��" };


void displayMain(); // ���θ޴��� ������
int selectMenu(); // ����ڷκ��� �޴� ������ �޴� �Լ�
void gotoSchedule(int select); // ���õ� �����ٷ� �̵��ϴ� �Լ�
void gotoUpdateActivity(int select, char* fileName, int scheduleKind); // ������ �߰�/����/���� �� ���õ� �Լ� ȣ��
void displaySub(); // �� �������� ����޴��� ������

int main(void) {
	int select = 0; // ���θ޴����� �Է¹��� ������� ����

	while (1) {
		displayMain();

		select = selectMenu();

		if (select == -1) break;
		else if (select > 3 || select < 0) {
			printf("�߸��� �Է��Դϴ�.\n\n");
			continue;
		}

		gotoSchedule(select);
	}

	printf("���α׷��� �����մϴ�.\n");

	return 0;
}

void displayMain() {
	printf("\n==============================================\n\n");
	printf("�ϰ���� �۾��� �����ϼ���. \n\n");
	printf("[0] ���� �Ϸ� ������ ���� �� Ȯ��\n");
	printf("[1] ���� ������ Ȯ��\n");
	printf("[2] �Ͻ� ������ Ȯ��\n");
	printf("[3] �ؾ��� �� Ȯ��\n");
	printf("[-1] ���α׷� ����\n\n");
	printf("���� (���ڸ� �Է�): ");
}

int selectMenu() {
	int select = 0;
	scanf("%d", &select);
	clearBuffer(); // �Է¹��� ����
	printf("==============================================\n\n");
	return select;
}

void gotoSchedule(int select) {
	char* fileName = NULL;
	char* scheduleName = NULL;
	int selectSub = 0;

	fileName = fileTable[select];
	scheduleName = scheduleTable[select];

	while (1) {
		if (select == 0) { // ���� �������� �������� ���
			makeToday();
			break; // ���� �������� ��ȸ�� ����.
		}

		viewSchedule(fileName, scheduleName); // ���õ� ���� ������

		displaySub();
		selectSub = selectMenu();

		if (selectSub == -1) break;
		else if (selectSub < 0 || selectSub > 2) {
			printf("�߸��� �Է��Դϴ�.\n");
			continue;
		}
		
		gotoUpdateActivity(selectSub, fileName, select-1);
	}
}

void displaySub() {
	printf("[0] ������ �߰�\n");
	printf("[1] ������ ����\n");
	printf("[2] ������ ����\n");
	printf("[-1] ���θ޴��� ���ư���\n");
	printf("���� (���ڸ� �Է�): ");
}

void gotoUpdateActivity(int select, char *fileName, int scheduleKind) {
	void (*funcP[3])(char*, int) = { addSchedule, updateSchedule, deleteSchedule };
	funcP[select](fileName, scheduleKind);
}
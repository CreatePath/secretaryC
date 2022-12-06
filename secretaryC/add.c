#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "utils.h"

extern char* tmpFile;
extern char* inputTable[3][4]; // ������� ���� ������, �Ͻ� ������, �ؾ��� �Ͽ��� �޾ƾ��ϴ� �Է°�

// �߰��� ������ ������ ���Ͽ� �߰��ϴ� �Լ�.
void writeFile(char* fileName, char answer[3][30]) {
	FILE* fp = NULL; // ������ ���� ������
	FILE* tmpf = NULL; // �ӽ÷� ���� �����ص� ���� ������
	char contents[100] = { 0 }, add[100] = { 0 }; // contents:���Ͽ� �� ����, add:�߰��� ����
	char fileBuff[100] = { 0 }; // ���� ���� �޾Ƶ� ����
	char* date = NULL; // ������ �� ���� ���� �����ص� ����. ���� �Ǵ� ��¥�� ��.
	int isFound = 0; // ����ڰ� �Է��� ���� �Ǵ� ��¥�� ���Ͽ��� ã�Ҵ��� �˷��ִ� ����.

	fp = fopen(fileName, "r"); // ������ ������ �б������ ����
	tmpf = fopen(tmpFile, "w+"); // �߰� ������ �ٿ����� �ӽ� ����d

	while (fgets(fileBuff, sizeof(fileBuff), fp) != NULL) { // ������ ���� ������ ���پ� ����
		strcpy(contents, fileBuff); // contents�� ���� ������ ����.

		date = strtok(fileBuff, ","); // ���� ������ ','�� �������� �Ľ���.

		if (strcmp(date, answer[0]) == 0) { // ����ڰ� �Է��� ���� �Ǵ� ��¥�� date�� ��
			isFound = 1; // ����ڰ� �Է��� ���� �Ǵ� ��¥�� ã�Ҵٰ� ǥ��.

			if (contents[strlen(contents) - 1] == '\n') {
				contents[strlen(contents) - 1] = '\0'; // �ٹٲ� ���� ����
			}

			sprintf(add, ",%s,%s\n", answer[1], answer[2]);
			strcat(contents, add);
		}
		fputs(contents, tmpf); // contents ������ �ӽ� ���Ͽ� �߰�.
		memset(contents, 0, sizeof(contents));
		memset(fileBuff, 0, sizeof(fileBuff));
	}

	fclose(fp);
	fclose(tmpf);

	if (isFound == 1) { // ����ڰ� �Է��� ���� �Ǵ� ��¥�� �߰ߵǾ��ٸ�
		fp = fopen(fileName, "w"); // ��������� ������ ���� ����
		tmpf = fopen(tmpFile, "r"); // ���� �߰��� �ӽ� ���� �б������ ����

		while (fgets(fileBuff, sizeof(fileBuff), tmpf) != NULL) { // �ӽ� ������ ���پ� �о
			fputs(fileBuff, fp); // �����ٿ� �ٿ��ֱ�
			memset(fileBuff, 0, sizeof(fileBuff));
		}

		fclose(fp);
		fclose(tmpf);
	}
	else { // ����ڰ� ���ο� ���� �Ǵ� ��¥�� �Է��ߴٸ�
		sprintf(contents, "%s,%s,%s\n", answer[0], answer[1], answer[2]);

		fp = fopen(fileName, "a");

		fputs(contents, fp);

		fclose(fp);
	}
}

// �߰��ϴ� ������ �޴� �Լ�
void addSchedule(char* fileName, int scheduleKind) {
	char answer[3][30] = { 0 }; // inputTable�� �����ϴ� ������� �Է��� ����.
	char ok = 0; // ���� �߰��� ���� ����

	for (int i = 0; i < 3; ++i) {
		getScheduleInput(scheduleKind, answer, i);
	}

	printf("\n�Է¹��� ����\n[ %s: [%s: %s] ]\n", answer[0], answer[1], answer[2]);
	printf("�߰��Ͻðڽ��ϱ�? (y/n)");
	scanf(" %c", &ok);

	if (ok == 'y' || ok == 'Y') {
		writeFile(fileName, answer); // �߰� ������ ���Ͽ� �ݿ�
		printf("�ش� ������ �߰��Ǿ����ϴ�.\n");
	}
	else {
		printf("�ش� ������ �߰����� �ʾҽ��ϴ�.\n");
	}
}
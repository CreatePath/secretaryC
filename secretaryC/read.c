#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <memory.h>


// ������ ������ �ҷ��� ������ �����ִ� �Լ�
void viewSchedule(char* fileName, char* scheduleName) {
	int select = 0;
	FILE* fp;
	char fileBuff[100];
	char* tokPtr = NULL;
	int i = 0;

	memset(fileBuff, 0, sizeof(fileBuff));

	printf("\n%s\n\n", scheduleName);

	fp = fopen(fileName, "r");

	if (fp == NULL) {
		fp = fopen(fileName, "w+"); // �ش� ������ ������ �б�� ���� ���� ������ ����
	}

	// ����: [ "����(��¥)" [ "Ȱ���ð���(����)" "����(����ҿ�ð�)" ] ]
	while (fgets(fileBuff, sizeof(fileBuff), fp) != NULL) { 
		i = 0;
		tokPtr = strtok(fileBuff, ",");
		while (tokPtr != NULL) {
			if (tokPtr[strlen(tokPtr) - 1] == '\n') { // �ٹٲ� ���� ����
				tokPtr[strlen(tokPtr) - 1] = '\0';
				printf("%s ] ]\n", tokPtr);
			}
			else if (i > 0 && i % 2 == 0) {
				printf("%s ], ", tokPtr);
			}
			else {
				printf("[ %s ", tokPtr);
			}
			tokPtr = strtok(NULL, ",");
			i++;
		}
		printf("\n");
		memset(fileBuff, 0, sizeof(fileBuff));
	}

	fclose(fp);

	printf("\n==============================================\n\n");
}
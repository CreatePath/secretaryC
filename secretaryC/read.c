#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <memory.h>


// 스케줄 파일을 불러와 일정을 보여주는 함수
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
		fp = fopen(fileName, "w+"); // 해당 파일이 없으면 읽기와 쓰기 모드로 파일을 생성
	}

	// 형식: [ "요일(날짜)" [ "활동시간대(할일)" "할일(예상소요시간)" ] ]
	while (fgets(fileBuff, sizeof(fileBuff), fp) != NULL) { 
		i = 0;
		tokPtr = strtok(fileBuff, ",");
		while (tokPtr != NULL) {
			if (tokPtr[strlen(tokPtr) - 1] == '\n') { // 줄바꿈 문자 삭제
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
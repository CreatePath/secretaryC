#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "utils.h"

extern char* tmpFile;
extern char* inputTable[3][4]; // 순서대로 고정 스케줄, 일시 스케줄, 해야할 일에서 받아야하는 입력값

// 추가된 일정을 스케줄 파일에 추가하는 함수.
void writeFile(char* fileName, char answer[3][30]) {
	FILE* fp = NULL; // 스케줄 파일 포인터
	FILE* tmpf = NULL; // 임시로 내용 복사해둘 파일 포인터
	char contents[100] = { 0 }, add[100] = { 0 }; // contents:파일에 쓸 내용, add:추가할 조각
	char fileBuff[100] = { 0 }; // 파일 내용 받아둘 버퍼
	char* date = NULL; // 파일의 맨 왼쪽 값을 저장해둘 변수. 요일 또는 날짜가 들어감.
	int isFound = 0; // 사용자가 입력한 요일 또는 날짜를 파일에서 찾았는지 알려주는 변수.

	fp = fopen(fileName, "r"); // 스케줄 파일을 읽기용으로 열기
	tmpf = fopen(tmpFile, "w+"); // 추가 내용을 붙여넣을 임시 파일d

	while (fgets(fileBuff, sizeof(fileBuff), fp) != NULL) { // 스케줄 파일 내용을 한줄씩 읽음
		strcpy(contents, fileBuff); // contents에 읽은 내용을 복사.

		date = strtok(fileBuff, ","); // 읽은 내용을 ','를 기준으로 파싱함.

		if (strcmp(date, answer[0]) == 0) { // 사용자가 입력한 요일 또는 날짜를 date와 비교
			isFound = 1; // 사용자가 입력한 요일 또는 날짜를 찾았다고 표시.

			if (contents[strlen(contents) - 1] == '\n') {
				contents[strlen(contents) - 1] = '\0'; // 줄바꿈 문자 제거
			}

			sprintf(add, ",%s,%s\n", answer[1], answer[2]);
			strcat(contents, add);
		}
		fputs(contents, tmpf); // contents 내용을 임시 파일에 추가.
		memset(contents, 0, sizeof(contents));
		memset(fileBuff, 0, sizeof(fileBuff));
	}

	fclose(fp);
	fclose(tmpf);

	if (isFound == 1) { // 사용자가 입력한 요일 또는 날짜가 발견되었다면
		fp = fopen(fileName, "w"); // 쓰기용으로 스케줄 파일 열기
		tmpf = fopen(tmpFile, "r"); // 내용 추가된 임시 파일 읽기용으로 열기

		while (fgets(fileBuff, sizeof(fileBuff), tmpf) != NULL) { // 임시 파일을 한줄씩 읽어서
			fputs(fileBuff, fp); // 스케줄에 붙여넣기
			memset(fileBuff, 0, sizeof(fileBuff));
		}

		fclose(fp);
		fclose(tmpf);
	}
	else { // 사용자가 새로운 요일 또는 날짜를 입력했다면
		sprintf(contents, "%s,%s,%s\n", answer[0], answer[1], answer[2]);

		fp = fopen(fileName, "a");

		fputs(contents, fp);

		fclose(fp);
	}
}

// 추가하는 일정을 받는 함수
void addSchedule(char* fileName, int scheduleKind) {
	char answer[3][30] = { 0 }; // inputTable에 대응하는 사용자의 입력을 저장.
	char ok = 0; // 정말 추가할 건지 여부

	for (int i = 0; i < 3; ++i) {
		getScheduleInput(scheduleKind, answer, i);
	}

	printf("\n입력받은 일정\n[ %s: [%s: %s] ]\n", answer[0], answer[1], answer[2]);
	printf("추가하시겠습니까? (y/n)");
	scanf(" %c", &ok);

	if (ok == 'y' || ok == 'Y') {
		writeFile(fileName, answer); // 추가 사항을 파일에 반영
		printf("해당 일정이 추가되었습니다.\n");
	}
	else {
		printf("해당 일정이 추가되지 않았습니다.\n");
	}
}
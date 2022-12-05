#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include "read.h"
#include "utils.h"

extern char* inputTable[3][4]; // 순서대로 고정 스케줄, 일시 스케줄, 해야할 일에서 받아야하는 입력값

// 일정을 수정할 때 사용자가 어떻게 수정할 건지 입력받는 함수. 수정사항을 파일 업데이트 함수에 전달.
void updateSchedule(char* fileName, int scheduleKind) {
	char wantToChange[3][30] = { 0 }; // 수정하고 싶은 일정을 담을 배열
	char change[3][30] = { 0 }; // 수정 사항을 담을 배열
	int scheduleNum = 0; // 일정의 순번을 저장.
	char ok = 0;
	char startTime[20] = { 0 }, endTime[20] = { 0 }; // 활동 시간대 입력시 시작 시각과 종료 시각을 저장할 배열들.
	char* kind = "수정"; // 수정 또는 삭제. 여기서는 수정하는 상황이므로 수정.

	// 어떤 요일(날짜) 몇 번째 일정을 바꿀건지 받음.
	scheduleNum = askWhereChange(fileName, scheduleKind, wantToChange, kind); 

	if (scheduleNum == -1) { // -1을 받았다면 선택창으로 복귀.
		printf("일정을 수정하지 않으셨습니다.\n");
		return;
	}

	strcpy(change[0], wantToChange[0]); // 요일(날짜)은 못바꾸니 복사
	for (int i = 1; i < 3; ++i) {
		printf("%s를 수정하시겠어요?(y/n)>>>", inputTable[scheduleKind][i]);
		scanf(" %c", &ok);

		clearBuffer(); // 입력 버퍼 비우기

		if (ok == 'n' || ok == 'N') {
			strcpy(change[i], wantToChange[i]);
			continue;
		}

		getScheduleInput(scheduleKind, change, i);
	}

	updateFile(fileName, scheduleNum, change, kind); // 수정사항 전달
}
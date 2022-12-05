#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "add.h"
#include "read.h"
#include "update.h"
#include "delete.h"
#include "utils.h"
#include "today.h"

char* tmpFile = "writetmp.txt"; // 스케줄 파일 내용 업데이트 할 때 업데이트 사항을 잠시 저장할 파일.
char* inputTable[3][4] = { // 순서대로 고정 스케줄, 일시 스케줄, 해야할 일에서 받아야하는 입력값
		{"요일", "활동 시간대", "할 일"},
		{"날짜", "활동 시간대", "할 일"},
		{"날짜", "할 일", "예상소요시간"}
};
char* scheduleTable[4] = { "오늘 스케줄", "고정 스케줄", "일시 스케줄", "해야할 일" }; // 스케줄 종류를 모두 모아둠.
char* fileTable[4] = { "today.txt", "fixed.txt", "temporary.txt", "ToDo.txt" }; // 각 스케줄을 저장하는 파일 배열
char* wdayMap[7] = { "일", "월", "화", "수", "목", "금", "토" };


void displayMain(); // 메인메뉴를 보여줌
int selectMenu(); // 사용자로부터 메뉴 선택을 받는 함수
void gotoSchedule(int select); // 선택된 스케줄로 이동하는 함수
void gotoUpdateActivity(int select, char* fileName, int scheduleKind); // 스케줄 추가/수정/삭제 중 선택된 함수 호출
void displaySub(); // 각 스케줄의 서브메뉴를 보여줌

int main(void) {
	int select = 0; // 메인메뉴에서 입력받을 사용자의 선택

	while (1) {
		displayMain();

		select = selectMenu();

		if (select == -1) break;
		else if (select > 3 || select < 0) {
			printf("잘못된 입력입니다.\n\n");
			continue;
		}

		gotoSchedule(select);
	}

	printf("프로그램을 종료합니다.\n");

	return 0;
}

void displayMain() {
	printf("\n==============================================\n\n");
	printf("하고싶은 작업을 선택하세요. \n\n");
	printf("[0] 오늘 하루 스케줄 생성 및 확인\n");
	printf("[1] 고정 스케줄 확인\n");
	printf("[2] 일시 스케줄 확인\n");
	printf("[3] 해야할 일 확인\n");
	printf("[-1] 프로그램 종료\n\n");
	printf("선택 (숫자만 입력): ");
}

int selectMenu() {
	int select = 0;
	scanf("%d", &select);
	clearBuffer(); // 입력버퍼 비우기
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
		if (select == 0) { // 오늘 스케줄을 선택했을 경우
			makeToday();
			break; // 오늘 스케줄은 조회만 가능.
		}

		viewSchedule(fileName, scheduleName); // 선택된 일정 보여줌

		displaySub();
		selectSub = selectMenu();

		if (selectSub == -1) break;
		else if (selectSub < 0 || selectSub > 2) {
			printf("잘못된 입력입니다.\n");
			continue;
		}
		
		gotoUpdateActivity(selectSub, fileName, select-1);
	}
}

void displaySub() {
	printf("[0] 스케줄 추가\n");
	printf("[1] 스케줄 수정\n");
	printf("[2] 스케줄 삭제\n");
	printf("[-1] 메인메뉴로 돌아가기\n");
	printf("선택 (숫자만 입력): ");
}

void gotoUpdateActivity(int select, char *fileName, int scheduleKind) {
	void (*funcP[3])(char*, int) = { addSchedule, updateSchedule, deleteSchedule };
	funcP[select](fileName, scheduleKind);
}
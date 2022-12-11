#include "delete.h"

// 일정을 삭제하는 함수
void deleteSchedule(char* fileName, int scheduleKind) {
	int scheduleNum = 0; // 일정 순번
	char wantToDelete[3][30] = { 0 };
	char* kind = "삭제";

	// 사용자가 입력한 일정이 파일에 있는지 검사
	scheduleNum = askWhereChange(fileName, scheduleKind, wantToDelete, kind);

	if (scheduleNum == -1) {
		printf("일정을 삭제하지 않으셨습니다.\n");
		return;
	}

	updateFile(fileName, scheduleNum, wantToDelete, kind);
}
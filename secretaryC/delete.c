#include "delete.h"

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
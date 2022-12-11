#include "utils.h"

// 입력 버퍼 비우는 함수
void clearBuffer(void) { 
	while (getchar() != '\n');
}

void getDate(int dateArr[2]) {
	char ok = 0;

	while (1) {
		printf("'월'을 입력하세요. (1 ~ 12)>>>");
		scanf("%d", &dateArr[0]);
		clearBuffer();

		if (dateArr[0] < 1 || 12 < dateArr[0]) {
			printf("잘못된 입력입니다. 다시 입력해주세요.\n");
			continue;
		}

		printf("'일'을 입력해주세요. (1 ~ 31)>>>");
		scanf("%d", &dateArr[1]);
		clearBuffer();

		if (dateArr[1] < 1 || 31 < dateArr[1]) {
			printf("잘못된 입력입니다. 다시 입력해주세요.\n");
			continue;
		}

		printf("입력하신 날짜: %d/%d\n", dateArr[0], dateArr[1]);
		printf("정확하게 입력하셨나요?(y/n)");
		scanf(" %c", &ok);
		clearBuffer();

		if (ok == 'n' || ok == 'N') {
			printf("다시 입력해주세요.\n");
			continue;
		}

		break;
	}
}

void displayWDay() {
	printf("\n==========");
	for (int k = 0; k < 7; ++k) {
		printf("%s: %d, ", wdayMap[k], k);
	}
	printf("==========\n\n");
}

void getWDay(int wdayArr[]) {
	char ok = 0;

	while (1) {
		displayWDay();

		printf("추가하고 싶은 요일 입력(0 ~ 7)>>>");
		scanf("%d", &wdayArr[0]);
		clearBuffer();

		if (wdayArr[0] < 0 || 7 < wdayArr[0]) {
			printf("잘못된 입력입니다. 다시 입력해주세요.\n");
			continue;
		}

		printf("입력하신 요일: %s\n", wdayMap[wdayArr[0]]);
		printf("정확하게 입력하셨나요? (y/n)");
		scanf(" %c", &ok);
		clearBuffer();

		if (ok == 'n' || ok == 'N') {
			printf("다시 입력해주세요.\n");
			continue;
		}

		break;
	}
}

void getTime(int timeArr[2]) {
	char ok = 0;

	while (1) {
		printf("HH:MM에서 HH 입력(0 ~ 24)>>>");
		scanf("%d", &timeArr[0]);
		clearBuffer();

		if (timeArr[0] < 0 || 24 < timeArr[0]) {
			printf("잘못된 입력입니다. 다시 입력해주세요.\n");
			continue;
		}

		printf("HH:MM에서 MM 입력(0 ~ 60)>>>");
		scanf("%d", &timeArr[1]);
		clearBuffer();

		if (timeArr[1] < 0 || 59 < timeArr[1] || (timeArr[0] == 24 && timeArr[1] > 0)) {
			printf("잘못된 입력입니다. 다시 입력해주세요.\n");
			continue;
		}

		printf("입력하신 시각(시간): %d:%d\n", timeArr[0], timeArr[1]);
		printf("정확하게 입력하셨나요?(y/n)");
		scanf(" %c", &ok);
		clearBuffer();

		if (ok == 'n' || ok == 'N') {
			printf("다시 입력해주세요.\n");
			continue;
		}

		break;
	}
}

// 스케줄 요소를 사용자로부터 입력받는 함수.
void getScheduleInput(int scheduleKind, char answer[3][30], int i) {
	int start[2] = { 0 };
	int end[2] = { 0 };

	printf("\n%s 입력(','는 입력불가)\n", inputTable[scheduleKind][i]);

	while (1) {
		if (strcmp(inputTable[scheduleKind][i], "날짜") == 0) {
			printf("\n날짜 입력 (MM/DD)\n\n");
			getDate(start);
			sprintf(answer[i], "%d/%d", start[0], start[1]);
		}
		else if (strcmp(inputTable[scheduleKind][i], "요일") == 0) {
			getWDay(start);
			sprintf(answer[i], "%s", wdayMap[start[0]]);
		}
		else if (strcmp(inputTable[scheduleKind][i], "활동 시간대") == 0) {
			printf("\n시작 시각 입력(HH:MM)\n\n");
			getTime(start);

			printf("\n종료 시각 입력(HH:MM)\n\n");
			getTime(end);

			if (start[0] * 60 + start[1] > end[0] * 60 + end[1]) {
				printf("시작 시각이 종료 시각보다 빨라야합니다. 다시 입력해주세요.\n");
				continue;
			}

			sprintf(answer[i], "%d:%d ~ %d:%d", start[0], start[1], end[0], end[1]);
		}
		else if (strcmp(inputTable[scheduleKind][i], "예상소요시간") == 0) {
			getTime(start);
			sprintf(answer[i], "%d:%d", start[0], start[1]);
		}
		else {
			gets_s(answer[i], sizeof(answer[i]));

			if (strchr(answer[i], ',')) {
				printf("','는 입력하실 수 없습니다.\n");
				printf("다시 입력해주세요.\n");
				continue;
			}
		}
		break;
	}
}

// 사용자가 수정 또는 삭제해달라고 입력한 일정이 실제로 있는지 확인하는 함수. ex) '월요일 3번째 일정'이 있는지 확인.
int findSchedule(char wantToChange[3][30], char* fileName, char date[30], int scheduleNum) {
	FILE* fp = NULL;
	char fileBuff[100] = { 0 };
	char* fileData = NULL;
	int isFound = 0, i = 0;

	fp = fopen(fileName, "r");

	while (fgets(fileBuff, sizeof(fileBuff), fp) != NULL) {
		fileData = strtok(fileBuff, ",");
		if (strcmp(fileData, date) == 0) { // 읽은 요일(날짜)이 수정하고자 하는 요일(날짜)과 동일하다면
			strcpy(wantToChange[0], date);

			while (i < scheduleNum - 1 && fileData != NULL) { // 수정할 필요 없는 것들은 통과
				fileData = strtok(NULL, ","); // 활동시간대(할일) 
				fileData = strtok(NULL, ","); // 할일(예상소요시간)
				i++;
			}

			// 모두 통과되었다면 while문 탈출. 유저가 수정(삭제)하고자 하는 일정 찾기 실패.
			if (fileData == NULL || fileData[strlen(fileData) - 1] == '\n') break;

			fileData = strtok(NULL, ","); 
			strcpy(wantToChange[1], fileData); // 유저가 찾는 일정의 활동시간대(할일) 저장

			fileData = strtok(NULL, ",");
			strcpy(wantToChange[2], fileData); // 유저가 찾는 일정의 할일(예상소요시간) 저장

			if (wantToChange[2] != NULL) {
				if (wantToChange[2][strlen(wantToChange[2]) - 1] == '\n') {
					wantToChange[2][strlen(wantToChange[2]) - 1] = '\0'; // 줄바꿈문자 제거
				}
				isFound = 1; // 유저가 찾고자하는 일정을 찾았다고 표시
			}

			break;
		}
	}

	fclose(fp);

	if (isFound == 0) { // 일정을 못 찾았을 경우
		printf("'%s'의 '%d번째 일정'을 찾지 못했습니다.\n", date, scheduleNum);
		printf("선택창으로 돌아갑니다.\n");
		return -1;
	}

	return 0;
}

// 수정 또는 삭제 시 어디를 바꿀건지 물어보는 함수
int askWhereChange(char* fileName, int scheduleKind, char wantToChange[3][30], char* kind) {
	char date[30] = { 0 };
	int scheduleNum = 0, isFound = 0;
	char ok = 0;

	while (1) {
		viewSchedule(fileName, scheduleTable[scheduleKind + 1]);

		printf("%s하고 싶은 일정의 %s>>>", kind, inputTable[scheduleKind][0]);
		gets_s(date, sizeof(date));

		printf("'%s'에서 몇번째 일정을 %s하고 싶으신가요?>>>", date, kind);
		scanf("%d", &scheduleNum); // 일정 순번 저장
		clearBuffer();

		if (scheduleNum <= 0) {
			printf("잘못된 입력입니다. 다시 입력해주세요.\n");
			continue;
		}
		break;
	}

	isFound = findSchedule(wantToChange, fileName, date, scheduleNum); // 일정 찾기
	if (isFound == -1) return -1; // 입력받은 요일(날짜)가 없거나, 일정 순번이 없으면 실패.

	printf("[ %s: [%s %s] ]을 %s하시겠어요?(y/n)>>>", wantToChange[0], wantToChange[1], wantToChange[2], kind);
	scanf(" %c", &ok);

	if (ok == 'n' || ok == 'N') return -1; // 수정(삭제) 질문에 no라고 답하면 수정(삭제) 안함

	return scheduleNum; // 일정 순번 넘김.
}

// 파일을 수정 또는 삭제하는 함수.
void updateFile(char* fileName, int scheduleNum, char change[3][30], char* kind) { 
	FILE* fp = NULL;
	FILE* tmpf = NULL;
	char fileBuff[100] = { 0 }, contents[100] = { 0 }, add[100] = { 0 };
	char* fileData = NULL;
	int i = 0;

	fp = fopen(fileName, "r");
	tmpf = fopen(tmpFile, "w");

	while (fgets(fileBuff, sizeof(fileBuff), fp) != NULL) {
		strcpy(contents, fileBuff);

		fileData = strtok(fileBuff, ",");

		if (strcmp(fileData, change[0]) == 0) {
			memset(contents, 0, sizeof(contents));

			sprintf(contents, "%s", fileData);

			while (i < scheduleNum - 1 && fileData != NULL) {
				for (int j = 0; j < 2; ++j) {
					fileData = strtok(NULL, ",");
					sprintf(add, ",%s", fileData);
					strcat(contents, add);
				}

				i++;
			}

			if (kind == "수정") {
				sprintf(add, ",%s,%s", change[1], change[2]);
				strcat(contents, add);
			}

			for (int j = 0; j < 2; ++j) {
				fileData = strtok(NULL, ",");
			}


			if (fileData[strlen(fileData) - 1] == '\n') {
				// 삭제하려는 일정이 사라지면 해당 요일(날짜)의 일정이 사라지는 경우 row자체를 지움. 
				if (kind == "삭제" && scheduleNum == 1) memset(contents, 0, sizeof(contents));
				else strcat(contents, "\n");
			}
			else {
				fileData = strtok(NULL, ",");
				while (fileData != NULL) {
					sprintf(add, ",%s", fileData);
					strcat(contents, add);

					fileData = strtok(NULL, ",");
				}
			}
		}

		fputs(contents, tmpf);

		memset(fileBuff, 0, sizeof(fileBuff));
		memset(contents, 0, sizeof(contents));
	}

	fclose(tmpf);
	fclose(fp);

	fp = fopen(fileName, "w"); // 쓰기용으로 스케줄 파일 열기
	tmpf = fopen(tmpFile, "r"); // 내용 수정된 임시 파일 읽기용으로 열기

	while (fgets(fileBuff, sizeof(fileBuff), tmpf) != NULL) { // 임시 파일을 한줄씩 읽어서
		fputs(fileBuff, fp); // 스케줄에 붙여넣기

		memset(fileBuff, 0, sizeof(fileBuff));
	}

	fclose(fp);
	fclose(tmpf);
}
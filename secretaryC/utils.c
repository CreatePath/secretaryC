#include "utils.h"

// �Է� ���� ���� �Լ�
void clearBuffer(void) { 
	while (getchar() != '\n');
}

void getDate(int dateArr[2]) {
	char ok = 0;

	while (1) {
		printf("'��'�� �Է��ϼ���. (1 ~ 12)>>>");
		scanf("%d", &dateArr[0]);
		clearBuffer();

		if (dateArr[0] < 1 || 12 < dateArr[0]) {
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");
			continue;
		}

		printf("'��'�� �Է����ּ���. (1 ~ 31)>>>");
		scanf("%d", &dateArr[1]);
		clearBuffer();

		if (dateArr[1] < 1 || 31 < dateArr[1]) {
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");
			continue;
		}

		printf("�Է��Ͻ� ��¥: %d/%d\n", dateArr[0], dateArr[1]);
		printf("��Ȯ�ϰ� �Է��ϼ̳���?(y/n)");
		scanf(" %c", &ok);
		clearBuffer();

		if (ok == 'n' || ok == 'N') {
			printf("�ٽ� �Է����ּ���.\n");
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

		printf("�߰��ϰ� ���� ���� �Է�(0 ~ 7)>>>");
		scanf("%d", &wdayArr[0]);
		clearBuffer();

		if (wdayArr[0] < 0 || 7 < wdayArr[0]) {
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");
			continue;
		}

		printf("�Է��Ͻ� ����: %s\n", wdayMap[wdayArr[0]]);
		printf("��Ȯ�ϰ� �Է��ϼ̳���? (y/n)");
		scanf(" %c", &ok);
		clearBuffer();

		if (ok == 'n' || ok == 'N') {
			printf("�ٽ� �Է����ּ���.\n");
			continue;
		}

		break;
	}
}

void getTime(int timeArr[2]) {
	char ok = 0;

	while (1) {
		printf("HH:MM���� HH �Է�(0 ~ 24)>>>");
		scanf("%d", &timeArr[0]);
		clearBuffer();

		if (timeArr[0] < 0 || 24 < timeArr[0]) {
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");
			continue;
		}

		printf("HH:MM���� MM �Է�(0 ~ 60)>>>");
		scanf("%d", &timeArr[1]);
		clearBuffer();

		if (timeArr[1] < 0 || 59 < timeArr[1] || (timeArr[0] == 24 && timeArr[1] > 0)) {
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");
			continue;
		}

		printf("�Է��Ͻ� �ð�(�ð�): %d:%d\n", timeArr[0], timeArr[1]);
		printf("��Ȯ�ϰ� �Է��ϼ̳���?(y/n)");
		scanf(" %c", &ok);
		clearBuffer();

		if (ok == 'n' || ok == 'N') {
			printf("�ٽ� �Է����ּ���.\n");
			continue;
		}

		break;
	}
}

// ������ ��Ҹ� ����ڷκ��� �Է¹޴� �Լ�.
void getScheduleInput(int scheduleKind, char answer[3][30], int i) {
	int start[2] = { 0 };
	int end[2] = { 0 };

	printf("\n%s �Է�(','�� �ԷºҰ�)\n", inputTable[scheduleKind][i]);

	while (1) {
		if (strcmp(inputTable[scheduleKind][i], "��¥") == 0) {
			printf("\n��¥ �Է� (MM/DD)\n\n");
			getDate(start);
			sprintf(answer[i], "%d/%d", start[0], start[1]);
		}
		else if (strcmp(inputTable[scheduleKind][i], "����") == 0) {
			getWDay(start);
			sprintf(answer[i], "%s", wdayMap[start[0]]);
		}
		else if (strcmp(inputTable[scheduleKind][i], "Ȱ�� �ð���") == 0) {
			printf("\n���� �ð� �Է�(HH:MM)\n\n");
			getTime(start);

			printf("\n���� �ð� �Է�(HH:MM)\n\n");
			getTime(end);

			if (start[0] * 60 + start[1] > end[0] * 60 + end[1]) {
				printf("���� �ð��� ���� �ð����� ������մϴ�. �ٽ� �Է����ּ���.\n");
				continue;
			}

			sprintf(answer[i], "%d:%d ~ %d:%d", start[0], start[1], end[0], end[1]);
		}
		else if (strcmp(inputTable[scheduleKind][i], "����ҿ�ð�") == 0) {
			getTime(start);
			sprintf(answer[i], "%d:%d", start[0], start[1]);
		}
		else {
			gets_s(answer[i], sizeof(answer[i]));

			if (strchr(answer[i], ',')) {
				printf("','�� �Է��Ͻ� �� �����ϴ�.\n");
				printf("�ٽ� �Է����ּ���.\n");
				continue;
			}
		}
		break;
	}
}

// ����ڰ� ���� �Ǵ� �����ش޶�� �Է��� ������ ������ �ִ��� Ȯ���ϴ� �Լ�. ex) '������ 3��° ����'�� �ִ��� Ȯ��.
int findSchedule(char wantToChange[3][30], char* fileName, char date[30], int scheduleNum) {
	FILE* fp = NULL;
	char fileBuff[100] = { 0 };
	char* fileData = NULL;
	int isFound = 0, i = 0;

	fp = fopen(fileName, "r");

	while (fgets(fileBuff, sizeof(fileBuff), fp) != NULL) {
		fileData = strtok(fileBuff, ",");
		if (strcmp(fileData, date) == 0) { // ���� ����(��¥)�� �����ϰ��� �ϴ� ����(��¥)�� �����ϴٸ�
			strcpy(wantToChange[0], date);

			while (i < scheduleNum - 1 && fileData != NULL) { // ������ �ʿ� ���� �͵��� ���
				fileData = strtok(NULL, ","); // Ȱ���ð���(����) 
				fileData = strtok(NULL, ","); // ����(����ҿ�ð�)
				i++;
			}

			// ��� ����Ǿ��ٸ� while�� Ż��. ������ ����(����)�ϰ��� �ϴ� ���� ã�� ����.
			if (fileData == NULL || fileData[strlen(fileData) - 1] == '\n') break;

			fileData = strtok(NULL, ","); 
			strcpy(wantToChange[1], fileData); // ������ ã�� ������ Ȱ���ð���(����) ����

			fileData = strtok(NULL, ",");
			strcpy(wantToChange[2], fileData); // ������ ã�� ������ ����(����ҿ�ð�) ����

			if (wantToChange[2] != NULL) {
				if (wantToChange[2][strlen(wantToChange[2]) - 1] == '\n') {
					wantToChange[2][strlen(wantToChange[2]) - 1] = '\0'; // �ٹٲ޹��� ����
				}
				isFound = 1; // ������ ã�����ϴ� ������ ã�Ҵٰ� ǥ��
			}

			break;
		}
	}

	fclose(fp);

	if (isFound == 0) { // ������ �� ã���� ���
		printf("'%s'�� '%d��° ����'�� ã�� ���߽��ϴ�.\n", date, scheduleNum);
		printf("����â���� ���ư��ϴ�.\n");
		return -1;
	}

	return 0;
}

// ���� �Ǵ� ���� �� ��� �ٲܰ��� ����� �Լ�
int askWhereChange(char* fileName, int scheduleKind, char wantToChange[3][30], char* kind) {
	char date[30] = { 0 };
	int scheduleNum = 0, isFound = 0;
	char ok = 0;

	while (1) {
		viewSchedule(fileName, scheduleTable[scheduleKind + 1]);

		printf("%s�ϰ� ���� ������ %s>>>", kind, inputTable[scheduleKind][0]);
		gets_s(date, sizeof(date));

		printf("'%s'���� ���° ������ %s�ϰ� �����Ű���?>>>", date, kind);
		scanf("%d", &scheduleNum); // ���� ���� ����
		clearBuffer();

		if (scheduleNum <= 0) {
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");
			continue;
		}
		break;
	}

	isFound = findSchedule(wantToChange, fileName, date, scheduleNum); // ���� ã��
	if (isFound == -1) return -1; // �Է¹��� ����(��¥)�� ���ų�, ���� ������ ������ ����.

	printf("[ %s: [%s %s] ]�� %s�Ͻðھ��?(y/n)>>>", wantToChange[0], wantToChange[1], wantToChange[2], kind);
	scanf(" %c", &ok);

	if (ok == 'n' || ok == 'N') return -1; // ����(����) ������ no��� ���ϸ� ����(����) ����

	return scheduleNum; // ���� ���� �ѱ�.
}

// ������ ���� �Ǵ� �����ϴ� �Լ�.
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

			if (kind == "����") {
				sprintf(add, ",%s,%s", change[1], change[2]);
				strcat(contents, add);
			}

			for (int j = 0; j < 2; ++j) {
				fileData = strtok(NULL, ",");
			}


			if (fileData[strlen(fileData) - 1] == '\n') {
				// �����Ϸ��� ������ ������� �ش� ����(��¥)�� ������ ������� ��� row��ü�� ����. 
				if (kind == "����" && scheduleNum == 1) memset(contents, 0, sizeof(contents));
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

	fp = fopen(fileName, "w"); // ��������� ������ ���� ����
	tmpf = fopen(tmpFile, "r"); // ���� ������ �ӽ� ���� �б������ ����

	while (fgets(fileBuff, sizeof(fileBuff), tmpf) != NULL) { // �ӽ� ������ ���پ� �о
		fputs(fileBuff, fp); // �����ٿ� �ٿ��ֱ�

		memset(fileBuff, 0, sizeof(fileBuff));
	}

	fclose(fp);
	fclose(tmpf);
}
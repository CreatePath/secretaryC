#include "today.h"

// ���� ������ �Ǵ� �Ͻ� �������� ����ü �迭�� �߰�
int insertTimeSchedule(struct tm* date, TimeSchedule schedules[50], FILE* fp, int i) {
    char fileBuff[100] = { 0 };
    char* fileData = NULL;
    char* start = NULL;
    char* end = NULL;
    char tmp[20] = { 0 };
    char compareDate[10] = { 0 };
    int j = i; // ���߿� ����ü ������ ������ ���� ����ü �迭�� index

    if (i == 0) {
        strcpy(compareDate, wdayMap[date->tm_wday]);
    }
    else {
        sprintf(compareDate, "%d/%d", date->tm_mon + 1, date->tm_mday);
    }

    while (fgets(fileBuff, sizeof(fileBuff), fp) != NULL) {
        fileData = strtok(fileBuff, ",");

        if (strstr(fileData, compareDate) != NULL) {
            fileData = strtok(NULL, ",");
            while (fileData != NULL) {
                strcpy(schedules[i].startTime, fileData);
                fileData = strtok(NULL, ",");

                if (fileData[strlen(fileData) - 1] == '\n') { // �ٹٲ� ����
                    fileData[strlen(fileData) - 1] = '\0';
                }

                strcpy(schedules[i].activity, fileData);
                fileData = strtok(NULL, ",");

                i++;
            }
            break;
        }
    }

    for (;  j < i; ++j) { // ���۽ð��� ����ð� �и� (���߿� ������ �ϱ� ���ؼ�)
        strcpy(tmp, schedules[j].startTime);
        start = strtok(tmp, " ~ ");
        end = strtok(NULL, " ~ ");
        strcpy(schedules[j].startTime, start);
        strcpy(schedules[j].endTime, end);
    }

    return i;
}

// ����ڰ� ���� �ؾ��� ���� �����ϰ� �̸� ����ü �迭�� �ִ� �Լ�
int insertToDo(struct tm* date, ToDo todoList[50], FILE* toDo) {
    char fileBuff[100] = { 0 };
    char cpyFile[100] = { 0 };
    char* fileData = NULL;
    char work[60] = { 0 };
    char timeData[10] = { 0 };
    int month = 0, day = 0;
    char ok = 0;
    int resTime[2] = { 0 };
    int i = 0; // todoList�� index

    while (fgets(fileBuff, sizeof(fileBuff), toDo) != NULL) {
        strcpy(cpyFile, fileBuff); // ���� ���� ����

        fileData = strtok(fileBuff, ","); // �ؾ��� �Ͽ��� ��¥ ���� (MM/DD)

        fileData = strtok(fileData, "/"); // �� ����
        sscanf(fileData, "%d", &month); // month�� int�� ����

        month--; // date�� ����� month�� �츮�� ����ϴ� ������ 1��ŭ �����Ƿ� month--�� ��.

        fileData = strtok(NULL, "/"); // �� ����
        sscanf(fileData, "%d", &day); // day�� int�� ����

        // ������ ��¥�� ���� ��¥ ���Ķ�� (12��->1���� ����� ����..)
        if ((month > date->tm_mon) || (month == date->tm_mon && day >= date->tm_mday)) {
            fileData = strtok(cpyFile, ",");
            fileData = strtok(NULL, ",");

            while (fileData != NULL) {
                strcpy(work, fileData);
                fileData = strtok(NULL, ",");

                strcpy(timeData, fileData);
                fileData = strtok(NULL, ",");

                if (timeData[strlen(timeData) - 1] == '\n') { // �ٹٲ� ���� ����
                    timeData[strlen(timeData) - 1] = '\0';
                }
                
                printf("[%d/%d [%s ����ҿ�ð�: %s]\n", month+1, day, work, timeData);
                printf("���� �� ���� �Ͻǰǰ���? (y/n)\n");
                scanf(" %c", &ok);
                clearBuffer(); // �Է¹��� ����
                
                if (ok == 'y' || ok == 'Y') {
                    printf("�� �ð� �Ͻǰǰ���?(HH:MM)\n");
                    getTime(resTime);

                    strcpy(todoList[i].activity, work); // �� �� ����
                    sprintf(todoList[i].takenTime, "%d:%d", resTime[0], resTime[1]); // �ҿ�ð� ����
                    todoList[i].month = month; // �� ����
                    todoList[i].day = day; // �� ����

                    i++; // todoList�� index ����
                }

            }
        }
    }

    return i;
}

void transformTime(char strTime[10], int intTime[2]) {
    char* p = NULL;

    p = strtok(strTime, ":");
    sscanf(p, "%d", &intTime[0]);

    p = strtok(NULL, ":");
    sscanf(p, "%d", &intTime[1]);
}

// ������ �� �� ������ �Ǵ� �Լ�
int cmpfunc(const void* first, const void* second) {
    TimeSchedule* f = (TimeSchedule*)first;
    TimeSchedule* s = (TimeSchedule*)second;
    char time1[10] = { 0 };
    char time2[10] = { 0 };
    char* p = NULL;
    int intTime1[2] = {0};
    int intTime2[2] = {0};

    strcpy(time1, f->startTime);
    strcpy(time2, s->startTime);

    transformTime(time1, intTime1);
    transformTime(time2, intTime2);

    return (intTime1[0] * 60 + intTime1[1] - intTime2[0] * 60 + intTime2[1]); // ������ ������ ��������, ����� ���������� ����.
}

// ������ ��ġ���� Ȯ���ϴ� �Լ�
int checkDuplicated(TimeSchedule schedules[50], int size) {
    char strSchedule1[10] = { 0 };
    char strSchedule2[10] = { 0 };
    int schedule1[2] = { 0 };
    int schedule2[2] = { 0 };
    int gap = 0;

    for (int j = 0; j < size - 1; ++j) {
        strcpy(strSchedule1, schedules[j].endTime);
        strcpy(strSchedule2, schedules[j + 1].startTime);

        transformTime(strSchedule1, schedule1);
        transformTime(strSchedule2, schedule2);

        gap = (schedule2[0] * 60 + schedule2[1]) - (schedule1[0] * 60 + schedule1[1]);

        if (gap < 0) {
            printf("\n�Ʒ� �� ���� ������ ��Ĩ�ϴ�.\n���� ������ �� �Ͻ� �����ٿ��� �� �� �ϳ��� �����ϰ� �ٽ� ���ּ���.\n\n");

            for (int k = 0; k < 2; ++k) {
                printf("[%s ~ %s %s]\n", schedules[j+k].startTime, schedules[j+k].endTime, schedules[j+k].activity);
            }

            return 1;
        }
    }

    return 0;
}

// todoList�� TimeSchedule�� �����ִ� �Լ�
int appendToDo(TimeSchedule schedules[50], ToDo todoList[50], int sSize, int todoSize) {
    int gap = 0;
    int takenTime = 0;
    int todo[2] = { 0 };
    int schedule1[2] = { 0 };
    int schedule2[2] = { 0 };
    char strSchedule1[10] = { 0 };
    char strSchedule2[10] = { 0 };

    // TimeSchedule�� todoList �����ֱ�
    for (int i = 0; i < todoSize; ++i) {
        strcpy(strSchedule1, todoList[i].takenTime);
        transformTime(strSchedule1, todo);
        takenTime = todo[0] * 60 + todo[1];

        for (int j = 0; j < sSize - 1; ++j) {
            strcpy(strSchedule1, schedules[j].endTime);
            strcpy(strSchedule2, schedules[j + 1].startTime);

            transformTime(strSchedule1, schedule1);
            transformTime(strSchedule2, schedule2);

            gap = (schedule2[0] * 60 + schedule2[1]) - (schedule1[0] * 60 + schedule1[1]);

            if (gap > takenTime) {
                for (int pos = sSize; pos > j + 1; --pos) {
                    schedules[pos] = schedules[pos - 1];
                }

                sprintf(schedules[j+1].startTime, "%d:%d", schedule1[0], schedule1[1]);

                schedule1[0] += (schedule1[1] + takenTime) / 60;
                schedule1[1] = (schedule1[1] + takenTime) % 60;

                sprintf(schedules[j+1].endTime, "%d:%d", schedule1[0], schedule1[1]);
                strcpy(schedules[j+1].activity, todoList[i].activity);

                sSize++;
                break;
            }
            else if (j == sSize - 2) {
                strcpy(strSchedule2, schedules[j + 1].endTime);
                transformTime(strSchedule2, schedule2);

                gap = 24 * 60 - (schedule2[0] * 60 + schedule2[1]);
                    
                if (gap < takenTime) {
                    printf("'%s'��(��) ���� �����ٿ� �߰����� ���߽��ϴ�.\n", todoList[i].activity);
                    break;
                }

                sprintf(schedules[sSize].startTime, "%d:%d", schedule2[0], schedule2[1]);

                schedule2[0] += (schedule2[1] + takenTime) / 60;
                schedule2[1] = (schedule2[1] + takenTime) % 60;
                    
                sprintf(schedules[sSize].endTime, "%d:%d", schedule2[0], schedule2[1]);
                strcpy(schedules[sSize].activity, todoList[i].activity);

                sSize++;
                break;
            }
        }
    }

    return sSize;
}

// ���� ��¥�� ����ϴ� �Լ�.
void displayToday(struct tm* date, TimeSchedule schedules[50], int size) {
    printf("\n==============================================\n\n");

    // ���� ��¥ ���
    printf(
        "%d�� %d�� %d�� %s����\n", date->tm_year + 1900, date->tm_mon + 1, 
        date->tm_mday, wdayMap[date->tm_wday]
    ); 

    // ���� ���� ���
    for (int i = 0; i < size; ++i) {
        printf("[%s ~ %s\t%s]\n", schedules[i].startTime, schedules[i].endTime, schedules[i].activity);
    } 

}

// ���� �������� ����� ������.
void makeToday() {
    struct tm* date;
    const time_t t = time(NULL);
    date = localtime(&t); // ���� �ð��� date�� ����.

    FILE* fixed = fopen(fileTable[0], "r"); // ���� ������ ���� ������
    FILE* tmpSchedule = fopen(fileTable[1], "r"); // �Ͻ� ������ ���� ������
    FILE* toDo = fopen(fileTable[2], "r"); // �ؾ��� �� ���� ������

    TimeSchedule schedules[50]; // Ȱ���ð��밡 ���Ե� �����ٵ��� ���� ����ü �迭
    ToDo todoList[50]; // �ؾ��� �� �����ٵ��� ���� ����ü �迭

    int tsIdx = 0; // schedules index
    int todoIdx = 0; // todoList index
    int isDuplicated = 0; // ������ ��ġ���� ǥ���ϴ� ����

    tsIdx = insertTimeSchedule(date, schedules, fixed, tsIdx); // ���� �������� TimeSchedule ����ü �迭�� �߰�
    tsIdx = insertTimeSchedule(date, schedules, tmpSchedule, tsIdx); // �Ͻ� �������� TimeSchedule ����ü �迭�� �߰�
    todoIdx = insertToDo(date, todoList, toDo); // �ؾ��� ���� ToDo ����ü �迭�� �߰�

    // ���� �ݱ�
    fclose(fixed);
    fclose(tmpSchedule);
    fclose(toDo);

    qsort((TimeSchedule*)schedules, tsIdx, sizeof(TimeSchedule), cmpfunc); // TimeSchedule �迭�� startTime�� ���� ������� ����.

    isDuplicated = checkDuplicated(schedules, tsIdx);
    if (isDuplicated) return; // �ߺ��� ������ ������ �Լ����� ������.

    tsIdx = appendToDo(schedules, todoList, tsIdx, todoIdx); // TimeSchedule �迭�� todoList �����ֱ�

    displayToday(date, schedules, tsIdx); // TimeSchedule �迭���� �ϳ��� ���� ���
}
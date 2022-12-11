#include "today.h"

// 고정 스케줄 또는 일시 스케줄을 구조체 배열에 추가
int insertTimeSchedule(struct tm* date, TimeSchedule schedules[50], FILE* fp, int i) {
    char fileBuff[100] = { 0 };
    char* fileData = NULL;
    char* start = NULL;
    char* end = NULL;
    char tmp[20] = { 0 };
    char compareDate[10] = { 0 };
    int j = i; // 나중에 구조체 데이터 수정을 위한 구조체 배열의 index

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

                if (fileData[strlen(fileData) - 1] == '\n') { // 줄바꿈 제거
                    fileData[strlen(fileData) - 1] = '\0';
                }

                strcpy(schedules[i].activity, fileData);
                fileData = strtok(NULL, ",");

                i++;
            }
            break;
        }
    }

    for (;  j < i; ++j) { // 시작시각과 종료시각 분리 (나중에 정렬을 하기 위해서)
        strcpy(tmp, schedules[j].startTime);
        start = strtok(tmp, " ~ ");
        end = strtok(NULL, " ~ ");
        strcpy(schedules[j].startTime, start);
        strcpy(schedules[j].endTime, end);
    }

    return i;
}

// 사용자가 오늘 해야할 일을 결정하고 이를 구조체 배열에 넣는 함수
int insertToDo(struct tm* date, ToDo todoList[50], FILE* toDo) {
    char fileBuff[100] = { 0 };
    char cpyFile[100] = { 0 };
    char* fileData = NULL;
    char work[60] = { 0 };
    char timeData[10] = { 0 };
    int month = 0, day = 0;
    char ok = 0;
    int resTime[2] = { 0 };
    int i = 0; // todoList의 index

    while (fgets(fileBuff, sizeof(fileBuff), toDo) != NULL) {
        strcpy(cpyFile, fileBuff); // 파일 내용 저장

        fileData = strtok(fileBuff, ","); // 해야할 일에서 날짜 추출 (MM/DD)

        fileData = strtok(fileData, "/"); // 월 추출
        sscanf(fileData, "%d", &month); // month에 int로 저장

        month--; // date에 저장된 month는 우리가 사용하는 월보다 1만큼 작으므로 month--를 함.

        fileData = strtok(NULL, "/"); // 일 추출
        sscanf(fileData, "%d", &day); // day에 int로 저장

        // 추출한 날짜가 현재 날짜 이후라면 (12월->1월은 고려를 안함..)
        if ((month > date->tm_mon) || (month == date->tm_mon && day >= date->tm_mday)) {
            fileData = strtok(cpyFile, ",");
            fileData = strtok(NULL, ",");

            while (fileData != NULL) {
                strcpy(work, fileData);
                fileData = strtok(NULL, ",");

                strcpy(timeData, fileData);
                fileData = strtok(NULL, ",");

                if (timeData[strlen(timeData) - 1] == '\n') { // 줄바꿈 문자 제거
                    timeData[strlen(timeData) - 1] = '\0';
                }
                
                printf("[%d/%d [%s 예상소요시간: %s]\n", month+1, day, work, timeData);
                printf("오늘 이 일을 하실건가요? (y/n)\n");
                scanf(" %c", &ok);
                clearBuffer(); // 입력버퍼 비우기
                
                if (ok == 'y' || ok == 'Y') {
                    printf("몇 시간 하실건가요?(HH:MM)\n");
                    getTime(resTime);

                    strcpy(todoList[i].activity, work); // 할 일 저장
                    sprintf(todoList[i].takenTime, "%d:%d", resTime[0], resTime[1]); // 소요시간 저장
                    todoList[i].month = month; // 월 저장
                    todoList[i].day = day; // 일 저장

                    i++; // todoList의 index 증가
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

// 정렬할 때 비교 기준이 되는 함수
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

    return (intTime1[0] * 60 + intTime1[1] - intTime2[0] * 60 + intTime2[1]); // 음수가 나오면 왼쪽으로, 양수는 오른쪽으로 보냄.
}

// 일정이 겹치는지 확인하는 함수
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
            printf("\n아래 두 개의 일정이 겹칩니다.\n고정 스케줄 및 일시 스케줄에서 둘 중 하나를 수정하고 다시 와주세요.\n\n");

            for (int k = 0; k < 2; ++k) {
                printf("[%s ~ %s %s]\n", schedules[j+k].startTime, schedules[j+k].endTime, schedules[j+k].activity);
            }

            return 1;
        }
    }

    return 0;
}

// todoList를 TimeSchedule에 끼워넣는 함수
int appendToDo(TimeSchedule schedules[50], ToDo todoList[50], int sSize, int todoSize) {
    int gap = 0;
    int takenTime = 0;
    int todo[2] = { 0 };
    int schedule1[2] = { 0 };
    int schedule2[2] = { 0 };
    char strSchedule1[10] = { 0 };
    char strSchedule2[10] = { 0 };

    // TimeSchedule에 todoList 끼워넣기
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
                    printf("'%s'은(는) 오늘 스케줄에 추가하지 못했습니다.\n", todoList[i].activity);
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

// 오늘 날짜를 출력하는 함수.
void displayToday(struct tm* date, TimeSchedule schedules[50], int size) {
    printf("\n==============================================\n\n");

    // 오늘 날짜 출력
    printf(
        "%d년 %d월 %d일 %s요일\n", date->tm_year + 1900, date->tm_mon + 1, 
        date->tm_mday, wdayMap[date->tm_wday]
    ); 

    // 오늘 일정 출력
    for (int i = 0; i < size; ++i) {
        printf("[%s ~ %s\t%s]\n", schedules[i].startTime, schedules[i].endTime, schedules[i].activity);
    } 

}

// 오늘 스케줄을 만들고 보여줌.
void makeToday() {
    struct tm* date;
    const time_t t = time(NULL);
    date = localtime(&t); // 현재 시각을 date에 저장.

    FILE* fixed = fopen(fileTable[0], "r"); // 고정 스케줄 파일 포인터
    FILE* tmpSchedule = fopen(fileTable[1], "r"); // 일시 스케줄 파일 포인터
    FILE* toDo = fopen(fileTable[2], "r"); // 해야할 일 파일 포인터

    TimeSchedule schedules[50]; // 활동시간대가 포함된 스케줄들을 담을 구조체 배열
    ToDo todoList[50]; // 해야할 일 스케줄들을 담을 구조체 배열

    int tsIdx = 0; // schedules index
    int todoIdx = 0; // todoList index
    int isDuplicated = 0; // 일정이 겹치는지 표시하는 정수

    tsIdx = insertTimeSchedule(date, schedules, fixed, tsIdx); // 고정 스케줄을 TimeSchedule 구조체 배열에 추가
    tsIdx = insertTimeSchedule(date, schedules, tmpSchedule, tsIdx); // 일시 스케줄을 TimeSchedule 구조체 배열에 추가
    todoIdx = insertToDo(date, todoList, toDo); // 해야할 일을 ToDo 구조체 배열에 추가

    // 파일 닫기
    fclose(fixed);
    fclose(tmpSchedule);
    fclose(toDo);

    qsort((TimeSchedule*)schedules, tsIdx, sizeof(TimeSchedule), cmpfunc); // TimeSchedule 배열을 startTime이 빠른 순서대로 정렬.

    isDuplicated = checkDuplicated(schedules, tsIdx);
    if (isDuplicated) return; // 중복된 일정이 있으면 함수에서 나오기.

    tsIdx = appendToDo(schedules, todoList, tsIdx, todoIdx); // TimeSchedule 배열에 todoList 끼워넣기

    displayToday(date, schedules, tsIdx); // TimeSchedule 배열에서 하나씩 꺼내 출력
}
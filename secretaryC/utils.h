void clearBuffer();
void getTime(int timeArr[2]);
void getScheduleInput(int scheduleKind, int answer[3][30], int i);
int findSchedule(char wantToChange[3][30], char* fileName, char date[30], int scheduleNum);
char askWhereChange(char* fileName, int scheduleKind, char wantToChange[3][30], char* kind);
void updateFile(char* fileName, int scheduleNum, char change[3][30], char* kind);
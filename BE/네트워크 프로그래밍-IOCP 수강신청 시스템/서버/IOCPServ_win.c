#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <WinSock2.h>
#include <windows.h>
#include "message_passing.h"
#include <time.h>

////////function area///////////
void App_init();
void set_limit_time();
DWORD WINAPI SugangThreadMain(LPVOID);
int CheckID(char*);
int CheckClass(char*);
void ErrorHandling(char*);
void refresh_student();
void refresh_subject();
DWORD WINAPI check_timeout();
int check_timetable(class_info, class_info);

////////global variables//////////
student_info* student;
class_info* subject;
int limit_time = 0;
int timeover = FALSE;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;
	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	int recvBytes, flags = 0;

	App_init();
	set_limit_time();
	_beginthreadex(NULL, 0, check_timeout, NULL, 0, NULL);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);

	for (unsigned int i = 0; i < sysInfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, SugangThreadMain, (LPVOID)hComPort, 0, NULL);

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(atoi("121415"));
	bind(hServSock, (SOCKADDR*)& servAdr, sizeof(servAdr));
	listen(hServSock, 5);

	printf("경북대학교 수강신청 서버\n");
	while (1)
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		hClntSock = accept(hServSock, (SOCKADDR*)& clntAdr, &addrLen);

		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (DWORD)handleInfo, 0);

		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE * 500;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, &recvBytes, &flags, &(ioInfo->overlapped), NULL);
	}
	return 0;
}

void App_init()
{
	FILE* fp;
	student_info stdtmp;
	class_info clstmp;
	int index = 0;

	fp = fopen("student.bin", "rb");
	if (fp == NULL) ErrorHandling("No Student File.");

	student = (student_info *)malloc(sizeof(student_info) * student_size);

	for (int i = 0; i < student_size; i++)
	{
		if (fread(&stdtmp, sizeof(stdtmp), 1, fp) < 1) break;
		student[i] = stdtmp;
	}
	fclose(fp);

	index = 0;
	fp = fopen("subject.bin", "rb");
	if (fp == NULL) ErrorHandling("No Subject File.");

	subject = (class_info *)malloc(sizeof(class_info) * subject_size);

	for (int i = 0; i < subject_size; i++)
	{
		if (fread(&clstmp, sizeof(clstmp), 1, fp) < 1) break;
		subject[i] = clstmp;
	}
	fclose(fp);

}

void set_limit_time() //서버 시간 설정 (몇시까지 수강 신청을 받을 것인가)
{
	int limit_day = 0;
	int limit_hour = 0;
	int limit_min = 0;
	char yorn = 0;

	while (1)
	{
		printf("수강 신청 종료 시간을 설정해 주세요. \n");
		printf("일 : ");
		scanf("%d", &limit_day);
		if (limit_day < 0 || limit_day >31)
		{	
			printf("잘못된 입력입니다. 다시 입력해 주세요. \n");
			Sleep(500);
			continue;
		}
		printf("시 : ");
		scanf("%d", &limit_hour);
		if (limit_hour < 0 || limit_hour >23)
		{
			printf("잘못된 입력입니다. 다시 입력해 주세요. \n");
			Sleep(500);
			continue;
		}
		printf("분 : ");
		scanf("%d", &limit_min);
		if (limit_min < 0 || limit_min >59)
		{
			printf("잘못된 입력입니다. 다시 입력해 주세요. \n");
			Sleep(500);
			continue;
		}

		printf("설정하신 시간이 %d일 %d시 %d분. 다시 설정하시겠습니까? [y/n] : ",limit_day, limit_hour, limit_min);
		//fgets(&yorn,1,stdin);
		getchar();
		scanf("%c", &yorn);
		if (yorn == 'n')
			break;
	}
	limit_time = limit_day * 10000 + limit_hour * 100 + limit_min;
	printf("설정되었습니다.\n");
	Sleep(500);
	system("cls");
}

DWORD WINAPI SugangThreadMain(LPVOID pComPort)
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	MessagefromServer server_message;
	MessagefromClient *client_message;
	DWORD flags = 0;
	//   FILE *student_file, *subject_file;
	WSAEVENT evobj;

	evobj = WSACreateEvent();
	while (1)
	{
		time_t timer;
		struct tm *t;
		int hour = 0, min = 0, sec = 0;
		int index;

		GetQueuedCompletionStatus(hComPort, &bytesTrans, (LPDWORD)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE); // 성공하면 0 실패하면 0 아닌값 리턴. 여기 예외처리해서 0 아닌거 받으면 클라이언트에게 에러메세지 전송해야함
		sock = handleInfo->hClntSock;
		ioInfo->overlapped.hEvent = evobj;

		if (ioInfo->rwMode == READ)                              //Recv인 경우
		{
			if (bytesTrans == 0)                              // EOF 전송 시
			{
				closesocket(sock);
				free(handleInfo); free(ioInfo);
				continue;
			}
			timer = time(NULL);
			t = localtime(&timer);

			client_message = (MessagefromClient *)ioInfo->buffer;
			//printf("Recv %d %s %d\n", client_message->msgtype, client_message->stringtemp, client_message->integer);   //메시지 확인용

			if (client_message->msgtype == 0)                              // 0. 연결종료
			{
				printf("%04d.%02d.%02d %02d:%02d:%02d  ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
				printf("%s has disconnected.\n", client_message->stringtemp);
				closesocket(sock);
				free(handleInfo); free(ioInfo);
				continue;
			}

			else if (client_message->msgtype == 1)                              //1.학생정보 요청 시
			{
				if ((index = CheckID(client_message->stringtemp)) != -1)         //올바른 학번인지 확인
				{
					server_message.Valid = TRUE;
					server_message.sinfo = student[index];
					printf("%04d.%02d.%02d %02d:%02d:%02d  ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
					printf("%s has connected.\n", client_message->stringtemp);
				}
				else server_message.Valid = FALSE;                           //올바른 학번이 아닐 경우 FALSE
			}

			if (client_message->msgtype == 2)                           //2. 과목정보 요청 시
			{
				index = client_message->integer;
				if (index >= 0 && index < subject_size)
				{
					server_message.Valid = TRUE;
					server_message.cinfo = subject[index];
					server_message.integer = subject_size;
				}
				else
					server_message.Valid = FALSE;
			}

			else if (client_message->msgtype == 3)                           //3. 수강신청
			{
				int target_student;   //검색된 학생 인덱스
				int target_subject;   //검색된 과목 인덱스
				int len_points = 0, err_flag = 0;
				int i;
				char overlap_test[11];
				FILE * class_reserv;
				char file_name[10];
				if ((target_student = CheckID(client_message->stringtemp)) == -1)   //에러 처리 필요
				{
					ErrorHandling("Invalid std_id input.");
				}

				///////////////////////////////오류 판별/////////////////////////////////
				err_flag = 0;
				memset(overlap_test, 0, sizeof(overlap_test));
				strncpy(overlap_test, (char*)client_message->assist, 10);

				if (timeover) 
				{
					err_flag = 5;
					server_message.Valid = FALSE;
					server_message.sinfo = student[target_student];
					server_message.integer = err_flag;
					strcpy(server_message.dest, "신청 기간이 아닙니다.");
				}

				else
				{
					//시간표 겹침 찾기
					//assist = overlap_test 안의 문자열에 해당하는 과목코드로 과목을 찾는다.
					class_info class_temp;
					memset(&class_temp, 0, sizeof(class_temp));
					for (int i = 0; i < subject_size; i++) {
						if (strcmp(subject[i].classID, overlap_test) == 0) {
							class_temp = subject[i];
							break;
						}
					}

					for (i = 0; i < student[target_student].requested_num; i++)
					{
						if (strcmp(student[target_student].requested[i].classID, overlap_test) == 0)
						{
							err_flag = 1;
							break;
						}
						//student가 가진 과목들마다 시간표가 겹치는지 확인한다.
						if (check_timetable(student[target_student].requested[i], class_temp) == 1) {
							err_flag = 6;
							server_message.Valid = FALSE;
							server_message.sinfo = student[target_student];
							server_message.integer = err_flag;
							strcpy(server_message.dest, "시간표에 겹치는 과목이 있습니다.");
						}
					
					}

					if (err_flag == 1) // 이미 신청한 과목을 또다시 신청할 경우
					{
						server_message.Valid = FALSE;
						server_message.sinfo = student[target_student];
						server_message.integer = err_flag;
						strcpy(server_message.dest, "이미 신청한 과목입니다.");
					}

					if (student[target_student].points < client_message->integer) //잔여 포인트를 넘어서 신청할 경우
					{
						err_flag = 2;
						server_message.Valid = FALSE;
						server_message.sinfo = student[target_student];
						server_message.integer = err_flag;
						strcpy(server_message.dest, "포인트가 부족합니다.");
					}

					if (student[target_student].cur_gradp < student[target_student].requested_num) //이수학점인 24점을 초과하여 신청할 경우
					{
						char tmp[50];

						err_flag = 3;
						server_message.Valid = FALSE;
						server_message.sinfo = student[target_student];
						server_message.integer = err_flag;
						sprintf(tmp, "이수학점 %d점을 초과하였습니다.", student[i].cur_gradp);
						strcpy(server_message.dest, tmp);
					}

					if (CheckClass(client_message->assist) == -1) //신청하려는 과목 번호가 올바르지 않을 경우의 에러처리
					{
						err_flag = 4;
						server_message.Valid = FALSE;
						server_message.sinfo = student[target_student];
						server_message.integer = err_flag;
						strcpy(server_message.dest, "존재하지 않는 과목입니다.");
					}

					if (err_flag == 0)
					{
						//올바른 입력일 경우
						//subject 배열에서 인풋으로 들어온 과목의 인덱스 찾음
						if ((target_subject = CheckClass(client_message->assist)) == -1)   //에러 처리 필요
						{
							ErrorHandling("Invalid cls_id input.");
						}
						//해당 학생의 포인트 입력된만큼 차감, 신청 과목에 투자한 포인트 명시
						student[target_student].points -= client_message->integer;
						student[target_student].used_points[student[target_student].requested_num]
							= client_message->integer;
						//해당 학생의 신청 현황에 신청한 과목 추가
						student[target_student].requested[student[target_student].requested_num] =
							subject[target_subject];
						student[target_student].requested_num++;
						//해당 학생의 이수학점 증가
						student[target_student].cur_gradp += subject[target_subject].grad;
						//학생정보 갱신(파일에 저장)
						refresh_student();

						//해당 과목의 현재 수강인원 증가
						subject[target_subject].cur_req++;
						//과목정보 갱신
						refresh_subject();

						//갱신된 학생 정보 client에게 전송.
						server_message.Valid = TRUE;
						server_message.sinfo = student[target_student];
						server_message.integer = err_flag;
						
						strncpy(file_name,client_message->assist,11); //파일의 끝부분에 추가된 내용 삽입
						strcat(file_name, ".txt");
						class_reserv = fopen(file_name,"a");
//						printf("%s",file_name);

						fprintf(class_reserv,"%d %d\n",target_student,client_message->integer);
						fclose(class_reserv);

						printf("%04d.%02d.%02d %02d:%02d:%02d  ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
						printf("%s requested %s, %d point(s).\n", client_message->stringtemp, client_message->assist, client_message->integer);
					}
				}
			}

			else if (client_message->msgtype == 4)                           //4. 수강취소
			{
				int target_student;   //검색된 학생 인덱스
				int target_subject;   //검색된 과목 인덱스
				int len_points = 0;
				int delete_subject_idx = 0;
				int delete_file = 0;
				int i, j;
				int index = 0;
				char temp_file_name[20];
				int reserv_file[student_size][2];
				FILE *delete_before;
				FILE *delete_after;
				if ((target_student = CheckID(client_message->stringtemp)) == -1)   //에러 처리 필요
				{
					ErrorHandling("Invalid std_id input.");
				}

				if (timeover)		//신청 기간이 지난 경우의 에러처리
				{
					server_message.Valid = FALSE;
					server_message.sinfo = student[target_student];
					server_message.integer = 5;
					strcpy(server_message.dest, "신청 기간이 아닙니다.");
				}

				else if ((target_subject = CheckClass(client_message->assist)) == -1)   //삭제할 과목 입력이 올바르지 않을 경우
				{
					//                  ErrorHandling("Invalid cls_id input.");
					server_message.Valid = FALSE;
					server_message.sinfo = student[target_student];
					server_message.integer = -1;
					strcpy(server_message.dest, "존재하지 않는 과목입니다.");
				}
				else
				{
					//올바른 입력
					memset(reserv_file, sizeof(reserv_file), 0);
					strncpy(temp_file_name, client_message->assist,10);
					temp_file_name[10] = 0;
					strcat(temp_file_name,".txt");
					printf("%s",temp_file_name);
					if ((delete_before = fopen(temp_file_name, "r")) != NULL)	//파일의 저장된 내용을 수정하기 위해 불러온다.
					{
						while (1)
						{
							fscanf(delete_before, "%d %d\n", &reserv_file[index][0], &reserv_file[index++][1]);
							if (feof(delete_before))
								break;
						}
						fclose(delete_before);
					}
					
					//올바른 입력일 경우
					//subject 배열에서 인풋으로 들어온 과목의 인덱스 찾음
					for (i = 0; i < student[target_student].requested_num; i++)
					{
						//                     printf("%s %s", student[target_student].requested[i].classID, client_message->assist);
						if (strcmp(student[target_student].requested[i].classID, client_message->assist) == 0)
						{
							delete_subject_idx = i;
							break;
						}
					}

								  //해당 학생의 포인트 입력했던만큼 복원, 신청 과목에 투자한 포인트 명시
					student[target_student].points += student[target_student].used_points[delete_subject_idx];
					student[target_student].requested_num = student[target_student].requested_num - 1;
					
					for (i = delete_subject_idx; i < student[target_student].requested_num; i++)
						student[target_student].used_points[i] = student[target_student].used_points[i + 1];
					student[target_student].used_points[i] = 0;		//shift해줌
				
					 //해당 학생의 신청 현황에 신청한 과목 추가
					for (j = delete_subject_idx; j < student[target_student].requested_num; j++)
						student[target_student].requested[j] = student[target_student].requested[j + 1];
					memset(&student[target_student].requested[j], 0, sizeof(student[target_student].requested[j]));

					//해당 학생의 이수학점 감소
					student[target_student].cur_gradp -= subject[target_subject].grad;
					//학생정보 갱신(파일에 저장)
					refresh_student();

					//해당 과목의 현재 수강인원 감소
					subject[target_subject].cur_req--;
					//과목정보 갱신
					refresh_subject();

					//갱신된 학생 정보 client에게 전송.
					server_message.Valid = TRUE;
					server_message.sinfo = student[target_student];
					server_message.integer = 0;

					for (int k = 0; k < index; k++)		//파일에서 받아온 배열에서 해당 학생의 index를 찾아 지운다.
					{
						if (reserv_file[k][0] == target_student)
						{
							delete_file = k;
						break;
						}
					}

					for (int k = delete_file; k < index - 1; k++)	//shift
					{
						reserv_file[k][0] = reserv_file[k + 1][0];
					}
					if ((delete_after = fopen(temp_file_name, "w")) != NULL) //파일을 열어 수정한 내용을 저장한다.
					{
						for (int k = 0; k < index - 1; k++)
						{
							fprintf(delete_after, "%d %d\n", reserv_file[k][0], reserv_file[k][1]);
						}
						fclose(delete_after);
					}
					
					printf("%04d.%02d.%02d %02d:%02d:%02d  ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
					printf("%s canceled %s.\n", client_message->stringtemp, client_message->assist);
				}
			}
			//server_message구조체에 클라이언트에게 전달할 메시지를 넣고 ioinfo의 wsabuf에 넣어 보낸다.
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = sizeof(server_message);
			ioInfo->wsaBuf.buf = (char *)&server_message;
			ioInfo->rwMode = WRITE;
			//printf("Send %s\n", server_message.sinfo.stdID);   //메시지 확인용

			if (WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL))
			{
				if (WSAGetLastError() == WSA_IO_PENDING)
				{
					puts("Background data receive");
					WSAWaitForMultipleEvents(1, &evobj, TRUE, WSA_INFINITE, FALSE);
					WSAGetOverlappedResult(sock, &ioInfo->overlapped, NULL, FALSE, NULL);
				}
				else
					ErrorHandling("WSASend() error");
			}

			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE * 500;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
		}

		else
		{
			free(ioInfo);
		}
	}
	return 0;
}

int CheckID(char *id)
{
	for (int i = 0; i < student_size; i++)
	{
		if (strcmp(id, student[i].stdID) == 0)
			return i;
	}
	return -1;
}

int CheckClass(char *id)
{
	for (int i = 0; i < subject_size; i++)
	{
		if (strcmp(id, subject[i].classID) == 0)
			return i;
	}
	return -1;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	//   exit(1);
}

void refresh_student()
{
	FILE* fp;

	fp = fopen("student.bin", "wb");
	if (fp == NULL) ErrorHandling("Student Refresh Error");

	for (int i = 0; i <= student_size; i++)
		fwrite(&student[i], sizeof(student_info), 1, fp);

	fclose(fp);
}

void refresh_subject()
{
	FILE* fp;

	fp = fopen("subject.bin", "wb");
	if (fp == NULL) ErrorHandling("Subject Refresh Error");

	for (int i = 0; i <= subject_size; i++)
		fwrite(&subject[i], sizeof(class_info), 1, fp);

	fclose(fp);
}

DWORD WINAPI check_timeout()
{
	time_t timer;
	struct tm *t;
	int timenow;
	FILE *f;
	char temp_filename[20];
	int temp_reserved[student_size][2];
	int index = 0;
	int temp1;
	int temp2;
	int repeat;
	while (1)
	{
		timer = time(NULL);
		t = localtime(&timer);

		timenow = t->tm_mday * 10000 + t->tm_hour * 100 + t->tm_min;

		if (timenow >= limit_time)
		{
			timeover = TRUE;
			printf("Time Done!\n");
			for (repeat = 0; repeat < subject_size; repeat++)
			{
				memset(temp_reserved, 0, sizeof(temp_reserved));
				index = 0;
				strcpy(temp_filename, subject[repeat].classID);
				strcat(temp_filename, ".txt");
				if ((f = fopen(temp_filename, "r")) != NULL)
				{
					while (1)
					{
						fscanf(f, "%d %d\n", &temp_reserved[index][0], &temp_reserved[index][1]);
						index++;
						if (feof(f))
							break;

					}
					fclose(f);
				}
				for (int k = 0; k < index; k++)
				{
					for (int j = 0; j < index - 1 - k; j++)
					{
						if (temp_reserved[j][1] < temp_reserved[j + 1][1])
						{
							temp1 = temp_reserved[j][0];
							temp2 = temp_reserved[j][1];
							temp_reserved[j][0] = temp_reserved[j + 1][0];
							temp_reserved[j][1] = temp_reserved[j + 1][1];
							temp_reserved[j + 1][0] = temp1;
							temp_reserved[j + 1][1] = temp2;
						}
					}
				}
				//각 과목마다 정렬된 student 인덱스들 중에서 그 과목의 accept만큼 인원 선발
				
				int limit = subject[repeat].max_acpt;
				//최대인원보다 신청인원이 적은 경우 전원 통과
				if (subject[repeat].cur_req < subject[repeat].max_acpt) continue;
				//그렇지 않은 경우, 정렬된 결과에서 limit을 넘는 인덱스를 가진 학생 구조체의
				//requested 배열에서 해당 과목을 제거한다.
				for (int j = limit; j < subject[repeat].cur_req; j++) {
					int student_index = temp_reserved[j][0];
					//학생의 신청 과목 배열에서 해당 과목을 찾는다.
					for (int k = 0; k < student[student_index].requested_num; k++) {
						//과목 ID비교해 같다면 제거
						if (strcmp(student[student_index].requested[k].classID, subject[repeat].classID) == 0) {
							//해당 과목을 제거 (인덱스 당겨오기)
							for (int m = k; m < student[student_index].requested_num - 1; m++)
							{
								student[student_index].requested[m] = student[student_index].requested[m + 1];
								student[student_index].used_points[m] = student[student_index].used_points[m + 1];
							}
							//신청 학점 다시 감소 / 신청 과목 갯수 감소 (이제 신청 완료 과목 수로 취급됨)
							student[student_index].cur_gradp -= subject[repeat].grad;
							student[student_index].requested_num -= 1;
							break;
						}
					}
				}
			}
			refresh_student();
			refresh_subject();
			
			break;
		}
		Sleep(1000);
	}
	return 0;
}

int check_timetable(class_info c1, class_info c2) {
	//c1의 time1에 대해 먼저 검사
	//같은 날이어야 함.
	if (c1.time1.day == c2.time1.day) {
		//c1의 시작~ 끝 사이에 c2의 시작 또는 끝이 있으면 겹치는 것임.
		if ((c1.time1.start < c2.time1.start && c1.time1.end > c2.time1.start) || (c1.time1.start < c2.time1.end && c1.time1.end > c2.time1.end)) return 1;	//겹침을 리턴.
	}
	//c2.time2의 어떤 멤버가 유효한 값일 때(즉, c2의 time2가 존재할 때.)
	if (c2.time2.day > 0) {
		if (c1.time1.day == c2.time2.day) {
			if ((c1.time1.start < c2.time2.start && c1.time1.end > c2.time2.start) || (c1.time1.start < c2.time2.end && c1.time1.end > c2.time2.end)) return 1;	//겹침을 리턴.
		}
	}
	//c1의 time2가 유효하다면
	if (c1.time2.day > 0) {
		//c1의 time1, 2에 대해서 검사
		if (c1.time2.day == c2.time1.day) {
			//c1의 시작~ 끝 사이에 c2의 시작 또는 끝이 있으면 겹치는 것임.
			if ((c1.time2.start < c2.time1.start && c1.time2.end > c2.time1.start) || (c1.time2.start < c2.time1.end && c1.time2.end > c2.time1.end)) return 1;	//겹침을 리턴.
		}
		//c2.time2의 어떤 멤버가 유효한 값일 때(즉, c2의 time2가 존재할 때.)
		if (c2.time2.day > 0) {
			if (c1.time2.day == c2.time2.day) {
				if ((c1.time2.start < c2.time2.start && c1.time2.end > c2.time2.start) || (c1.time2.start < c2.time2.end && c1.time2.end > c2.time2.end)) return 1;	//겹침을 리턴.
			}
		}
	}
	return 0;
}
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

	printf("��ϴ��б� ������û ����\n");
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

void set_limit_time() //���� �ð� ���� (��ñ��� ���� ��û�� ���� ���ΰ�)
{
	int limit_day = 0;
	int limit_hour = 0;
	int limit_min = 0;
	char yorn = 0;

	while (1)
	{
		printf("���� ��û ���� �ð��� ������ �ּ���. \n");
		printf("�� : ");
		scanf("%d", &limit_day);
		if (limit_day < 0 || limit_day >31)
		{	
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է��� �ּ���. \n");
			Sleep(500);
			continue;
		}
		printf("�� : ");
		scanf("%d", &limit_hour);
		if (limit_hour < 0 || limit_hour >23)
		{
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է��� �ּ���. \n");
			Sleep(500);
			continue;
		}
		printf("�� : ");
		scanf("%d", &limit_min);
		if (limit_min < 0 || limit_min >59)
		{
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է��� �ּ���. \n");
			Sleep(500);
			continue;
		}

		printf("�����Ͻ� �ð��� %d�� %d�� %d��. �ٽ� �����Ͻðڽ��ϱ�? [y/n] : ",limit_day, limit_hour, limit_min);
		//fgets(&yorn,1,stdin);
		getchar();
		scanf("%c", &yorn);
		if (yorn == 'n')
			break;
	}
	limit_time = limit_day * 10000 + limit_hour * 100 + limit_min;
	printf("�����Ǿ����ϴ�.\n");
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

		GetQueuedCompletionStatus(hComPort, &bytesTrans, (LPDWORD)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE); // �����ϸ� 0 �����ϸ� 0 �ƴѰ� ����. ���� ����ó���ؼ� 0 �ƴѰ� ������ Ŭ���̾�Ʈ���� �����޼��� �����ؾ���
		sock = handleInfo->hClntSock;
		ioInfo->overlapped.hEvent = evobj;

		if (ioInfo->rwMode == READ)                              //Recv�� ���
		{
			if (bytesTrans == 0)                              // EOF ���� ��
			{
				closesocket(sock);
				free(handleInfo); free(ioInfo);
				continue;
			}
			timer = time(NULL);
			t = localtime(&timer);

			client_message = (MessagefromClient *)ioInfo->buffer;
			//printf("Recv %d %s %d\n", client_message->msgtype, client_message->stringtemp, client_message->integer);   //�޽��� Ȯ�ο�

			if (client_message->msgtype == 0)                              // 0. ��������
			{
				printf("%04d.%02d.%02d %02d:%02d:%02d  ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
				printf("%s has disconnected.\n", client_message->stringtemp);
				closesocket(sock);
				free(handleInfo); free(ioInfo);
				continue;
			}

			else if (client_message->msgtype == 1)                              //1.�л����� ��û ��
			{
				if ((index = CheckID(client_message->stringtemp)) != -1)         //�ùٸ� �й����� Ȯ��
				{
					server_message.Valid = TRUE;
					server_message.sinfo = student[index];
					printf("%04d.%02d.%02d %02d:%02d:%02d  ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
					printf("%s has connected.\n", client_message->stringtemp);
				}
				else server_message.Valid = FALSE;                           //�ùٸ� �й��� �ƴ� ��� FALSE
			}

			if (client_message->msgtype == 2)                           //2. �������� ��û ��
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

			else if (client_message->msgtype == 3)                           //3. ������û
			{
				int target_student;   //�˻��� �л� �ε���
				int target_subject;   //�˻��� ���� �ε���
				int len_points = 0, err_flag = 0;
				int i;
				char overlap_test[11];
				FILE * class_reserv;
				char file_name[10];
				if ((target_student = CheckID(client_message->stringtemp)) == -1)   //���� ó�� �ʿ�
				{
					ErrorHandling("Invalid std_id input.");
				}

				///////////////////////////////���� �Ǻ�/////////////////////////////////
				err_flag = 0;
				memset(overlap_test, 0, sizeof(overlap_test));
				strncpy(overlap_test, (char*)client_message->assist, 10);

				if (timeover) 
				{
					err_flag = 5;
					server_message.Valid = FALSE;
					server_message.sinfo = student[target_student];
					server_message.integer = err_flag;
					strcpy(server_message.dest, "��û �Ⱓ�� �ƴմϴ�.");
				}

				else
				{
					//�ð�ǥ ��ħ ã��
					//assist = overlap_test ���� ���ڿ��� �ش��ϴ� �����ڵ�� ������ ã�´�.
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
						//student�� ���� ����鸶�� �ð�ǥ�� ��ġ���� Ȯ���Ѵ�.
						if (check_timetable(student[target_student].requested[i], class_temp) == 1) {
							err_flag = 6;
							server_message.Valid = FALSE;
							server_message.sinfo = student[target_student];
							server_message.integer = err_flag;
							strcpy(server_message.dest, "�ð�ǥ�� ��ġ�� ������ �ֽ��ϴ�.");
						}
					
					}

					if (err_flag == 1) // �̹� ��û�� ������ �Ǵٽ� ��û�� ���
					{
						server_message.Valid = FALSE;
						server_message.sinfo = student[target_student];
						server_message.integer = err_flag;
						strcpy(server_message.dest, "�̹� ��û�� �����Դϴ�.");
					}

					if (student[target_student].points < client_message->integer) //�ܿ� ����Ʈ�� �Ѿ ��û�� ���
					{
						err_flag = 2;
						server_message.Valid = FALSE;
						server_message.sinfo = student[target_student];
						server_message.integer = err_flag;
						strcpy(server_message.dest, "����Ʈ�� �����մϴ�.");
					}

					if (student[target_student].cur_gradp < student[target_student].requested_num) //�̼������� 24���� �ʰ��Ͽ� ��û�� ���
					{
						char tmp[50];

						err_flag = 3;
						server_message.Valid = FALSE;
						server_message.sinfo = student[target_student];
						server_message.integer = err_flag;
						sprintf(tmp, "�̼����� %d���� �ʰ��Ͽ����ϴ�.", student[i].cur_gradp);
						strcpy(server_message.dest, tmp);
					}

					if (CheckClass(client_message->assist) == -1) //��û�Ϸ��� ���� ��ȣ�� �ùٸ��� ���� ����� ����ó��
					{
						err_flag = 4;
						server_message.Valid = FALSE;
						server_message.sinfo = student[target_student];
						server_message.integer = err_flag;
						strcpy(server_message.dest, "�������� �ʴ� �����Դϴ�.");
					}

					if (err_flag == 0)
					{
						//�ùٸ� �Է��� ���
						//subject �迭���� ��ǲ���� ���� ������ �ε��� ã��
						if ((target_subject = CheckClass(client_message->assist)) == -1)   //���� ó�� �ʿ�
						{
							ErrorHandling("Invalid cls_id input.");
						}
						//�ش� �л��� ����Ʈ �Էµȸ�ŭ ����, ��û ���� ������ ����Ʈ ���
						student[target_student].points -= client_message->integer;
						student[target_student].used_points[student[target_student].requested_num]
							= client_message->integer;
						//�ش� �л��� ��û ��Ȳ�� ��û�� ���� �߰�
						student[target_student].requested[student[target_student].requested_num] =
							subject[target_subject];
						student[target_student].requested_num++;
						//�ش� �л��� �̼����� ����
						student[target_student].cur_gradp += subject[target_subject].grad;
						//�л����� ����(���Ͽ� ����)
						refresh_student();

						//�ش� ������ ���� �����ο� ����
						subject[target_subject].cur_req++;
						//�������� ����
						refresh_subject();

						//���ŵ� �л� ���� client���� ����.
						server_message.Valid = TRUE;
						server_message.sinfo = student[target_student];
						server_message.integer = err_flag;
						
						strncpy(file_name,client_message->assist,11); //������ ���κп� �߰��� ���� ����
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

			else if (client_message->msgtype == 4)                           //4. �������
			{
				int target_student;   //�˻��� �л� �ε���
				int target_subject;   //�˻��� ���� �ε���
				int len_points = 0;
				int delete_subject_idx = 0;
				int delete_file = 0;
				int i, j;
				int index = 0;
				char temp_file_name[20];
				int reserv_file[student_size][2];
				FILE *delete_before;
				FILE *delete_after;
				if ((target_student = CheckID(client_message->stringtemp)) == -1)   //���� ó�� �ʿ�
				{
					ErrorHandling("Invalid std_id input.");
				}

				if (timeover)		//��û �Ⱓ�� ���� ����� ����ó��
				{
					server_message.Valid = FALSE;
					server_message.sinfo = student[target_student];
					server_message.integer = 5;
					strcpy(server_message.dest, "��û �Ⱓ�� �ƴմϴ�.");
				}

				else if ((target_subject = CheckClass(client_message->assist)) == -1)   //������ ���� �Է��� �ùٸ��� ���� ���
				{
					//                  ErrorHandling("Invalid cls_id input.");
					server_message.Valid = FALSE;
					server_message.sinfo = student[target_student];
					server_message.integer = -1;
					strcpy(server_message.dest, "�������� �ʴ� �����Դϴ�.");
				}
				else
				{
					//�ùٸ� �Է�
					memset(reserv_file, sizeof(reserv_file), 0);
					strncpy(temp_file_name, client_message->assist,10);
					temp_file_name[10] = 0;
					strcat(temp_file_name,".txt");
					printf("%s",temp_file_name);
					if ((delete_before = fopen(temp_file_name, "r")) != NULL)	//������ ����� ������ �����ϱ� ���� �ҷ��´�.
					{
						while (1)
						{
							fscanf(delete_before, "%d %d\n", &reserv_file[index][0], &reserv_file[index++][1]);
							if (feof(delete_before))
								break;
						}
						fclose(delete_before);
					}
					
					//�ùٸ� �Է��� ���
					//subject �迭���� ��ǲ���� ���� ������ �ε��� ã��
					for (i = 0; i < student[target_student].requested_num; i++)
					{
						//                     printf("%s %s", student[target_student].requested[i].classID, client_message->assist);
						if (strcmp(student[target_student].requested[i].classID, client_message->assist) == 0)
						{
							delete_subject_idx = i;
							break;
						}
					}

								  //�ش� �л��� ����Ʈ �Է��ߴ���ŭ ����, ��û ���� ������ ����Ʈ ���
					student[target_student].points += student[target_student].used_points[delete_subject_idx];
					student[target_student].requested_num = student[target_student].requested_num - 1;
					
					for (i = delete_subject_idx; i < student[target_student].requested_num; i++)
						student[target_student].used_points[i] = student[target_student].used_points[i + 1];
					student[target_student].used_points[i] = 0;		//shift����
				
					 //�ش� �л��� ��û ��Ȳ�� ��û�� ���� �߰�
					for (j = delete_subject_idx; j < student[target_student].requested_num; j++)
						student[target_student].requested[j] = student[target_student].requested[j + 1];
					memset(&student[target_student].requested[j], 0, sizeof(student[target_student].requested[j]));

					//�ش� �л��� �̼����� ����
					student[target_student].cur_gradp -= subject[target_subject].grad;
					//�л����� ����(���Ͽ� ����)
					refresh_student();

					//�ش� ������ ���� �����ο� ����
					subject[target_subject].cur_req--;
					//�������� ����
					refresh_subject();

					//���ŵ� �л� ���� client���� ����.
					server_message.Valid = TRUE;
					server_message.sinfo = student[target_student];
					server_message.integer = 0;

					for (int k = 0; k < index; k++)		//���Ͽ��� �޾ƿ� �迭���� �ش� �л��� index�� ã�� �����.
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
					if ((delete_after = fopen(temp_file_name, "w")) != NULL) //������ ���� ������ ������ �����Ѵ�.
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
			//server_message����ü�� Ŭ���̾�Ʈ���� ������ �޽����� �ְ� ioinfo�� wsabuf�� �־� ������.
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = sizeof(server_message);
			ioInfo->wsaBuf.buf = (char *)&server_message;
			ioInfo->rwMode = WRITE;
			//printf("Send %s\n", server_message.sinfo.stdID);   //�޽��� Ȯ�ο�

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
				//�� ���񸶴� ���ĵ� student �ε����� �߿��� �� ������ accept��ŭ �ο� ����
				
				int limit = subject[repeat].max_acpt;
				//�ִ��ο����� ��û�ο��� ���� ��� ���� ���
				if (subject[repeat].cur_req < subject[repeat].max_acpt) continue;
				//�׷��� ���� ���, ���ĵ� ������� limit�� �Ѵ� �ε����� ���� �л� ����ü��
				//requested �迭���� �ش� ������ �����Ѵ�.
				for (int j = limit; j < subject[repeat].cur_req; j++) {
					int student_index = temp_reserved[j][0];
					//�л��� ��û ���� �迭���� �ش� ������ ã�´�.
					for (int k = 0; k < student[student_index].requested_num; k++) {
						//���� ID���� ���ٸ� ����
						if (strcmp(student[student_index].requested[k].classID, subject[repeat].classID) == 0) {
							//�ش� ������ ���� (�ε��� ��ܿ���)
							for (int m = k; m < student[student_index].requested_num - 1; m++)
							{
								student[student_index].requested[m] = student[student_index].requested[m + 1];
								student[student_index].used_points[m] = student[student_index].used_points[m + 1];
							}
							//��û ���� �ٽ� ���� / ��û ���� ���� ���� (���� ��û �Ϸ� ���� ���� ��޵�)
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
	//c1�� time1�� ���� ���� �˻�
	//���� ���̾�� ��.
	if (c1.time1.day == c2.time1.day) {
		//c1�� ����~ �� ���̿� c2�� ���� �Ǵ� ���� ������ ��ġ�� ����.
		if ((c1.time1.start < c2.time1.start && c1.time1.end > c2.time1.start) || (c1.time1.start < c2.time1.end && c1.time1.end > c2.time1.end)) return 1;	//��ħ�� ����.
	}
	//c2.time2�� � ����� ��ȿ�� ���� ��(��, c2�� time2�� ������ ��.)
	if (c2.time2.day > 0) {
		if (c1.time1.day == c2.time2.day) {
			if ((c1.time1.start < c2.time2.start && c1.time1.end > c2.time2.start) || (c1.time1.start < c2.time2.end && c1.time1.end > c2.time2.end)) return 1;	//��ħ�� ����.
		}
	}
	//c1�� time2�� ��ȿ�ϴٸ�
	if (c1.time2.day > 0) {
		//c1�� time1, 2�� ���ؼ� �˻�
		if (c1.time2.day == c2.time1.day) {
			//c1�� ����~ �� ���̿� c2�� ���� �Ǵ� ���� ������ ��ġ�� ����.
			if ((c1.time2.start < c2.time1.start && c1.time2.end > c2.time1.start) || (c1.time2.start < c2.time1.end && c1.time2.end > c2.time1.end)) return 1;	//��ħ�� ����.
		}
		//c2.time2�� � ����� ��ȿ�� ���� ��(��, c2�� time2�� ������ ��.)
		if (c2.time2.day > 0) {
			if (c1.time2.day == c2.time2.day) {
				if ((c1.time2.start < c2.time2.start && c1.time2.end > c2.time2.start) || (c1.time2.start < c2.time2.end && c1.time2.end > c2.time2.end)) return 1;	//��ħ�� ����.
			}
		}
	}
	return 0;
}
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "Ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include "message_passing.h"

void ErrorHandling(char*);
MessagefromClient createMessage(int, char*, char*, int);
void Send(MessagefromClient);
void Recv(MessagefromServer);
int Action(int);
int UI(int);
void printClassinfo(class_info);
void printTimeinfo(classtime);

//Global variable area--------------------
WSADATA wsaData;
SOCKET hSocket;
SOCKADDR_IN recvAddr;
WSAEVENT event;
WSAOVERLAPPED overlapped;
WSABUF recvBuf, dataBuf;
student_info user;
class_info subject_list[LISTPERPAGE];
int sendBytes = 0, recvBytes = 0;
int flags = 0;
int IsHome = TRUE;
int page = 0, total;      //���� �а��ִ� ������, ��ü ������
//-----------------------------------

int main()
{
	//to global_wsadata

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED); //to global
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	recvAddr;   //to global
	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	recvAddr.sin_port = htons(atoi("121415"));

	if (connect(hSocket, (SOCKADDR*)& recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
		ErrorHandling("������ �߻��Ͽ����ϴ�. ����� �ٽ� �õ��� �ֽʽÿ�.\n");

	event = WSACreateEvent(); //to global

	//to global_overlapped
	memset(&overlapped, 0, sizeof(overlapped));

	overlapped.hEvent = event;

	//WSABUF dataBuf, recvBuf; <to global
	char message[1024] = { 0, };
	int nextState = 0;
	int cur_action = 0;

	printf("��ϴ��б� ������û Ŭ���̾�Ʈ ver.0.1\n");
	while (1)
	{
		nextState = UI(nextState);
		//cur_state = Action(cur_action);
		if (nextState == 9) break;
	}

	closesocket(hSocket);

	WSACleanup();

	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);

	exit(1);
}

MessagefromClient createMessage(int type, char* string, char* assist, int integer) {
	MessagefromClient mfc;
	mfc.msgtype = type;
	strcpy(mfc.stringtemp, string);
	strcpy(mfc.assist, assist);
	mfc.integer = integer;
	return mfc;
} //MessagefromClient ����ü�� �����Ѵ�.

void Send(MessagefromClient msg) {

	memset(&overlapped, 0, sizeof(overlapped));
	dataBuf.len = sizeof(msg);
	dataBuf.buf = (char*)& msg;

	if (WSASend(hSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			ErrorHandling("WSASend() error");
	}
	WSAWaitForMultipleEvents(1, &event, TRUE, 500, FALSE);
}

void Recv(MessagefromServer msg) {
	recvBuf.len = BUF_SIZE * 500;
	recvBuf.buf = (char*)&msg;

	if (WSARecv(hSocket, &recvBuf, 1, (LPDWORD)& recvBytes, (LPDWORD)& flags, &overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			ErrorHandling("WSARECV() error");
	}
}

int Action(int anum) {
	char temp[BUF_SIZE], temp2[BUF_SIZE];
	int select;
	int nextState = 0;
	MessagefromClient mfc;
	MessagefromServer mfs;
	MessagefromServer* mfs_p;

	memset(&mfs, 0, sizeof(mfs));
	switch (anum) {
	case 0:            //login action
		fgets(temp, sizeof(temp), stdin);   //�й��� �Է¹ް�
		temp[strlen(temp) - 1] = '\0';
		mfc = createMessage(1, temp, "\0", 0);   //�޼��� ����
		Send(mfc);                              //������ ����
		memset(&recvBuf, 0, sizeof(recvBuf));
		Recv(mfs);
		mfs_p = (MessagefromServer*)recvBuf.buf;
		if (mfs_p->Valid == TRUE)
		{
			strcpy(user.stdID, temp);
			user = mfs_p->sinfo;
			printf("�α��� ����. ��ø� ��ٷ��ּ���.\n");
			Sleep(700);
			nextState = 1;   //������ ������ UI�� ���� �޴��̴�.
		}
		else {
			printf("�α��� ����. �α��� ȭ������ ���ư��ϴ�.\n");
			Sleep(300);
		}
		//recv >> ����ü�� ����. (MessagefromServer)
		//������ �Ǵ� �Ѵ�(if������)
		//������ ������
		//�������� �α��� ���� ��ȣ�� ���� ���ߴٸ�, nextState�� 0���� ��� �� ��.
		break;

	case 1:   //�������� ���� ����Ʈ �޾ƿ�
		for (int i = 0; i < LISTPERPAGE; i++)         //�������� ���� ����ŭ ��û
		{
			mfc = createMessage(2, "\0", "\0", page + i);      //������ ���� ��û
			Send(mfc);
			memset(&recvBuf, 0, sizeof(recvBuf));
			Recv(mfs);

			mfs_p = (MessagefromServer*)recvBuf.buf;

			if (mfs_p->Valid)
			{
				subject_list[i] = mfs_p->cinfo;            //Ŭ���̾�Ʈ ����ҿ� �ӽ�����
				total = mfs_p->integer;
			}
		}

		nextState = 2;
		break;

	case 2:            //request action
		if (IsHome)
			system("cls");

		memset(temp, 0, sizeof(temp));
		printf("��û�� ���� ��ȣ�� �Է��ϼ��� : ");
		scanf("%s", temp);

		//���� list���� �´� ���� ã�Ƽ� ������
		printf("��û�Ͻ÷��� ����Ʈ�� �Է��ϼ���. ���� ����Ʈ [%d] (����Ϸ��� 0�� �Է��ϼ���) : ", user.points);
		scanf("%s", &temp2);
		select = atoi(temp2);

		if (select == 0 && strcmp(temp2, "0") == 0)
		{
			nextState = 1;   //���� ȭ������
			break;
		}
		//�Է� ����Ʈ bound�� ����� ��� ���� �߻�.

		mfc = createMessage(3, user.stdID, temp, select);      //������ ���� ��û
		Send(mfc);
		memset(&recvBuf, 0, sizeof(recvBuf));
		Recv(mfs);

		mfs_p = (MessagefromServer*)recvBuf.buf;
		user = mfs_p->sinfo;

		if (mfs_p->Valid == FALSE)
		{
			printf("������û�� �����Ͽ����ϴ�. : %s", mfs_p->dest);
			Sleep(1000);
		}
		else printf("%s ��û�Ǿ����ϴ�.\n", temp);
		Sleep(1000);

		mfs_p = (MessagefromServer*)recvBuf.buf;
		//      printf("from server : %s\n", mfs_p->dest);
		   //��û �޼����� ������ ����. �������� ���� �������� ��� �׿� �´� �޼����� ���.
		if (IsHome)   nextState = 1;   //���� ȭ������
		else nextState = 2;
		break;

	case 3:            //view requested class action
	   //student�� requested ������ ������ ������ ����Ʈ�� �����ش�.
	   //���ġ�� �����ش�. (���� ��, ���� ����Ʈ ��)
		system("cls");

		mfc = createMessage(3, user.stdID, "\0", 0);      //��û�� �׸� Ȯ��
		Send(mfc);
		memset(&recvBuf, 0, sizeof(recvBuf));
		Recv(mfs);
		mfs_p = (MessagefromServer*)recvBuf.buf;
		user = mfs_p->sinfo;
		printf("%s ���� ���� ��û�� �׸��Դϴ�. [�̼����� : %d / 24]\n",
			user.name, user.cur_gradp);

		printf("�ܿ� point : %d\n", mfs_p->sinfo.points);
		printf("\n������û ����\n");
		printf("[������ȣ]\t[�����]\t\t�������Ʈ\n");
		printf("===================================================\n");
		for (int i = 0; i < mfs_p->sinfo.requested_num; i++)
		{
			printf("%s\t", user.requested[i].classID);
			printf("%s\t", user.requested[i].name);
			printf("\t%d\n", user.used_points[i]);
		}
		printf("�����Ͻ÷��� 0��, ���� ȭ������ ���ư��÷��� �ƹ� ���ڳ� �Է��ϼ��� : ");
		scanf("%s", &temp);
		select = atoi(temp);

		if (select == 0 && strcmp(temp, "0") == 0)
		{
			printf("�����Ͻ� �����ڵ带 �Է����ּ���. :");
			scanf("%s", &temp2);

			mfc = createMessage(4, user.stdID, temp2, 0);      //��û�� �׸� Ȯ��
			Send(mfc);
			memset(&recvBuf, 0, sizeof(recvBuf));
			Recv(mfs);
			mfs_p = (MessagefromServer*)recvBuf.buf;

			if (mfs_p->Valid == FALSE)
			{
				printf("%s\n", mfs_p->dest);
			}
			else
			{
				printf("%s ������ �Ϸ�Ǿ����ϴ�.", temp2);
			}
			Sleep(1000);
		}
		//fflush(stdin);
		//scanf("%c", &hold);
		//if (hold == '\n') { hold = '\0'; scanf("%c", &hold); }
		nextState = 1;
		break;

	case 4:            //quit
	   //����
		mfc = createMessage(0, user.stdID, "", 0);      //��û�� �׸� Ȯ��
		Send(mfc);
		nextState = 9;
		break;
	case 5: //����

		break;

	default:
		printf("Invalid input error. Return to login phase.\n");
		nextState = 0;
	}
	//������ anum �̿��� ���� ���� ���, �α��� ������� ���ư����� nextState�� 0�� ��ȯ�ϰ� ��.
	return nextState;
}

int UI(int state) {
	int action = 0;
	int retact = 0;      //Action �Լ����� �����ϴ� nextState

	switch (state) {
	case 0:      //login phase
		Sleep(500);
		system("cls");
		printf("�й��� �Է��� �ּ��� : ");
		retact = Action(state);
		break;

	case 1:      //main menu
		system("cls");
		printf("%s�� ȯ���մϴ�.\n", user.name);
		printf("[1] ���� ��ȸ\t[2] ���� ��û\t [3] ��û ����\t[4] ���� : ");
		while (1)
		{
			scanf("%d", &action);
			if (action > 0 || action < 5)
			{
				IsHome = TRUE;            //Ȩ���� �°ǰ�?
				retact = Action(action);   //�Է��� �׼�����
				break;
			}
			else
				printf("�߸��� �Է��Դϴ�.\n");
		}
		break;

	case 2:      //view subjects
		do
		{
			system("cls");
			printf("[�ڵ�]\t\t[�����]\t[����]\t[����/�ִ� ��û�ο�]\n");
			printf("====================================================\n");
			for (int i = 0; i < LISTPERPAGE && (page + i) < total; i++)
				printClassinfo(subject_list[i]);
			printf("0. ����\t1. ��û\t2. ����\t3. Ȩ : ");      //������ ǥ��(���� ������/��ü ������)
			scanf("%d", &action);
			if (action == 0)                              //��û�� �Է����� ���,
			{
				page -= LISTPERPAGE;
				page < 0 ? page = 0 : page;
				retact = Action(1);
			}
			else if (action == 1)
			{
				IsHome = FALSE;
				retact = Action(2);
				break;
			}
			else if (action == 2)
			{
				page += LISTPERPAGE;
				page > total ? page = page - LISTPERPAGE : page;
				retact = Action(1);
			}
			else if (action == 3)
			{
				retact = 1;
				break;
			}
		} while (1);
		//������ �̵��� ���� ������ �ȶ�����. ���߿�..
		break;
	}

	return retact;
}

void printClassinfo(class_info c) {
	printf("%s\t", c.classID);
	printf("%s\t", c.name);
	printf("%s\t", c.prof);
	printf("%d/%d\n", c.cur_req, c.max_acpt);
	printf("���� �ð� : ");
	printTimeinfo(c.time1);
	printTimeinfo(c.time2);
	printf("\n\n");
}

void getrealTime(int time) {
	switch (time) {
	case 0: printf("09:00"); break;
	case 1: printf("10:30"); break;
	case 2: printf("12:00"); break;
	case 3: printf("13:30"); break;
	case 4: printf("15:00"); break;
	case 5: printf("16:30"); break;
	case 6: printf("18:00"); break;
	}
}

void printTimeinfo(classtime t) {
	switch (t.day) {
	case 1: printf("��\t"); break;
	case 2: printf("ȭ\t"); break;
	case 3: printf("��\t"); break;
	case 4: printf("��\t"); break;
	case 5: printf("��\t"); break;
	default: return;
	}
	getrealTime(t.start);
	printf(" ~ ");
	getrealTime(t.end);
	printf("\t");
}
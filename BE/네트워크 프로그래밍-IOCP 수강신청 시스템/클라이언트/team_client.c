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
int page = 0, total;      //현재 읽고있는 페이지, 전체 페이지
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
		ErrorHandling("오류가 발생하였습니다. 잠시후 다시 시도해 주십시오.\n");

	event = WSACreateEvent(); //to global

	//to global_overlapped
	memset(&overlapped, 0, sizeof(overlapped));

	overlapped.hEvent = event;

	//WSABUF dataBuf, recvBuf; <to global
	char message[1024] = { 0, };
	int nextState = 0;
	int cur_action = 0;

	printf("경북대학교 수강신청 클라이언트 ver.0.1\n");
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
} //MessagefromClient 구조체를 생성한다.

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
		fgets(temp, sizeof(temp), stdin);   //학번을 입력받고
		temp[strlen(temp) - 1] = '\0';
		mfc = createMessage(1, temp, "\0", 0);   //메세지 생성
		Send(mfc);                              //서버에 보냄
		memset(&recvBuf, 0, sizeof(recvBuf));
		Recv(mfs);
		mfs_p = (MessagefromServer*)recvBuf.buf;
		if (mfs_p->Valid == TRUE)
		{
			strcpy(user.stdID, temp);
			user = mfs_p->sinfo;
			printf("로그인 성공. 잠시만 기다려주세요.\n");
			Sleep(700);
			nextState = 1;   //다음에 보여줄 UI는 메인 메뉴이다.
		}
		else {
			printf("로그인 실패. 로그인 화면으로 돌아갑니다.\n");
			Sleep(300);
		}
		//recv >> 구조체로 받음. (MessagefromServer)
		//에러를 판단 한다(if문으로)
		//에러가 없으면
		//서버에서 로그인 성공 신호를 받지 못했다면, nextState를 0으로 계속 둘 것.
		break;

	case 1:   //서버에서 과목 리스트 받아옴
		for (int i = 0; i < LISTPERPAGE; i++)         //페이지당 과목 수만큼 요청
		{
			mfc = createMessage(2, "\0", "\0", page + i);      //서버에 과목 요청
			Send(mfc);
			memset(&recvBuf, 0, sizeof(recvBuf));
			Recv(mfs);

			mfs_p = (MessagefromServer*)recvBuf.buf;

			if (mfs_p->Valid)
			{
				subject_list[i] = mfs_p->cinfo;            //클라이언트 저장소에 임시저장
				total = mfs_p->integer;
			}
		}

		nextState = 2;
		break;

	case 2:            //request action
		if (IsHome)
			system("cls");

		memset(temp, 0, sizeof(temp));
		printf("신청할 과목 번호를 입력하세요 : ");
		scanf("%s", temp);

		//과목 list에서 맞는 과목 찾아서 보여줌
		printf("신청하시려면 포인트를 입력하세요. 현재 포인트 [%d] (취소하려면 0을 입력하세요) : ", user.points);
		scanf("%s", &temp2);
		select = atoi(temp2);

		if (select == 0 && strcmp(temp2, "0") == 0)
		{
			nextState = 1;   //메인 화면으로
			break;
		}
		//입력 포인트 bound를 벗어났을 경우 오류 발생.

		mfc = createMessage(3, user.stdID, temp, select);      //서버에 과목 신청
		Send(mfc);
		memset(&recvBuf, 0, sizeof(recvBuf));
		Recv(mfs);

		mfs_p = (MessagefromServer*)recvBuf.buf;
		user = mfs_p->sinfo;

		if (mfs_p->Valid == FALSE)
		{
			printf("수강신청에 실패하였습니다. : %s", mfs_p->dest);
			Sleep(1000);
		}
		else printf("%s 신청되었습니다.\n", temp);
		Sleep(1000);

		mfs_p = (MessagefromServer*)recvBuf.buf;
		//      printf("from server : %s\n", mfs_p->dest);
		   //신청 메세지를 서버에 보냄. 성공했을 경우와 실패했을 경우 그에 맞는 메세지를 출력.
		if (IsHome)   nextState = 1;   //메인 화면으로
		else nextState = 2;
		break;

	case 3:            //view requested class action
	   //student의 requested 과목의 정보와 투자한 포인트를 보여준다.
	   //통계치를 보여준다. (학점 합, 남은 포인트 등)
		system("cls");

		mfc = createMessage(3, user.stdID, "\0", 0);      //신청한 항목 확인
		Send(mfc);
		memset(&recvBuf, 0, sizeof(recvBuf));
		Recv(mfs);
		mfs_p = (MessagefromServer*)recvBuf.buf;
		user = mfs_p->sinfo;
		printf("%s 님의 수강 신청된 항목입니다. [이수학점 : %d / 24]\n",
			user.name, user.cur_gradp);

		printf("잔여 point : %d\n", mfs_p->sinfo.points);
		printf("\n수강신청 내역\n");
		printf("[교과번호]\t[과목명]\t\t사용포인트\n");
		printf("===================================================\n");
		for (int i = 0; i < mfs_p->sinfo.requested_num; i++)
		{
			printf("%s\t", user.requested[i].classID);
			printf("%s\t", user.requested[i].name);
			printf("\t%d\n", user.used_points[i]);
		}
		printf("수정하시려면 0번, 메인 화면으로 돌아가시려면 아무 문자나 입력하세요 : ");
		scanf("%s", &temp);
		select = atoi(temp);

		if (select == 0 && strcmp(temp, "0") == 0)
		{
			printf("삭제하실 과목코드를 입력해주세요. :");
			scanf("%s", &temp2);

			mfc = createMessage(4, user.stdID, temp2, 0);      //신청한 항목 확인
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
				printf("%s 삭제가 완료되었습니다.", temp2);
			}
			Sleep(1000);
		}
		//fflush(stdin);
		//scanf("%c", &hold);
		//if (hold == '\n') { hold = '\0'; scanf("%c", &hold); }
		nextState = 1;
		break;

	case 4:            //quit
	   //종료
		mfc = createMessage(0, user.stdID, "", 0);      //신청한 항목 확인
		Send(mfc);
		nextState = 9;
		break;
	case 5: //수정

		break;

	default:
		printf("Invalid input error. Return to login phase.\n");
		nextState = 0;
	}
	//정해진 anum 이외의 값이 들어올 경우, 로그인 페이즈로 돌아가도록 nextState는 0을 반환하게 됨.
	return nextState;
}

int UI(int state) {
	int action = 0;
	int retact = 0;      //Action 함수에서 리턴하는 nextState

	switch (state) {
	case 0:      //login phase
		Sleep(500);
		system("cls");
		printf("학번을 입력해 주세요 : ");
		retact = Action(state);
		break;

	case 1:      //main menu
		system("cls");
		printf("%s님 환영합니다.\n", user.name);
		printf("[1] 과목 조회\t[2] 과목 신청\t [3] 신청 내역\t[4] 종료 : ");
		while (1)
		{
			scanf("%d", &action);
			if (action > 0 || action < 5)
			{
				IsHome = TRUE;            //홈에서 온건가?
				retact = Action(action);   //입력한 액션으로
				break;
			}
			else
				printf("잘못된 입력입니다.\n");
		}
		break;

	case 2:      //view subjects
		do
		{
			system("cls");
			printf("[코드]\t\t[과목명]\t[교수]\t[현재/최대 신청인원]\n");
			printf("====================================================\n");
			for (int i = 0; i < LISTPERPAGE && (page + i) < total; i++)
				printClassinfo(subject_list[i]);
			printf("0. 이전\t1. 신청\t2. 다음\t3. 홈 : ");      //페이지 표시(현재 페이지/전체 페이지)
			scanf("%d", &action);
			if (action == 0)                              //신청을 입력했을 경우,
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
		//페이지 이동은 아직 생각이 안떠오름. 나중에..
		break;
	}

	return retact;
}

void printClassinfo(class_info c) {
	printf("%s\t", c.classID);
	printf("%s\t", c.name);
	printf("%s\t", c.prof);
	printf("%d/%d\n", c.cur_req, c.max_acpt);
	printf("강의 시간 : ");
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
	case 1: printf("월\t"); break;
	case 2: printf("화\t"); break;
	case 3: printf("수\t"); break;
	case 4: printf("목\t"); break;
	case 5: printf("금\t"); break;
	default: return;
	}
	getrealTime(t.start);
	printf(" ~ ");
	getrealTime(t.end);
	printf("\t");
}
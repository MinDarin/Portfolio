#include <WinSock2.h>

#define STUDENT      "student.txt"
#define SUBJECT      "subject.txt"
#define BUF_SIZE   100
#define FALSE      0
#define TRUE      1
#define READ      3
#define WRITE      5
#define LISTPERPAGE 2            //페이지당 표시되는 과목 수
#define student_size 3
#define subject_size 5

// socket info
typedef struct {
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

// buffer info
typedef struct {
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE * 500];
	int rwMode;                  // READ or WRITE
} PER_IO_DATA, *LPPER_IO_DATA;

//수업 시간 구조체
typedef struct {
	int day;                  //요일. 1-5 월-금으로 정한다.
	int start;
	int end;                        //시간. 0 : 09:00 / 1 : 10:30 / 2 : 12:00 / 3 : 13:30 / 4 : 15:00 / 5 : 16:30 / 6 : 18:00 
}classtime;

//과목 구조체
typedef struct {
	char   classID[11];            //과목코드
	char   name[50];            //과목 이름
	char   prof[15];            //담당 교수
	int      max_acpt;            //수강가능인원(최대)
	int      cur_req;            //현재 신청 인원. 초기값 0
	int      grad;                  //과목 학점
	classtime time1;
	classtime time2;            //수업 시간 객체. 한 주에 한 번만 수업할 경우 time2는 사용하지 않는다.
}class_info;

//학생 구조체
typedef struct {
	char    stdID[11];              //학번
	char    name[20];               //이름
	int      points;                //수강신청 시 사용하는 포인트. 초기값은 100이 될 것이다.
	int      cur_gradp;             //이수학점 (최대 24)
	int      requested_num;         //현재 몇과목을 신청하였는가
	class_info    requested[12];   //학생이 신청한 과목 목록
	int used_points[12];            //위의 구조체 배열과 대응됨. 각 과목마다 학생이 사용한 point이다.
}student_info;

typedef struct {
	//   char sender[11];            //발신자 학번
	int msgtype;               //메세지 타입. 1 : 학생 정보 요청 / 2 : 과목 리스트 요청 / 3 : 신청
	char stringtemp[BUF_SIZE];      //주 버퍼
	char assist[BUF_SIZE];         //보조 버퍼
	int integer;                    //상수
}MessagefromClient;

/* Message Format (from client)
   type 1 : stringtemp <- 학번
   type 2 : stringtemp <- 과목코드
   type 3 : stringtemp <- 학번
   assist            <- 과목코드   / integer : 사용한 point
*/

typedef struct {
	char dest[BUF_SIZE];               //목적지. 일반적인 경우 보낸 client의 학번이 됨.
	student_info sinfo;
	class_info cinfo;
	int Valid;                  //0이면 valid 아니면 invalid
	int integer;               //가변사용 가능한 정수
}MessagefromServer;
//두 구조체 중 하나를 보내게 될텐데, 보내지 않는 구조체는 null로 설정한 후 보낼 것.
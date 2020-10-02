#include <WinSock2.h>

#define STUDENT      "student.txt"
#define SUBJECT      "subject.txt"
#define BUF_SIZE   100
#define FALSE      0
#define TRUE      1
#define READ      3
#define WRITE      5
#define LISTPERPAGE 2            //�������� ǥ�õǴ� ���� ��
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

//���� �ð� ����ü
typedef struct {
	int day;                  //����. 1-5 ��-������ ���Ѵ�.
	int start;
	int end;                        //�ð�. 0 : 09:00 / 1 : 10:30 / 2 : 12:00 / 3 : 13:30 / 4 : 15:00 / 5 : 16:30 / 6 : 18:00 
}classtime;

//���� ����ü
typedef struct {
	char   classID[11];            //�����ڵ�
	char   name[50];            //���� �̸�
	char   prof[15];            //��� ����
	int      max_acpt;            //���������ο�(�ִ�)
	int      cur_req;            //���� ��û �ο�. �ʱⰪ 0
	int      grad;                  //���� ����
	classtime time1;
	classtime time2;            //���� �ð� ��ü. �� �ֿ� �� ���� ������ ��� time2�� ������� �ʴ´�.
}class_info;

//�л� ����ü
typedef struct {
	char    stdID[11];              //�й�
	char    name[20];               //�̸�
	int      points;                //������û �� ����ϴ� ����Ʈ. �ʱⰪ�� 100�� �� ���̴�.
	int      cur_gradp;             //�̼����� (�ִ� 24)
	int      requested_num;         //���� ������� ��û�Ͽ��°�
	class_info    requested[12];   //�л��� ��û�� ���� ���
	int used_points[12];            //���� ����ü �迭�� ������. �� ���񸶴� �л��� ����� point�̴�.
}student_info;

typedef struct {
	//   char sender[11];            //�߽��� �й�
	int msgtype;               //�޼��� Ÿ��. 1 : �л� ���� ��û / 2 : ���� ����Ʈ ��û / 3 : ��û
	char stringtemp[BUF_SIZE];      //�� ����
	char assist[BUF_SIZE];         //���� ����
	int integer;                    //���
}MessagefromClient;

/* Message Format (from client)
   type 1 : stringtemp <- �й�
   type 2 : stringtemp <- �����ڵ�
   type 3 : stringtemp <- �й�
   assist            <- �����ڵ�   / integer : ����� point
*/

typedef struct {
	char dest[BUF_SIZE];               //������. �Ϲ����� ��� ���� client�� �й��� ��.
	student_info sinfo;
	class_info cinfo;
	int Valid;                  //0�̸� valid �ƴϸ� invalid
	int integer;               //������� ������ ����
}MessagefromServer;
//�� ����ü �� �ϳ��� ������ ���ٵ�, ������ �ʴ� ����ü�� null�� ������ �� ���� ��.
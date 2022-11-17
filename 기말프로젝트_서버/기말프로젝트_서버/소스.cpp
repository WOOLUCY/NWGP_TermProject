#include "SendRecvData.h"
#include "Common.h"


#define SERVERPORT 9000
#define BUFSIZE    512

HANDLE hWriteEvent;
HANDLE hReadEvent;
int buf[BUFSIZE];

DWORD WINAPI WriteThread(LPVOID arg)
{
	DWORD retval;
	for (int k = 1; k <= 500; k++) {
		// 읽기 완료 대기
		retval = WaitForSingleObject(hReadEvent, INFINITE);
		if (retval != WAIT_OBJECT_0) break;

		// 공유 버퍼에 데이터 저장
		for (int i = 0; i < BUFSIZE; i++)
			buf[i] = k;

		// 쓰기 완료 알림
		SetEvent(hWriteEvent);
	}
	return 0;
}

DWORD WINAPI Recv_Thread(LPVOID arg)
{
//	아래 주석 처리 코드는 책의 이벤트 예제임
//	DWORD retval;
//	while (1) {
//		// 쓰기 완료 대기
//		retval = WaitForSingleObject(hWriteEvent, INFINITE);
//		if (retval != WAIT_OBJECT_0) break;
//
//		// 읽은 데이터 출력 후 버퍼를 0으로 초기화
//		printf("Thread %4d:\t", GetCurrentThreadId());
//		for (int i = 0; i < BUFSIZE; i++)
//			printf("%3d ", buf[i]);
//		printf("\n");
//		memset(buf, 0, sizeof(buf));
//
//		// 읽기 완료 알림
//		SetEvent(hReadEvent);
//	}
//	return 0;

	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	int addrlen;
	char addr[INET_ADDRSTRLEN];
	char buf[BUFSIZE + 1]; // 가변 길이 데이터
	int len;

	RecvPlayerData *player;

	char testData[BUFSIZE + 1] = "\0";
	
	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// 클라이언트와 데이터 통신
	while (1) {
		// 가변 길이
		retval = recv(client_sock, (char*)&len, sizeof(int), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) break;

		// ID recv
		retval = recv(client_sock, buf, len, MSG_WAITALL);
		buf[retval] = '\0';

		sprintf(testData, buf);

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) break;

		// player = (RecvPlayerData*)buf;

		printf("접속한 Player의 ID: %s", testData);


	}
	printf("\n#No.%d '%s' SENDING COMPLATE\n", client_sock, testData);
	// 소켓 닫기
	closesocket(client_sock);

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
	return 0;
}

int main(int argc, char* argv[])
{
	// 이벤트 생성
	// hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// 스레드 세 개 생성
	//HANDLE hThread[3];
	//hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	//hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	//hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

	//// 읽기 완료 알림
	//SetEvent(hReadEvent);

	//// 스레드 세 개 종료 대기
	//WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	//// 이벤트 제거
	//CloseHandle(hWriteEvent);
	//CloseHandle(hReadEvent);
	//return 0;


	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;


	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));


		printf("\n============================================================================\n");
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n\n", addr, ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, Recv_Thread,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else CloseHandle(hThread);
	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;

}

#include "Common.h"
#include "SendRecvData.h"
#include <vector>
#include "Platform.h"
#include "Coin.h"

#define SERVERPORT 9000
#define BUFSIZE    512

#define PLATFORMNUM 10
#define COINNUM 10

HANDLE hWriteEvent;
HANDLE hReadEvent;
int buf[BUFSIZE];


Player user1;

vector<Platform> platform;
vector<Coin> coins;

ServerToClient TestData;


void UpdatePlayerLocation(Player* p);

void InitPlatform()
{
	//일단 100,200,300,400, 이렇게 때려넣겠음

	for (int i = 0; i < PLATFORMNUM; ++i) {
		platform.push_back(Platform(i+10, i * 50));
		printf("%d %d\n", platform[i].send.iXpos, platform[i].send.iYpos);
	}


}

void InitCoin()
{

	for (int i = 0; i < COINNUM; ++i) {
		coins.push_back(Coin(i *i, i * 50-30));
		printf("%d %d\n", coins[i].send.iXpos, coins[i].send.iYpos);
	}
}

DWORD WINAPI Send_Thread(LPVOID arg)
{

	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	int addrlen;
	char addr[INET_ADDRSTRLEN];

	char buf[BUFSIZE + 1]; // 가변 길이 데이터
	int len;


	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// 클라이언트와 데이터 통신
	while (1) {
		Sleep(16);
		UpdatePlayerLocation(&user1);

		TestData.player1 = user1.Send;
		
		retval = send(client_sock, (char*)&TestData, sizeof(TestData), 0);

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}



	}

	printf("\n#No.%d '%s' SENDING COMPLATE\n", client_sock, "end");
	// 소켓 닫기
	closesocket(client_sock);

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
	return 0;

}
DWORD WINAPI Recv_Thread(LPVOID arg)
{


	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	int addrlen;
	char addr[INET_ADDRSTRLEN];
	char buf[BUFSIZE + 1]; // 가변 길이 데이터
	
	ClientToServer * recvData;
	

	
	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// 클라이언트와 데이터 통신
	while (1) {
		// ID recv
		retval = recv(client_sock, buf, sizeof(ClientToServer), MSG_WAITALL);
		buf[retval] = '\0';

		recvData = (ClientToServer*)buf;

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) break;

		 printf("\n접속한 Player의 ID: %ws", recvData->wId);
		 printf("\n접속한 Player의 캐릭터: %d\n", recvData->uCharNum);

		 printf("bLeft: %s\n", recvData->Input.bLeft ? "true" : "false");
		 printf("bRight: %s\n", recvData->Input.bRight ? "true" : "false");
		 printf("bSpace: %s\n", recvData->Input.bSpace ? "true" : "false");

		 //printf("Collided Monster Number: %d\n", player->pPlayer.GetXPos());



		 user1.input.bLeft = recvData->Input.bLeft;

		 static bool first = true;
		 if (first) {
			 user1.input = recvData->Input;
			 if (recvData->Input.bSpace)
				 first = false;

		 }
		 else {
			 if (user1.input.bSpace == false || user1.bJumpKeyPressed == false) {
				 first = true;
			 }
		 }
		 //인픗을 초기화 해주니까 생긴문제임


		 user1.input.bLeft = recvData->Input.bLeft;

		 user1.input.bRight = recvData->Input.bRight;



	}
	printf("\n#No.%d '%ws' SENDING COMPLATE\n", client_sock, recvData->wId);
	// 소켓 닫기
	closesocket(client_sock);

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
	return 0;
}

void UpdatePlayerLocation(Player* p)
{
	static int curSpriteCnt = 0;



	if (!(p->input.bLeft)) {
		//왼쪽으로 이동
		p->velocity.x = 0;
		p->SetSpriteY(4);



	}

	if (!(p->input.bRight)) {
		//왼쪽으로 이동
		p->velocity.x = 0;
		p->SetSpriteY(0);

	}


	if (p->input.bLeft) {
		//왼쪽으로 이동
		curSpriteCnt = 3;

		p->velocity.x = -p->GetRunSpeed();
		p->SetSpriteY(3);

	}


	if (p->input.bRight) {
		//오른쪽으로 이동
		curSpriteCnt = 1;

		p->velocity.x = p->GetRunSpeed();
		p->SetSpriteY(1);
	}






	if (p->input.bSpace) {
		//점프
		p->SetSpriteY(2);
		p->bJumpKeyPressed = TRUE;
		p->Jump(curSpriteCnt);


	}



	p->UpdatePlayerLocation();



}


int main(int argc, char* argv[])
{

	InitPlatform();
	InitCoin();

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

	HANDLE hThread[2];


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

		//가온 - 한번보내고 안보낼 데이터 여기서 스레드생성전에 전송
		//일단 발판 개수 전송- 고정길이
		int tmp = PLATFORMNUM;
		retval = send(client_sock, (char*)&tmp, sizeof(int), 0);
		printf("%d\n", tmp);

		for (int i{ 0 }; i < tmp; ++i) {
			retval = send(client_sock, (char*)&platform[i].send.iXpos, sizeof(int)*2, 0);
			printf("%d %d \t", platform[i].send.iXpos, platform[i].send.iYpos);

		}

		tmp = COINNUM;
		retval = send(client_sock, (char*)&tmp, sizeof(int), 0);
		printf("%d\n", tmp);

		for (int i{ 0 }; i < tmp; ++i) {
			retval = send(client_sock, (char*)&coins[i].send.iXpos, sizeof(int) * 2, 0);
			printf("%d %d \t", coins[i].send.iXpos, coins[i].send.iYpos);

		}


		// 스레드 생성
		hThread[0] = CreateThread(NULL, 0, Recv_Thread, (LPVOID)client_sock, 0, NULL);
		hThread[1] = CreateThread(NULL, 0, Send_Thread, (LPVOID)client_sock, 0, NULL);




		if (hThread == NULL) { closesocket(client_sock); }
		else CloseHandle(hThread);
	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;

}



//발판 초기화 함수

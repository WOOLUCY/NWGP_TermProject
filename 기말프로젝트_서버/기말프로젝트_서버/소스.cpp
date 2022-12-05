#include "Common.h"
#include "global.h"
#include "SendRecvData.h"
#include <vector>
#include <algorithm>
#include <locale.h>
#include "Platform.h"
#include "Coin.h"

#define SERVERPORT 9000
#define BUFSIZE    512

#define PLATFORMNUM 10
#define COINNUM 10
#define MONSTERNUM 10


HANDLE hEventHandle;

int TotalClient;


Player				users[3];

vector<Platform>	platform;
vector<Coin>		coins;
//vector<CMonster>	monsters;

CMonster			cmonsters[MONSTERNUM];

int					backgroundMove;
int					eventCnt;	// 접속 유저들의 스레드 중 한 개만 event를 하기 위함

int	playerSpriteCnt = 0;	// 스프라이트 카운트 변수
int	MonsterSpriteCnt = 0;	// 스프라이트 카운트 변수

ServerToClient		SendData;


void UpdatePlayerLocation(Player* p);
void ChangePlayerSprite(Player* p, int* count);
void ChangeMonsterSprite(int* count);
void UpdateMonsters();

void InitPlatform()
{
	//일단 100,200,300,400, 이렇게 때려넣겠음

	for (int i = 0; i < PLATFORMNUM; ++i) {
		platform.push_back(Platform(i+10, i * 50));
		//printf("%d %d\n", platform[i].send.iXpos, platform[i].send.iYpos);
	}


}

void InitCoin()
{

	for (int i = 0; i < COINNUM; ++i) {
		coins.push_back(Coin(i *i, i * 50-30));
		//printf("%d %d\n", coins[i].send.iXpos, coins[i].send.iYpos);
	}
}

void InitMonster()
{
	// semin, 몬스터 벡터 -> 배열 바꾸면서 send 방법도 바꿈. 
	// 여기서 recv 하는 게 아니라 send_thread 에서 처음 init 데이터도 보냄.
	// 어차피 몬스터는 이동하면서 계속 보내질 것이기 때문에 send_thread 에 
	// 합치는 게 나을 것이라 판단

	for (int i{ 0 }; i < MONSTERNUM; ++i) {
		// monsters.push_back(CMonster(i * i, i));
		cmonsters[i].send.iXpos = i * i;
		cmonsters[i].send.iYpos = i + i * 10;
	}
}

void InitPlayer(int num, Player* p)
{
	if (num == 0) {
		p->SetHeart(5);
		p->SetRunSpeed(p->GetRunSpeed() * 0.75);

		printf("InitPlayer호출됨 %d %f\n", 5, p->GetRunSpeed() * 0.75);
	}
	else if (num == 1) {
		p->SetHeart(4);
		p->SetRunSpeed(p->GetRunSpeed());

		printf("InitPlayer호출됨 %d %f\n", 5, p->GetRunSpeed() * 0.75);

	}
	else if (num == 2) {
		p->SetHeart(3);
		p->SetRunSpeed(p->GetRunSpeed() * 1.25);
		printf("InitPlayer호출됨 %d %f\n", 3, p->GetRunSpeed() * 0.75);


	}
}



DWORD WINAPI Update_Thread(LPVOID arg)
{
	const int index = TotalClient - 1;
	clock_t start = clock(), pre = clock();
	double time = 0;
	if (TotalClient == 3)
		clock_t start = clock();
	// 클라이언트와 데이터 통신

	while (1) {
		if (users[index].GetCharNum() == 10000) break;

		WaitForSingleObject(hEventHandle, INFINITE);
		// 몬스터 스프라이트 업데이트 ( 이동도 여기서 하면 될 듯 )
		ChangeMonsterSprite(&MonsterSpriteCnt);
		ChangePlayerSprite(&(users[index]), &playerSpriteCnt);

		UpdateMonsters();

		// semin, 게임 시간
		if (index == 2) {	// 마지막 접속한 사람의 thread에서 계산함
			pre = clock();
			time = (pre - start);
			printf("%f 초 \n", time / CLOCKS_PER_SEC);
			if ((double)(time) / CLOCKS_PER_SEC >= 120 ) {	// 120초(2분) 지나면 게임 끝
				printf("게임 끝났음\n");
				SendData.bIsPlaying = FALSE;
			}
			SendData.ServerTime = time;	// time / CLOCKS_PER_SEC 하면 초 단위로 나온다
		}
		SetEvent(hEventHandle);

		Sleep(16);
	}

	SendData.player[index].charNum = 999;
	return 0;

}


DWORD WINAPI Send_Thread(LPVOID arg)
{

	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	char addr[INET_ADDRSTRLEN];

	char buf[BUFSIZE + 1]; 
	int len;

	const int index = TotalClient - 1;

	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));


	//const int index = TotalClient - 1;
	clock_t start = clock(), pre = clock();
	double time = 0;
	if (TotalClient == 3)
		clock_t start = clock();


	// 클라이언트와 데이터 통신

	while (1) {
		if (users[index].GetCharNum() == 10000) break;;
		UpdatePlayerLocation(&(users[index]));
	//	ChangePlayerSprite(&(users[index]), &playerSpriteCnt);

		//ChangePlayerSprite(&(users[index]), &playerSpriteCnt); ->이거 update로 옮김
		//UpdateMonsters();


//여기부터
		WaitForSingleObject(hEventHandle, INFINITE);
		// 몬스터 스프라이트 업데이트 ( 이동도 여기서 하면 될 듯 )
		ChangeMonsterSprite(&MonsterSpriteCnt);
		ChangePlayerSprite(&(users[index]), &playerSpriteCnt);
		UpdateMonsters();

		// semin, 게임 시간
		if (index == 2) {	// 마지막 접속한 사람의 thread에서 계산함
			pre = clock();
			time = (pre - start);
			printf("%f 초 \n", time / CLOCKS_PER_SEC);
			if ((double)(time) / CLOCKS_PER_SEC >= 120) {	// 120초(2분) 지나면 게임 끝
				printf("게임 끝났음\n");
				SendData.bIsPlaying = FALSE;
			}
			SendData.ServerTime = time;	// time / CLOCKS_PER_SEC 하면 초 단위로 나온다
		}
		SetEvent(hEventHandle);
//여긱까지

		Sleep(16);
		SendData.player[index] = users[index].Send;
		wcscpy(SendData.player[index].wID, users[index].Send.wID);
		for (int i = 0; i < MONSTERNUM; i++) {
			SendData.monsters[i] = cmonsters[i].send;
		}
		
		retval = send(client_sock, (char*)&SendData, sizeof(SendData), 0);


		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}

	}
	SendData.player[index].charNum = 999;
	printf("\n#No.%d '%s' SENDING COMPLATE\n", client_sock, "end");
	// 소켓 닫기
	//closesocket(client_sock);

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
	return 0;

}

DWORD WINAPI Recv_Thread(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	int option = TRUE;               //네이글 알고리즘 on/off
	setsockopt(client_sock,             //해당 소켓
		IPPROTO_TCP,          //소켓의 레벨
		TCP_NODELAY,          //설정 옵션
		(const char*)&option, // 옵션 포인터
		sizeof(option));      //옵션 크기
	struct sockaddr_in clientaddr;
	int addrlen;
	char addr[INET_ADDRSTRLEN];
	char buf[BUFSIZE + 1]; // 가변 길이 데이터
	
	ClientToServer* recvData;

	const int index = TotalClient - 1;
	
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

		if (recvData->uCharNum == 10000) break;

		 users[index].Send.charNum = recvData->uCharNum - 1;
		 users[index].SetId(recvData->wId);
		 wcscpy(users[index].Send.wID, recvData->wId);


		 if (users[index].GetHeart() == 0) {
			 InitPlayer(users[index].Send.charNum, &users[index]);
		 }



		 if (users[index].bJumpKeyPressed == true) {
			 users[index].input.bLeft = recvData->Input.bLeft;
			 users[index].input.bRight = recvData->Input.bRight;
			 users[index].up.bLeft = recvData->Up.bLeft;
			 users[index].up.bRight = recvData->Up.bRight;


		 }
		 else {
			 users[index].input = recvData->Input;
			 users[index].up = recvData->Up;

		 }


		 printf("\n접속한 Player의 ID: %ws", users[index].Send.wID);
		 printf("\n접속한 Player의 캐릭터: %d\n", recvData->uCharNum);
		 printf("\n접속한 Player의 캐릭터: %d\n", users[index].Send.charNum);
		 printf("\n접속한 Player의 인덱스: %d\n", index);

		 printf("bLeft: %s\n", recvData->Input.bLeft ? "true" : "false");
		 printf("bRight: %s\n", recvData->Input.bRight ? "true" : "false");
		 printf("bSpace: %s\n", recvData->Input.bSpace ? "true" : "false");


		 printf("bLeft: %s\n", users[index].input.bLeft ? "true" : "false");
		 printf("bRight: %s\n", users[index].input.bRight ? "true" : "false");
		 printf("bSpace: %s\n", users[index].input.bSpace ? "true" : "false");

		

	}
	printf("\n#No.%d '%ws' Recv_Thread COMPLATE\n", client_sock, recvData->wId);
	// 소켓 닫기
	//closesocket(client_sock);

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
	--TotalClient;
	return 0;
}

void ChangePlayerSprite(Player* p, int* count)
{
	// count 는 timer 의 시간에 따라 1 씩 올라감
	// 밑의 조건문에 count == "숫자" 숫자 올릴수록 스프라이트는 천천히 돌아갈 것
	if (*count == 3) {
		p->uSpriteX = (p->uSpriteX + 1) % 4;
		*count = 0;
	}
	*count += 1;

	printf("[%d] 번 스프라이트 업데이트불림\n", p->GetCharNum());
}


void ChangeMonsterSprite(int* count)
{
	if (*count == 3) {
		for (int i = 0; i < MONSTERNUM; i++) {
			cmonsters[i].send.uSpriteX = (cmonsters[i].send.uSpriteX + 1) % 8;
			*count = 0;
		}
	}
	*count += 1;
}


void UpdateMonsters()
{
	static int i{ 0 };
	//몬스터 위치그거 하겠슴다. 

	for (int i{ 0 }; i < MONSTERNUM; ++i) {
		cmonsters[i].UpdateMonsterLocation(&SendData.monsters[i]);
		++i;
		if (i == MONSTERNUM - 1) i = 0;
	}


}

void UpdatePlayerLocation(Player* p)
{
	static int curSpriteCnt = 0;

	if (!(p->input.bLeft) && (p->up.bLeft)) {
		//왼쪽으로 이동
		p->velocity.x = 0;
		p->SetSpriteY(4);
	}

	if (!(p->input.bRight) && (p->up.bRight)) {
		//왼쪽으로 이동
		p->velocity.x = 0;
		p->SetSpriteY(0);
	}

	// 캐릭터의 위치가 이동할 때는 배경화면이 끝에 갔을 때 ( 왼쪽 끝 )
	if (p->Send.iBgMove <= -200 ) {
		if (p->input.bLeft) {
			curSpriteCnt = 3;
			p->iXpos += -p->GetRunSpeed();
			p->SetSpriteY(3);

		}

		if (p->input.bRight) {
			curSpriteCnt = 1;
			p->iXpos += p->GetRunSpeed();
			p->SetSpriteY(1);
			if ( p->Send.iXpos >= 640 )
				p->Send.iBgMove = -195;

		}
	}

	// 캐릭터의 위치가 이동할 때는 배경화면이 끝에 갔을 때 ( 오른쪽 끝 )
	else if (p->Send.iBgMove >= 1200) {
		if (p->input.bLeft) {
			curSpriteCnt = 3;
			p->iXpos += -p->GetRunSpeed();
			p->SetSpriteY(3);
			if (p->Send.iXpos <= 640)
				p->Send.iBgMove = 1195;
		}

		if (p->input.bRight) {
			curSpriteCnt = 1;
			p->iXpos += p->GetRunSpeed();
			p->SetSpriteY(1);
		}
	}

	/* 그 외에는 배경화면이 이동 */
	else {
		if (p->input.bLeft) {
			//왼쪽으로 이동
			curSpriteCnt = 3;
			//p->velocity.x = -p->GetRunSpeed();
			p->Send.iBgMove += -p->GetRunSpeed();
			p->Send.iBgMove = std::clamp(p->Send.iBgMove, -200, 1200);
			p->SetSpriteY(3);
		}

		if (p->input.bRight) {
			//오른쪽으로 이동
			curSpriteCnt = 1;

			//p->velocity.x = p->GetRunSpeed();
			p->Send.iBgMove += p->GetRunSpeed();
			p->Send.iBgMove = std::clamp(p->Send.iBgMove, -200, 1200);
			p->SetSpriteY(1);
		}
	}
	if (p->input.bSpace) {
		//왼쪽 점프
		p->bJumpKeyPressed = TRUE;
		p->Jump(curSpriteCnt);
	}

	p->UpdatePlayerLocation();
}


int main(int argc, char* argv[])
{

	InitPlatform();
	InitCoin();
	InitMonster();

	int retval;
	setlocale(LC_ALL, "KOREAN");

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

	HANDLE hThread[3];

	// 이벤트 생성
	hEventHandle = CreateEvent(NULL, FALSE, TRUE, NULL);

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
		++TotalClient;

		printf("총 접속 클라이언트 : %d\n", TotalClient);
		printf("\n============================================================================\n");



		//가온 - 한번보내고 안보낼 데이터 여기서 스레드생성전에 전송
		//일단 발판 개수 전송- 고정길이


		for (int i{ 0 }; i < PLATFORMNUM; ++i) {
			retval = send(client_sock, (char*)&platform[i].send.iXpos, sizeof(int)*2, 0);

		}


		for (int i{ 0 }; i < COINNUM; ++i) {
			retval = send(client_sock, (char*)&coins[i].send.iXpos, sizeof(int) * 2, 0);

		}




		// 스레드 생성
		hThread[0] = CreateThread(NULL, 0, Recv_Thread, (LPVOID)client_sock, 0, NULL);
		hThread[1] = CreateThread(NULL, 0, Send_Thread, (LPVOID)client_sock, 0, NULL);
		//hThread[2] = CreateThread(NULL, 0, Update_Thread, (LPVOID)client_sock, 0, NULL);



		

		if (hThread == NULL) { closesocket(client_sock); }
		else CloseHandle(hThread);
	}

	// 이벤트 제거
	CloseHandle(hEventHandle);

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;

}



//발판 초기화 함수

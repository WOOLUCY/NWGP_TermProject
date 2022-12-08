#include "Common.h"
#include "global.h"
#include "SendRecvData.h"
#include <vector>
#include <algorithm>
#include <locale.h>
#include "Platform.h"
#include "Coin.h"

#define SERVERPORT 9000
#define BUFSIZE    128



// 이벤트 생성
HANDLE hWriteEvent;

int TotalClient;

bool				bFirstSelected = FALSE;
bool				bSecondSelected = FALSE;
bool				bThirdSelected = FALSE;


Player				users[3];

//vector<Platform>	platform;
Platform			platform[PLATFORMNUM];

Coin				coins[COINNUM];
CMonster			cmonsters[MONSTERNUM];

int					backgroundMove;

ServerToClient		SendData;

struct collidedef {
	bool iscrush = false;
	int crushnum;
	int index;
};

collidedef CoinCollide;
collidedef MonCollide;


void UpdatePlayerLocation(Player* p);
void ChangePlayerSprite(Player* p, int* count);
void ChangeMonsterSprite(int* count);
void ChangeCoinSprite(int* count);
void UpdateMonsters();
void WhosWinner();


void InitPlatform()
{
	//윈도우사이즈 - 1280, 800
	//바닥 = 800-336

	//**플랫폼추가하려면 서버,클라 #define 상수 고치기

	int height = 120;
	int width = 250;

	int floor = 800;

	
	//1층 
	platform[0] = Platform(0, floor - height * 2);
	platform[1] = Platform(0, floor - height*2);
	platform[2] = Platform(width*3, floor - height*2);
	platform[3] = Platform(width * 5, floor - height * 2);

	//2층
	platform[4] = Platform(width, floor - height*3);
	platform[5] = Platform(width*2, floor - height*3);
	platform[6] = Platform(width*4, floor - height*3);

	//3층
	platform[7] = Platform(-100, floor - height*4);
	platform[8] = Platform(width*5.5, floor - height*4);

	//4충
	platform[9] = Platform(600, floor - height*5);
	//platform[10] = (Platform(700, floor - height*5));


}

void InitCoin()
{

	for (int i = 0; i < COINNUM; ++i) {
		coins[i].send.iXpos = i * 100 - 400;
		coins[i].send.iYpos = 650;
		coins[i].CoinUpdate();
	}
}
void InitMonster()
{
	// semin, 몬스터 벡터 -> 배열 바꾸면서 send 방법도 바꿈. 
	// 여기서 recv 하는 게 아니라 send_thread 에서 처음 init 데이터도 보냄.
	// 어차피 몬스터는 이동하면서 계속 보내질 것이기 때문에 send_thread 에 
	// 합치는 게 나을 것이라 판단

	for (int i{ 0 }; i < MONSTERNUM; ++i) {
		cmonsters[i].send.iXpos = i * 150 - 200;
		cmonsters[i].send.iYpos = 620;
		cmonsters[i].updateRange();
		cmonsters[i].SetMonNum(i);
	}
}
void InitPlayer(int num, Player* p)
{
	if (num == 0) {
		p->Send.uHeart = 5;
		p->SetRunSpeed(p->GetRunSpeed() * 1);

		printf("InitPlayer호출됨 %d %f\n", 5, p->GetRunSpeed() * 1);
	}
	else if (num == 1) {
		p->Send.uHeart = 4;
		p->SetRunSpeed(p->GetRunSpeed()*1.25);

		printf("InitPlayer호출됨 %d %f\n", 5, p->GetRunSpeed() * 1.25);

	}
	else if (num == 2) {
		p->Send.uHeart = 3;
		p->SetRunSpeed(p->GetRunSpeed() * 1.5);
		printf("InitPlayer호출됨 %d %f\n", 3, p->GetRunSpeed() * 1.5);


	}
}


int	MonsterSpriteCnt = 0;	// 스프라이트 카운트 변수
int CoinSpriteCnt = 0;

DWORD WINAPI Update_Thread(LPVOID arg)
{
	clock_t start = clock(), pre = clock();
	double time = 0;
	// 클라이언트와 데이터 통신
	int cnt = 0;

	while (1) {

		WaitForSingleObject(hWriteEvent, INFINITE);

		if (cnt == 0 && TotalClient == 3) {
			start = clock();
			cnt++;
		}

		if (CoinCollide.iscrush) {
			users[CoinCollide.index].Send.uScore += 1;
			SendData.player[CoinCollide.index].uScore= users[CoinCollide.index].Send.uScore;//여기서 코인점수 업테이트 해야할듯
			printf("[%d]번코임충돌,코인점수 [%d]\n",CoinCollide.index, SendData.player[CoinCollide.index].uScore);

			CoinCollide.iscrush = false;
			ResetEvent(hWriteEvent);

		}

		if (MonCollide.iscrush) {
			users[MonCollide.index].Send.uScore += 2;
			SendData.player[MonCollide.index].uScore = users[MonCollide.index].Send.uScore; //여기서 코인점수 업테이트 해야할듯

			printf("%d번몬스터충돌함\n", MonCollide.crushnum);
			//여기서 몬스터충돌시하는거 업테이트 하면될듯
			MonCollide.iscrush = false;

			ResetEvent(hWriteEvent);
		}



		// 몬스터 스프라이트 업데이트 ( 이동도 여기서 하면 될 듯 )
		//printf("UpdateThread불림\n");
		ChangeMonsterSprite(&MonsterSpriteCnt);
		ChangeCoinSprite(&CoinSpriteCnt);
		UpdateMonsters();


		// W, 게임 시작 여부
		if (bFirstSelected && bSecondSelected && bThirdSelected) {
			SendData.bIsPlaying = TRUE;
		}
		// semin, 게임 시간
		if (TotalClient == 3) {	// 마지막 접속한 사람의 thread에서 계산함
			pre = clock();
			time = (pre - start);
			//printf("%f 초 \n", time / CLOCKS_PER_SEC);
			if ((double)(time) / CLOCKS_PER_SEC >= 120 ) {	// 120초(2분) 지나면 게임 끝
				//printf("게임 끝났음\n");
				WhosWinner();
				SendData.bGameEnd = TRUE;
			}
			SendData.ServerTime = time;	// time / CLOCKS_PER_SEC 하면 초 단위로 나온다
		}
		Sleep(16);
	}


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



	int	playerSpriteCnt = 0;	// 스프라이트 카운트 변수

	// 클라이언트와 데이터 통신

	while (1) {
		//printf("[%d]번째 sendThread\n", index);

		if (users[index].GetCharNum() == 10000) break;
		UpdatePlayerLocation(&(users[index]));

		ChangePlayerSprite(&(users[index]), &playerSpriteCnt);

		if (users[index].Send.uHeart == 0) {
			users[index].Send.uHeart = 4;
			//InitPlayer(users[index].Send.charNum, &users[index]);
		}

		// 몬스터 충돌
		for (int i = 0; i < MONSTERNUM; i++) {
			if (0 != users[index].IsCollidedMonster(&cmonsters[i])) {
				MonCollide.iscrush = true;
				MonCollide.crushnum = i;
				MonCollide.index = index;

				SetEvent(hWriteEvent);
			}
		}
		// 코인 충돌
		for (int i = 0; i < COINNUM; i++) {
			if (coins[i].send.bIsCrush == FALSE&&users[index].IsCollidedCoin(&coins[i])) {
				CoinCollide.iscrush = true;
				CoinCollide.crushnum = i;
				CoinCollide.index = index;

				SetEvent(hWriteEvent);
			}

		}
		// 플랫폼 충돌
		for (int i = 0; i < PLATFORMNUM; i++) {
			users[index].IsCollidedPlatform(platform[i]);
			users[index].killMonster = FALSE;
		}
		// 만약 플랫폼 충돌이 아닐 때 낙하
		bool check = false;
		for (int i = 0; i < PLATFORMNUM; i++) {
			if (users[index].IsNotCollidedPlatform(platform[i])) {
				check = true;
			}
		}
		if (users[index].killMonster == FALSE && users[index].bJumpKeyPressed == FALSE && check == false && !(users[index].Send.iYpos > 610)) {
			users[index].velocity.y = 20.f ;
		}
		// 620보다 더 낙하하면 더이상 추락하지 않도록 막음
		if (users[index].Send.iYpos > 620) {
			users[index].fJumpTime = 0;
			users[index].velocity.y = 0;
			users[index].Send.iYpos = 620;
		}

		if(index==TotalClient-1) SetEvent(hWriteEvent);

		SendData.player[index] = users[index].Send;
		wcscpy(SendData.player[index].wID, users[index].Send.wID);

		for (int i = 0; i < MONSTERNUM; i++)
			SendData.monsters[i] = cmonsters[i].send;
		for (int i = 0; i < COINNUM; i++)
			SendData.coins[i] = coins[i].send;
		retval = send(client_sock, (char*)&SendData, sizeof(SendData), 0);


		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}

		Sleep(16);

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

		// W 캐릭터가 모두 선택되었는지 파악
		if (recvData->uCharNum == 1)
		{
			bFirstSelected = TRUE;
		}
		if (recvData->uCharNum == 2)
		{
			bSecondSelected = TRUE;
		}
		if (recvData->uCharNum == 3)
		{
			bThirdSelected = TRUE;
		}
		

		 users[index].Send.charNum = recvData->uCharNum - 1;
		 users[index].SetId(recvData->wId);
		 wcscpy(users[index].Send.wID, recvData->wId);

		 


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


		 //printf("\n접속한 Player의 ID: %ws", users[index].Send.wID);
		 //printf("\n접속한 Player의 캐릭터: %d\n", recvData->uCharNum);
		 //printf("\n접속한 Player의 인덱스: %d\n", index);


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

	//printf("[%d] 번 스프라이트 업데이트불림\n", p->GetCharNum());
}

void ChangeCoinSprite(int* count) {
	if (*count == 2) {
		for (int i = 0; i < COINNUM; i++) {
			coins[i].send.uSpriteX = (coins[i].send.uSpriteX + 1) % 24;
			*count = 0;
		}
	}
	*count += 1;
}


void ChangeMonsterSprite(int* count)
{
	if (*count == 5) {
		for (int i = 0; i < MONSTERNUM; i++) {
			cmonsters[i].send.uSpriteX = (cmonsters[i].send.uSpriteX + 1) % 8;
			*count = 0;
			if (cmonsters[i].send.isDeath == TRUE) {
				if (cmonsters[i].send.uSpriteX >= 6) {
					cmonsters[i].send.iXpos = 4000;
					cmonsters[i].send.iYpos = 4000;
					cmonsters[i].send.aabb = { 4000, 4000, 4000, 4000 };
				}
			}
		}
	}
	*count += 1;
}


void UpdateMonsters()
{
	static int i{ 0 };

	for (int i{ 0 }; i < MONSTERNUM; ++i) {
		if (cmonsters[i].send.isDeath != TRUE) {
			cmonsters[i].UpdateMonsterLocation(&SendData.monsters[i]);
		}
	}
}


//여기다
void UpdatePlayerLocation(Player* p)
{
	p->UpdatePlayerLocation();
	p->Jump(p->curSpriteCnt);
}


void WhosWinner() {
	int num[3] = { users[0].Send.uScore, users[1].Send.uScore, users[2].Send.uScore};
	int i, j;
	int minIndex;
	for (i = 0; i < 3 - 1; i++) {
		minIndex = i;
		for (j = i + 1; j < 3; j++)
			if (num[j] < num[minIndex])
				minIndex = j;

		int temp = num[i];
		num[i] = num[minIndex];
		num[minIndex] = temp;
	}
	printf("1: %d, 2: %d, 3: %d\n", num[0], num[1], num[2]);
	for (int i = 0; i < 3; i++) {
		if (num[0] == users[i].Send.uScore)
			users[i].Send.uRank = 3;
		if (num[1] == users[i].Send.uScore)
			users[i].Send.uRank = 2;
		if (num[2] == users[i].Send.uScore)
			users[i].Send.uRank = 1;
	}

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
	hWriteEvent = CreateEvent(NULL, FALSE, TRUE, NULL);


	hThread[2] = CreateThread(NULL, 0, Update_Thread,NULL, 0, NULL);


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




		for (int i{ 0 }; i < PLATFORMNUM; ++i) {
			retval = send(client_sock, (char*)&platform[i].send.iXpos, sizeof(int)*2, 0);

		}



		// 스레드 생성
		hThread[0] = CreateThread(NULL, 0, Recv_Thread, (LPVOID)client_sock, 0, NULL);
		hThread[1] = CreateThread(NULL, 0, Send_Thread, (LPVOID)client_sock, 0, NULL);



		

		if (hThread == NULL) { closesocket(client_sock); }
		else CloseHandle(hThread);
	}

	// 이벤트 제거
	CloseHandle(hWriteEvent);

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;

}




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



// �̺�Ʈ ����
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
	//����������� - 1280, 800
	//�ٴ� = 800-336

	//**�÷����߰��Ϸ��� ����,Ŭ�� #define ��� ��ġ��

	int height = 120;
	int width = 250;

	int floor = 800;

	
	//1�� 
	platform[0] = Platform(0, floor - height * 2);
	platform[1] = Platform(0, floor - height*2);
	platform[2] = Platform(width*3, floor - height*2);
	platform[3] = Platform(width * 5, floor - height * 2);

	//2��
	platform[4] = Platform(width, floor - height*3);
	platform[5] = Platform(width*2, floor - height*3);
	platform[6] = Platform(width*4, floor - height*3);

	//3��
	platform[7] = Platform(-100, floor - height*4);
	platform[8] = Platform(width*5.5, floor - height*4);

	//4��
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
	// semin, ���� ���� -> �迭 �ٲٸ鼭 send ����� �ٲ�. 
	// ���⼭ recv �ϴ� �� �ƴ϶� send_thread ���� ó�� init �����͵� ����.
	// ������ ���ʹ� �̵��ϸ鼭 ��� ������ ���̱� ������ send_thread �� 
	// ��ġ�� �� ���� ���̶� �Ǵ�

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

		printf("InitPlayerȣ��� %d %f\n", 5, p->GetRunSpeed() * 1);
	}
	else if (num == 1) {
		p->Send.uHeart = 4;
		p->SetRunSpeed(p->GetRunSpeed()*1.25);

		printf("InitPlayerȣ��� %d %f\n", 5, p->GetRunSpeed() * 1.25);

	}
	else if (num == 2) {
		p->Send.uHeart = 3;
		p->SetRunSpeed(p->GetRunSpeed() * 1.5);
		printf("InitPlayerȣ��� %d %f\n", 3, p->GetRunSpeed() * 1.5);


	}
}


int	MonsterSpriteCnt = 0;	// ��������Ʈ ī��Ʈ ����
int CoinSpriteCnt = 0;

DWORD WINAPI Update_Thread(LPVOID arg)
{
	clock_t start = clock(), pre = clock();
	double time = 0;
	// Ŭ���̾�Ʈ�� ������ ���
	int cnt = 0;

	while (1) {

		WaitForSingleObject(hWriteEvent, INFINITE);

		if (cnt == 0 && TotalClient == 3) {
			start = clock();
			cnt++;
		}

		if (CoinCollide.iscrush) {
			users[CoinCollide.index].Send.uScore += 1;
			SendData.player[CoinCollide.index].uScore= users[CoinCollide.index].Send.uScore;//���⼭ �������� ������Ʈ �ؾ��ҵ�
			printf("[%d]�������浹,�������� [%d]\n",CoinCollide.index, SendData.player[CoinCollide.index].uScore);

			CoinCollide.iscrush = false;
			ResetEvent(hWriteEvent);

		}

		if (MonCollide.iscrush) {
			users[MonCollide.index].Send.uScore += 2;
			SendData.player[MonCollide.index].uScore = users[MonCollide.index].Send.uScore; //���⼭ �������� ������Ʈ �ؾ��ҵ�

			printf("%d�������浹��\n", MonCollide.crushnum);
			//���⼭ �����浹���ϴ°� ������Ʈ �ϸ�ɵ�
			MonCollide.iscrush = false;

			ResetEvent(hWriteEvent);
		}



		// ���� ��������Ʈ ������Ʈ ( �̵��� ���⼭ �ϸ� �� �� )
		//printf("UpdateThread�Ҹ�\n");
		ChangeMonsterSprite(&MonsterSpriteCnt);
		ChangeCoinSprite(&CoinSpriteCnt);
		UpdateMonsters();


		// W, ���� ���� ����
		if (bFirstSelected && bSecondSelected && bThirdSelected) {
			SendData.bIsPlaying = TRUE;
		}
		// semin, ���� �ð�
		if (TotalClient == 3) {	// ������ ������ ����� thread���� �����
			pre = clock();
			time = (pre - start);
			//printf("%f �� \n", time / CLOCKS_PER_SEC);
			if ((double)(time) / CLOCKS_PER_SEC >= 120 ) {	// 120��(2��) ������ ���� ��
				//printf("���� ������\n");
				WhosWinner();
				SendData.bGameEnd = TRUE;
			}
			SendData.ServerTime = time;	// time / CLOCKS_PER_SEC �ϸ� �� ������ ���´�
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



	int	playerSpriteCnt = 0;	// ��������Ʈ ī��Ʈ ����

	// Ŭ���̾�Ʈ�� ������ ���

	while (1) {
		//printf("[%d]��° sendThread\n", index);

		if (users[index].GetCharNum() == 10000) break;
		UpdatePlayerLocation(&(users[index]));

		ChangePlayerSprite(&(users[index]), &playerSpriteCnt);

		if (users[index].Send.uHeart == 0) {
			users[index].Send.uHeart = 4;
			//InitPlayer(users[index].Send.charNum, &users[index]);
		}

		// ���� �浹
		for (int i = 0; i < MONSTERNUM; i++) {
			if (0 != users[index].IsCollidedMonster(&cmonsters[i])) {
				MonCollide.iscrush = true;
				MonCollide.crushnum = i;
				MonCollide.index = index;

				SetEvent(hWriteEvent);
			}
		}
		// ���� �浹
		for (int i = 0; i < COINNUM; i++) {
			if (coins[i].send.bIsCrush == FALSE&&users[index].IsCollidedCoin(&coins[i])) {
				CoinCollide.iscrush = true;
				CoinCollide.crushnum = i;
				CoinCollide.index = index;

				SetEvent(hWriteEvent);
			}

		}
		// �÷��� �浹
		for (int i = 0; i < PLATFORMNUM; i++) {
			users[index].IsCollidedPlatform(platform[i]);
			users[index].killMonster = FALSE;
		}
		// ���� �÷��� �浹�� �ƴ� �� ����
		bool check = false;
		for (int i = 0; i < PLATFORMNUM; i++) {
			if (users[index].IsNotCollidedPlatform(platform[i])) {
				check = true;
			}
		}
		if (users[index].killMonster == FALSE && users[index].bJumpKeyPressed == FALSE && check == false && !(users[index].Send.iYpos > 610)) {
			users[index].velocity.y = 20.f ;
		}
		// 620���� �� �����ϸ� ���̻� �߶����� �ʵ��� ����
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
	// ���� �ݱ�
	//closesocket(client_sock);

	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
	return 0;

}

DWORD WINAPI Recv_Thread(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	int option = TRUE;               //���̱� �˰��� on/off
	setsockopt(client_sock,             //�ش� ����
		IPPROTO_TCP,          //������ ����
		TCP_NODELAY,          //���� �ɼ�
		(const char*)&option, // �ɼ� ������
		sizeof(option));      //�ɼ� ũ��
	struct sockaddr_in clientaddr;
	int addrlen;
	char addr[INET_ADDRSTRLEN];
	char buf[BUFSIZE + 1]; // ���� ���� ������
	
	ClientToServer* recvData;

	const int index = TotalClient - 1;
	
	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// Ŭ���̾�Ʈ�� ������ ���
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

		// W ĳ���Ͱ� ��� ���õǾ����� �ľ�
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


		 //printf("\n������ Player�� ID: %ws", users[index].Send.wID);
		 //printf("\n������ Player�� ĳ����: %d\n", recvData->uCharNum);
		 //printf("\n������ Player�� �ε���: %d\n", index);


	}
	printf("\n#No.%d '%ws' Recv_Thread COMPLATE\n", client_sock, recvData->wId);
	// ���� �ݱ�
	//closesocket(client_sock);

	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
	--TotalClient;
	return 0;
}

void ChangePlayerSprite(Player* p, int* count)
{
	// count �� timer �� �ð��� ���� 1 �� �ö�
	// ���� ���ǹ��� count == "����" ���� �ø����� ��������Ʈ�� õõ�� ���ư� ��
	if (*count == 3) {
		p->uSpriteX = (p->uSpriteX + 1) % 4;
		*count = 0;
	}
	*count += 1;

	//printf("[%d] �� ��������Ʈ ������Ʈ�Ҹ�\n", p->GetCharNum());
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


//�����
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

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;

	HANDLE hThread[3];

	// �̺�Ʈ ����
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

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));


		printf("\n============================================================================\n");
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n\n", addr, ntohs(clientaddr.sin_port));
		++TotalClient;

		printf("�� ���� Ŭ���̾�Ʈ : %d\n", TotalClient);
		printf("\n============================================================================\n");




		for (int i{ 0 }; i < PLATFORMNUM; ++i) {
			retval = send(client_sock, (char*)&platform[i].send.iXpos, sizeof(int)*2, 0);

		}



		// ������ ����
		hThread[0] = CreateThread(NULL, 0, Recv_Thread, (LPVOID)client_sock, 0, NULL);
		hThread[1] = CreateThread(NULL, 0, Send_Thread, (LPVOID)client_sock, 0, NULL);



		

		if (hThread == NULL) { closesocket(client_sock); }
		else CloseHandle(hThread);
	}

	// �̺�Ʈ ����
	CloseHandle(hWriteEvent);

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;

}




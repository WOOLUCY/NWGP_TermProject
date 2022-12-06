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

#define PLATFORMNUM 10
#define COINNUM 10
#define MONSTERNUM 10


// �̺�Ʈ ����
HANDLE hWriteEvent;

int TotalClient;

bool				bFirstSelected = FALSE;
bool				bSecondSelected = FALSE;
bool				bThirdSelected = FALSE;


Player				users[3];

vector<Platform>	platform;

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


void InitPlatform()
{
	//�ϴ� 100,200,300,400, �̷��� �����ְ���

	for (int i = 0; i < PLATFORMNUM; ++i) {
		platform.push_back(Platform(i+10, i * 50));
	}


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
		cmonsters[i].send.iXpos = i * 120 - 200;
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
	if (TotalClient == 3)
		clock_t start = clock();


	// Ŭ���̾�Ʈ�� ������ ���

	while (1) {




		WaitForSingleObject(hWriteEvent, INFINITE);

		if (CoinCollide.iscrush) {
			printf("%d�������浹\n",CoinCollide.crushnum);
			SendData.player[CoinCollide.index].uScore += 100;//���⼭ �������� ������Ʈ �ؾ��ҵ�
			CoinCollide.iscrush = false;
			ResetEvent(hWriteEvent);

		}

		if (MonCollide.iscrush) {
			printf("%d�������浹��\n", MonCollide.crushnum);
			//���⼭ �����浹���ϴ°� ������Ʈ �ϸ�ɵ�
			MonCollide.iscrush = false;
			//printf("���⼭ ���ߴ°Ŵ�??1��\n");

			ResetEvent(hWriteEvent);
			//printf("���⼭ ���ߴ°Ŵ�??2��\n");
		}



		// ���� ��������Ʈ ������Ʈ ( �̵��� ���⼭ �ϸ� �� �� )
		printf("UpdateThread�Ҹ�\n");
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
				SendData.bIsPlaying = FALSE;
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
		printf("[%d]��° sendThread\n", index);
		if (users[index].GetCharNum() == 10000) break;;
		UpdatePlayerLocation(&(users[index]));
		ChangePlayerSprite(&(users[index]), &playerSpriteCnt);

		// ���� �浹
		for (int i = 0; i < MONSTERNUM; i++) {


			if (0 != users[index].IsCollidedMonster(cmonsters[i])) {

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

		if(index==TotalClient-1) SetEvent(hWriteEvent);

		SendData.player[index] = users[index].Send;
		wcscpy(SendData.player[index].wID, users[index].Send.wID);
		for (int i = 0; i < MONSTERNUM; i++) {
			SendData.monsters[i] = cmonsters[i].send;
		}
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

		 if (users[index].Send.uHeart == 0) {
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


		 printf("\n������ Player�� ID: %ws", users[index].Send.wID);
		 printf("\n������ Player�� ĳ����: %d\n", recvData->uCharNum);
		 printf("\n������ Player�� �ε���: %d\n", index);


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

	for (int i{ 0 }; i < MONSTERNUM; ++i) {
		cmonsters[i].UpdateMonsterLocation(&SendData.monsters[i]);
	}
}

void UpdatePlayerLocation(Player* p)
{
	p->UpdatePlayerLocation();
	p->Jump(p->curSpriteCnt);
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




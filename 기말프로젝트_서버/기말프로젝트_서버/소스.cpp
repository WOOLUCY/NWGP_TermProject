#include "Common.h"
#include "global.h"
#include "SendRecvData.h"
#include <vector>
#include <algorithm>
#include <locale.h>
#include "Platform.h"
#include "Coin.h"
#include "Key.h"
#include "Portal.h"

#define SERVERPORT 9000
#define BUFSIZE    128



// �̺�Ʈ ����
HANDLE hWriteEvent;

int TotalClient;

bool				bFirstSelected = FALSE;
bool				bSecondSelected = FALSE;
bool				bThirdSelected = FALSE;


Player				users[3];

Platform			platform[PLATFORMNUM];
Key					key;
Portal				portal;

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

void InitKey()
{
	// Ű �⺻ ��ġ
	key = Key(300, 300);
}

void InitPortal()
{
	portal = Portal(1500, 400);
}


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
	platform[2] = Platform(width*3+100, floor - height*2);
	platform[3] = Platform(width * 5+150, floor - height * 2);

	//2��
	platform[4] = Platform(width+50, floor - height*3);
	platform[5] = Platform(width*2+50, floor - height*3);
	platform[6] = Platform(width*4+100, floor - height*3);

	//3��
	platform[7] = Platform(-100, floor - height*4);
	platform[8] = Platform(width*5.5+100, floor - height*4);

	//4��
	platform[9] = Platform(600, floor - height*5);
	platform[1] = Platform(width * 4,  floor - height * 5);

	//platform[10] = (Platform(700, floor - height*5));


}

void InitCoin()
{

	coins[0].iXpos = platform[0].send.iXpos;
	coins[0].iYpos = platform[0].send.iYpos - 30;

	coins[1].iXpos = platform[1].send.iXpos;
	coins[1].iYpos = platform[1].send.iYpos - 30;

	coins[2].iXpos = platform[2].send.iXpos;
	coins[2].iYpos = platform[2].send.iYpos - 30;

	coins[3].iXpos = platform[3].send.iXpos;
	coins[3].iYpos = platform[3].send.iYpos - 30;

	coins[4].iXpos = platform[4].send.iXpos;
	coins[4].iYpos = platform[4].send.iYpos - 30;

	coins[5].iXpos = platform[5].send.iXpos;
	coins[5].iYpos = platform[5].send.iYpos - 30;

	coins[6].iXpos = platform[6].send.iXpos;
	coins[6].iYpos = platform[6].send.iYpos - 30;
						   
	coins[7].iXpos = platform[7].send.iXpos;
	coins[7].iYpos = platform[7].send.iYpos - 30;

	coins[8].iXpos = platform[8].send.iXpos;
	coins[8].iYpos = platform[8].send.iYpos - 30;

	coins[9].iXpos = platform[9].send.iXpos;
	coins[9].iYpos = platform[9].send.iYpos - 30;

	//
	coins[10].iXpos = platform[0].send.iXpos + 40;
	coins[10].iYpos = platform[0].send.iYpos - 30;
	coins[11].iXpos = platform[1].send.iXpos + 40;
	coins[11].iYpos = platform[1].send.iYpos - 30;
	coins[12].iXpos = platform[2].send.iXpos + 40;
	coins[12].iYpos = platform[2].send.iYpos - 30;
	coins[13].iXpos = platform[3].send.iXpos + 40;
	coins[13].iYpos = platform[3].send.iYpos - 30;
	coins[14].iXpos = platform[4].send.iXpos + 40;
	coins[14].iYpos = platform[4].send.iYpos - 30;
	coins[15].iXpos = platform[5].send.iXpos + 40;
	coins[15].iYpos = platform[5].send.iYpos - 30;
	coins[16].iXpos = platform[6].send.iXpos + 40;
	coins[16].iYpos = platform[6].send.iYpos - 30;
	coins[17].iXpos = platform[7].send.iXpos + 40;
	coins[17].iYpos = platform[7].send.iYpos - 30;
	coins[18].iXpos = platform[8].send.iXpos + 40;
	coins[18].iYpos = platform[8].send.iYpos - 30;
	coins[19].iXpos = platform[9].send.iXpos + 40;
	coins[19].iYpos = platform[9].send.iYpos - 30;


	coins[20].iXpos = platform[0].send.iXpos + 40 * 2;
	coins[20].iYpos = platform[0].send.iYpos - 30;

	coins[21].iXpos = platform[1].send.iXpos + 40 * 2;
	coins[21].iYpos = platform[1].send.iYpos - 30;

	coins[22].iXpos = platform[2].send.iXpos + 40 * 2;
	coins[22].iYpos = platform[2].send.iYpos - 30;

	coins[23].iXpos = platform[3].send.iXpos + 40 * 2;
	coins[23].iYpos = platform[3].send.iYpos - 30;

	coins[24].iXpos = platform[4].send.iXpos + 40 * 2;
	coins[24].iYpos = platform[4].send.iYpos - 30;

	coins[25].iXpos = platform[5].send.iXpos + 40 * 2;
	coins[25].iYpos = platform[5].send.iYpos - 30;

	coins[26].iXpos = platform[6].send.iXpos + 40 * 2;
	coins[26].iYpos = platform[6].send.iYpos - 30;

	coins[27].iXpos = platform[7].send.iXpos + 40 * 2;
	coins[27].iYpos = platform[7].send.iYpos - 30;

	coins[28].iXpos = platform[8].send.iXpos + 40 * 2;
	coins[28].iYpos = platform[8].send.iYpos - 30;

	coins[29].iXpos = platform[9].send.iXpos + 40 * 2;
	coins[29].iYpos = platform[9].send.iYpos - 30;
//
	coins[30].iXpos = platform[0].send.iXpos + 40*3;
	coins[30].iYpos = platform[0].send.iYpos - 30;
	coins[31].iXpos = platform[1].send.iXpos + 40*3;
	coins[31].iYpos = platform[1].send.iYpos - 30;
	coins[32].iXpos = platform[2].send.iXpos + 40*3;
	coins[32].iYpos = platform[2].send.iYpos - 30;
	coins[33].iXpos = platform[3].send.iXpos + 40*3;
	coins[33].iYpos = platform[3].send.iYpos - 30;
	coins[34].iXpos = platform[4].send.iXpos + 40*3;
	coins[34].iYpos = platform[4].send.iYpos - 30;
	coins[35].iXpos = platform[5].send.iXpos + 40*3;
	coins[35].iYpos = platform[5].send.iYpos - 30;
	coins[36].iXpos = platform[6].send.iXpos + 40*3;
	coins[36].iYpos = platform[6].send.iYpos - 30;
	coins[37].iXpos = platform[7].send.iXpos + 40*3;
	coins[37].iYpos = platform[7].send.iYpos - 30;
	coins[38].iXpos = platform[8].send.iXpos + 40*3;
	coins[38].iYpos = platform[8].send.iYpos - 30;
	coins[39].iXpos = platform[9].send.iXpos + 40*3;
	coins[39].iYpos = platform[9].send.iYpos - 30;

	//
	coins[40].iXpos = platform[0].send.iXpos + 40 * 4;
	coins[40].iYpos = platform[0].send.iYpos - 30;
	coins[41].iXpos = platform[1].send.iXpos + 40 * 4;
	coins[41].iYpos = platform[1].send.iYpos - 30;
	coins[42].iXpos = platform[2].send.iXpos + 40 * 4;
	coins[42].iYpos = platform[2].send.iYpos - 30;
	coins[43].iXpos = platform[3].send.iXpos + 40 * 4;
	coins[43].iYpos = platform[3].send.iYpos - 30;
	coins[44].iXpos = platform[4].send.iXpos + 40 * 4;
	coins[44].iYpos = platform[4].send.iYpos - 30;
	coins[45].iXpos = platform[5].send.iXpos + 40 * 4;
	coins[45].iYpos = platform[5].send.iYpos - 30;
	coins[46].iXpos = platform[6].send.iXpos + 40 * 4;
	coins[46].iYpos = platform[6].send.iYpos - 30;
	coins[47].iXpos = platform[7].send.iXpos + 40 * 4;
	coins[47].iYpos = platform[7].send.iYpos - 30;
	coins[48].iXpos = platform[8].send.iXpos + 40 * 4;
	coins[48].iYpos = platform[8].send.iYpos - 30;
	coins[49].iXpos = platform[9].send.iXpos + 40 * 4;
	coins[49].iYpos = platform[9].send.iYpos - 30;



	for (int i = 0; i < COINNUM; ++i) {
		coins[i].CoinUpdate();
	}
}
void InitMonster()
{

	cmonsters[0].send.iXpos = platform[0].send.iXpos + 40 * 4;
	cmonsters[0].send.iYpos = platform[0].send.iYpos - 50;
	cmonsters[0].updateRange(10);
	cmonsters[0].SetMonNum(10);

	cmonsters[1].send.iXpos = platform[1].send.iXpos + 40 * 4;
	cmonsters[1].send.iYpos = platform[1].send.iYpos - 50;
	cmonsters[1].updateRange(10);
	cmonsters[1].SetMonNum(5);


	cmonsters[2].send.iXpos = platform[2].send.iXpos + 40 * 4;
	cmonsters[2].send.iYpos = platform[2].send.iYpos - 50;
	cmonsters[2].updateRange(10);
	cmonsters[2].SetMonNum(3);


	cmonsters[3].send.iXpos = platform[3].send.iXpos + 40 * 4;
	cmonsters[3].send.iYpos = platform[3].send.iYpos - 50;
	cmonsters[3].updateRange(10);
	cmonsters[3].SetMonNum(7);


	cmonsters[4].send.iXpos = platform[4].send.iXpos + 40 * 4;
	cmonsters[4].send.iYpos = platform[4].send.iYpos - 50;
	cmonsters[4].updateRange(10);
	cmonsters[4].SetMonNum(4);


	cmonsters[5].send.iXpos = platform[5].send.iXpos + 40 * 4;
	cmonsters[5].send.iYpos = platform[5].send.iYpos - 50;
	cmonsters[5].updateRange(10);
	cmonsters[5].SetMonNum(5);

	cmonsters[6].send.iXpos = platform[6].send.iXpos + 40 * 4;
	cmonsters[6].send.iYpos = platform[6].send.iYpos - 50;
	cmonsters[6].updateRange(10);
	cmonsters[6].SetMonNum(6);


	cmonsters[7].send.iXpos = platform[7].send.iXpos + 40 * 4;
	cmonsters[7].send.iYpos = platform[7].send.iYpos - 50;
	cmonsters[7].updateRange(10);
	cmonsters[7].SetMonNum(7);


	cmonsters[8].send.iXpos = platform[8].send.iXpos + 40 * 4;
	cmonsters[8].send.iYpos = platform[8].send.iYpos - 50;
	cmonsters[8].updateRange(10);
	cmonsters[8].SetMonNum(8);


	cmonsters[9].send.iXpos = platform[9].send.iXpos + 40 * 4;
	cmonsters[9].send.iYpos = platform[9].send.iYpos - 50;
	cmonsters[9].updateRange(10);
	cmonsters[9].SetMonNum(9);



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

		printf("InitPlayerȣ��� %d %f\n", 5, p->GetRunSpeed() * 1.2);

	}
	else if (num == 2) {
		p->Send.uHeart = 3;
		p->SetRunSpeed(p->GetRunSpeed() * 1.5);
		printf("InitPlayerȣ��� %d %f\n", 3, p->GetRunSpeed() * 1.35);


	}
}


int	MonsterSpriteCnt = 0;	// ��������Ʈ ī��Ʈ ����
int CoinSpriteCnt = 0;

DWORD WINAPI Update_Thread(LPVOID arg)
{
	clock_t start = clock(), pre = clock();
	double time = 0;
	int cnt = 0;
	bool gameEnd = FALSE;

	while (1) {

		WaitForSingleObject(hWriteEvent, INFINITE);

		if (cnt == 0 && TotalClient == 3) {
			start = clock();
			cnt++;
		}

		if (CoinCollide.iscrush) {

			++SendData.iTotalCoinNum;
			printf("�� ���� ���� : [%d]\n",SendData.iTotalCoinNum);
			users[CoinCollide.index].Send.uScore += 1;
			SendData.player[CoinCollide.index].uScore= users[CoinCollide.index].Send.uScore;//���⼭ �������� ������Ʈ �ؾ��ҵ�

			// W ���� �� ���� ������ ���� ������ ���ü� ����
			if (SendData.iTotalCoinNum >= 5)
			{
				key.send.bIsVisible = TRUE;
			}

			CoinCollide.iscrush = false;
			ResetEvent(hWriteEvent);

		}

		if (MonCollide.iscrush) {

			users[MonCollide.index].CheckLocationCollideMonster(&cmonsters[MonCollide.crushnum]);
			printf("%d�������浹��\n", MonCollide.crushnum);
			MonCollide.iscrush = false;

			ResetEvent(hWriteEvent);
		}

		ChangeMonsterSprite(&MonsterSpriteCnt);
		ChangeCoinSprite(&CoinSpriteCnt);
		UpdateMonsters();


		// W, ���� ���� ����
		if (bFirstSelected && bSecondSelected && bThirdSelected) {
			SendData.bIsPlaying = TRUE;
		}
		// W, ��Ż �浹 ���� ����
		if (portal.bIsCrush && !gameEnd)
		{
			WhosWinner();
			gameEnd = TRUE;
			SendData.bGameEnd = TRUE;
		}
		// semin, ���� �ð�
		if (TotalClient == 3) {	// ������ ������ ����� thread���� �����
			pre = clock();
			time = (pre - start);
			//printf("%f �� \n", time / CLOCKS_PER_SEC);
			if ((double)(time) / CLOCKS_PER_SEC >= 120 && !gameEnd) {	// 120��(2��) ������ ���� ��
				//printf("���� ������\n");
				WhosWinner();
				gameEnd = TRUE;
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

		// ���� �浹
		for (int i = 0; i < MONSTERNUM; i++) {
			if (cmonsters[i].send.isDeath == FALSE && 0 != users[index].IsCollidedMonster(&cmonsters[i])) {
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

		// key �浹
		//SendData.key.bIsVisible = key.send.bIsVisible;
		if (key.send.bIsVisible)
		{
			if (users[index].IsCollidedKey(key) && !key.bIsCrush)
			{
				key.bIsCrush = TRUE;
				SendData.key.bIsCrush = TRUE;
				users[index].bHasKey = TRUE;
				users[index].Send.bHasKey = TRUE;
				users[index].Send.uScore += 5;
				printf("%d�� �÷��̾ ���踦 ����\n", users[index].Send.charNum);
			}
		}
		SendData.key = key.send;

		// portal �浹
		if (users[index].bHasKey == TRUE)
		{
			if (users[index].IsCollidedPortal(portal))
			{
				portal.bIsCrush = TRUE;
			}
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
	bool first = true;
	
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
		
		if ( first && users[index].Send.uHeart == 0  ) {

			InitPlayer(users[index].Send.charNum, &users[index]);

		}
		else {
			first = false;
		}

		
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


		}
	printf("\n#No.%d '%ws' Recv_Thread COMPLATE\n", client_sock, recvData->wId);
	// ���� �ݱ�

	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
	--TotalClient;
	closesocket(client_sock);
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
	static int deathCnt = 0;
	static bool death = 0;
	if (*count == 5) {
		for (int i = 0; i < MONSTERNUM; i++) {
			cmonsters[i].send.uSpriteX = (cmonsters[i].send.uSpriteX + 1) % 8;
			if (cmonsters[i].send.isDeath == TRUE) {
				death = TRUE;
				if (cmonsters[i].send.uSpriteX >= 7) {
					cmonsters[i].send.iXpos = 4000;
					cmonsters[i].send.iYpos = 4000;
					cmonsters[i].send.isDeath = FALSE;
					deathCnt = 0;
				}
			}
			*count = 0;
		}
	}
	*count += 1;
	if (death == TRUE) {
		deathCnt++;
	}
}

void changeMonsterDeathSprite(CMonster* cmonster) {
	static int deathCnt = 0;
	if (deathCnt == 5) {
		cmonster->send.uSpriteX = (cmonster->send.uSpriteX + 1) % 8;
		if (cmonster->send.uSpriteX >= 6) {
			cmonster->send.iXpos = 4000;
			cmonster->send.iYpos = 4000;
			deathCnt = 0;
		}
	}
	deathCnt++;
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
	InitKey();
	InitPortal();

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

		for (int i{ 0 }; i < COINNUM; ++i) {
			retval = send(client_sock, (char*)&coins[i].iXpos, sizeof(int) * 2, 0);
		}


		retval = send(client_sock, (char*)&key.send.iXpos, sizeof(int) * 2, 0);

		retval = send(client_sock, (char*)&portal.send.iXpos, sizeof(int) * 2, 0);



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




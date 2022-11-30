#include "Common.h"
#include "SendRecvData.h"
#include <vector>
#include "Platform.h"
#include "Coin.h"

#define SERVERPORT 9000
#define BUFSIZE    512

#define PLATFORMNUM 10
#define COINNUM 10
#define MONSTERNUM 10


HANDLE hWriteEvent;
HANDLE hReadEvent;

int TotalClient;


Player users[3];

vector<Platform> platform;
vector<Coin> coins;
vector<CMonster> monsters;

ServerToClient SendData;


void UpdatePlayerLocation(Player* p);

void InitPlatform()
{
	//�ϴ� 100,200,300,400, �̷��� �����ְ���

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
	for (int i{ 0 }; i < MONSTERNUM; ++i) {
		monsters.push_back(CMonster(i * i, i));
	}
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

	// Ŭ���̾�Ʈ�� ������ ���

	while (1) {

		if (users[index].GetCharNum() == 10000) break;;
		UpdatePlayerLocation(&(users[index]));

		Sleep(16);




		SendData.player[index] = users[index].Send;
		
		retval = send(client_sock, (char*)&SendData, sizeof(SendData), 0);


		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}


	}

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

		 users[index].Send.charNum = recvData->uCharNum-1;
		 users[index].SetId(recvData->wId);

		 if (users[index].bJumpKeyPressed == true) {
			 users[index].input.bLeft = recvData->Input.bLeft;
			 users[index].input.bRight = recvData->Input.bRight;


		 }
		 else {
			 users[index].input = recvData->Input;

		 }


		 printf("\n������ Player�� ID: %ws", recvData->wId);
		 printf("\n������ Player�� ĳ����: %d\n", recvData->uCharNum);
		 printf("\n������ Player�� �ε���: %d\n", index);

		 printf("bLeft: %s\n", recvData->Input.bLeft ? "true" : "false");
		 printf("bRight: %s\n", recvData->Input.bRight ? "true" : "false");
		 printf("bSpace: %s\n", recvData->Input.bSpace ? "true" : "false");


		 printf("bLeft: %s\n", users[index].input.bLeft ? "true" : "false");
		 printf("bRight: %s\n", users[index].input.bRight ? "true" : "false");
		 printf("bSpace: %s\n", users[index].input.bSpace ? "true" : "false");

		

	}
	printf("\n#No.%d '%ws' Recv_Thread COMPLATE\n", client_sock, recvData->wId);
	// ���� �ݱ�
	//closesocket(client_sock);

	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
	--TotalClient;
	return 0;
}

void UpdatePlayerLocation(Player* p)
{
	static int curSpriteCnt = 0;


	if (!(p->input.bLeft)) {
		//�������� �̵�
		p->velocity.x = 0;
		p->SetSpriteY(4);



	}

	if (!(p->input.bRight)) {
		//�������� �̵�
		p->velocity.x = 0;
		p->SetSpriteY(0);

	}


	if (p->input.bLeft) {
		//�������� �̵�
		curSpriteCnt = 3;

		p->velocity.x = -p->GetRunSpeed();
		p->SetSpriteY(3);

	}


	if (p->input.bRight) {
		//���������� �̵�
		curSpriteCnt = 1;

		p->velocity.x = p->GetRunSpeed();
		p->SetSpriteY(1);
	}






	if (p->input.bSpace) {
		//����
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

	HANDLE hThread[2];

	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

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



		//���� - �ѹ������� �Ⱥ��� ������ ���⼭ ������������� ����
		//�ϴ� ���� ���� ����- ��������

		int tmp = PLATFORMNUM;
		retval = send(client_sock, (char*)&tmp, sizeof(int), 0);
		printf("%d\n", tmp);

		for (int i{ 0 }; i < tmp; ++i) {
			retval = send(client_sock, (char*)&platform[i].send.iXpos, sizeof(int)*2, 0);

		}

		tmp = COINNUM;
		retval = send(client_sock, (char*)&tmp, sizeof(int), 0);
		printf("%d\n", tmp);

		for (int i{ 0 }; i < tmp; ++i) {
			retval = send(client_sock, (char*)&coins[i].send.iXpos, sizeof(int) * 2, 0);

		}


		tmp = MONSTERNUM;
		retval = send(client_sock, (char*)&tmp, sizeof(int), 0);

		for (int i{ 0 }; i < tmp; ++i) {
			retval = send(client_sock, (char*)&monsters[i].send.iXpos, sizeof(int) * 2, 0);
			printf("%d %d \t", monsters[i].send.iXpos, monsters[i].send.iYpos);

		}


		// ������ ����
		hThread[0] = CreateThread(NULL, 0, Recv_Thread, (LPVOID)client_sock, 0, NULL);
		hThread[1] = CreateThread(NULL, 0, Send_Thread, (LPVOID)client_sock, 0, NULL);



		

		if (hThread == NULL) { closesocket(client_sock); }
		else CloseHandle(hThread);
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;

}



//���� �ʱ�ȭ �Լ�

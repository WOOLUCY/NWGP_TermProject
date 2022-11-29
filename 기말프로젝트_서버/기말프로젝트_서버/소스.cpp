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


vector<Platform> platform;
vector<Coin> coins;


void InitPlatform()
{
	//�ϴ� 100,200,300,400, �̷��� �����ְ���

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

	char buf[BUFSIZE + 1]; // ���� ���� ������
	int len;

	RecvPlayerData* player;

	char testData[BUFSIZE + 1] = "\0";

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// Ŭ���̾�Ʈ�� ������ ���
	while (1) {
		Sleep(16);
		// printf("���� ");
	}

	printf("\n#No.%d '%s' SENDING COMPLATE\n", client_sock, testData);
	// ���� �ݱ�
	closesocket(client_sock);

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
	
	RecvPlayerData *player;
	

	char testData[BUFSIZE + 1] = "\0";
	
	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// Ŭ���̾�Ʈ�� ������ ���
	while (1) {
		// ID recv
		retval = recv(client_sock, buf, sizeof(RecvPlayerData), MSG_WAITALL);
		buf[retval] = '\0';

		player = (RecvPlayerData*)buf;
		sprintf(testData, buf);

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) break;

		 printf("\n������ Player�� ID: %ws", player->wId);
		 printf("\n������ Player�� ĳ����: %d\n", player->uCharNum);

		 printf("bLeft: %s\n", player->Input.bLeft ? "true" : "false");
		 printf("bRight: %s\n", player->Input.bRight ? "true" : "false");
		 printf("bSpace: %s\n", player->Input.bSpace ? "true" : "false");

		 printf("Collided Monster Number: %d\n", player->pPlayer.GetXPos());
	}
	printf("\n#No.%d '%ws' SENDING COMPLATE\n", client_sock, player->wId);
	// ���� �ݱ�
	closesocket(client_sock);

	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
	return 0;
}

int main(int argc, char* argv[])
{
	// �̺�Ʈ ����
	// hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// ������ �� �� ����
	//HANDLE hThread[3];
	//hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	//hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	//hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

	//// �б� �Ϸ� �˸�
	//SetEvent(hReadEvent);

	//// ������ �� �� ���� ���
	//WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	//// �̺�Ʈ ����
	//CloseHandle(hWriteEvent);
	//CloseHandle(hReadEvent);
	//return 0;

	InitPlatform();
	InitCoin();

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

		//���� - �ѹ������� �Ⱥ��� ������ ���⼭ ������������� ����
		//�ϴ� ���� ���� ����- ��������
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


		// ������ ����
		hThread[0] = CreateThread(NULL, 0, Recv_Thread, (LPVOID)client_sock, 0, NULL);
		//hThread[1] = CreateThread(NULL, 0, Send_Thread, (LPVOID)client_sock, 0, NULL);




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

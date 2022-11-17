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
		// �б� �Ϸ� ���
		retval = WaitForSingleObject(hReadEvent, INFINITE);
		if (retval != WAIT_OBJECT_0) break;

		// ���� ���ۿ� ������ ����
		for (int i = 0; i < BUFSIZE; i++)
			buf[i] = k;

		// ���� �Ϸ� �˸�
		SetEvent(hWriteEvent);
	}
	return 0;
}

DWORD WINAPI Recv_Thread(LPVOID arg)
{
//	�Ʒ� �ּ� ó�� �ڵ�� å�� �̺�Ʈ ������
//	DWORD retval;
//	while (1) {
//		// ���� �Ϸ� ���
//		retval = WaitForSingleObject(hWriteEvent, INFINITE);
//		if (retval != WAIT_OBJECT_0) break;
//
//		// ���� ������ ��� �� ���۸� 0���� �ʱ�ȭ
//		printf("Thread %4d:\t", GetCurrentThreadId());
//		for (int i = 0; i < BUFSIZE; i++)
//			printf("%3d ", buf[i]);
//		printf("\n");
//		memset(buf, 0, sizeof(buf));
//
//		// �б� �Ϸ� �˸�
//		SetEvent(hReadEvent);
//	}
//	return 0;

	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	int addrlen;
	char addr[INET_ADDRSTRLEN];
	char buf[BUFSIZE + 1]; // ���� ���� ������
	int len;

	RecvPlayerData *player;

	char testData[BUFSIZE + 1] = "\0";
	
	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// Ŭ���̾�Ʈ�� ������ ���
	while (1) {
		// ���� ����
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

		printf("������ Player�� ID: %s", testData);


	}
	printf("\n#No.%d '%s' SENDING COMPLATE\n", client_sock, testData);
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
	HANDLE hThread;


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

		// ������ ����
		hThread = CreateThread(NULL, 0, Recv_Thread,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else CloseHandle(hThread);
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;

}

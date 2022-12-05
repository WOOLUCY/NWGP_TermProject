#pragma comment(lib, "winmm")
#include "Common.h"
#include "global.h"

#include <atlImage.h>
#include <mmsystem.h>
#include <algorithm>
#include <locale.h>

#include "Player.h"
#include "CMonster.h"
#include "Background.h"
#include "Platform.h"
#include "Collision.h"
#include "Portal.h"
#include "Key.h"
#include "Button.h"
#include "SendRecvData.h"

#include "Coin.h"
#include <time.h>

#define MONSTERNUM 10

using namespace std;
//[이세민] [오후 8:34] 192.168.219.31
char* SERVERIP = (char*)"127.0.0.1";
//char* SERVERIP = (char*)"192.168.100.116";
//char* SERVERIP = (char*)"192.168.141.42";

#define SERVERPORT 9000
#define BUFSIZE    512

#define	CHILD_BUTTON	111		// 컨트롤박스용
#define	CHILD_ID_EDIT	112


bool IsDebugMode = false;
void UpdatePlayerInput(WPARAM Input, Player player);

LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"쿠키런 이스케이프";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
char* ConvertWCtoC(wchar_t* str);
char* UTF8ToANSI(char* pszCode);

HINSTANCE hInst;	// 인스턴스 핸들
HWND hEdit;			// 에디트 컨트롤
HWND hButtonEdit;	// 에디트 컨트롤



int retval;
SOCKET sock;		// 소켓


vector<Platform> Platforms;
vector<Coin> Coins;
//vector<CMonster> Monsters;

CMonster Monsters[MONSTERNUM];
int monsterTotal = 1;	// semin, 몬스터 몇 마리인지 

Player player;

ClientToServer PlayerData;
ServerToClient GameData;


CImage platformImg;
CImage coinImg;
CImage monsterImg;
CImage playersImag[3];

void LoadImg()
{
	
	platformImg.Load(L"Image/platform3.png");
	coinImg.Load(L"Image/coin2.png");
	monsterImg.Load(L"Image/Monster2.png");

	playersImag[0].Load(L"Image/Cookies3-2.png");	
	playersImag[1].Load(L"Image/Cookies2-2.png");
	playersImag[2].Load(L"Image/Cookies4-1.png");



}

DWORD WINAPI ClientMain(LPVOID arg)
{


	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	int option = FALSE;               //네이글 알고리즘 on/off
	setsockopt(sock,             //해당 소켓
		IPPROTO_TCP,          //소켓의 레벨
		TCP_NODELAY,          //설정 옵션
		(const char*)&option, // 옵션 포인터
		sizeof(option));      //옵션 크기

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");



	//가온 - 플랫폼 데이터 일단 받기 - 테스트용 int 한개만 받고 잘 오는지확인용

	int temp[2];


	for (int i{ 0 }; i < PLATFORMNUM; ++i) {
		retval = recv(sock, (char*)temp, sizeof(int) * 2, 0);
		Platforms.push_back(Platform(temp[0], temp[1], &platformImg));
	}

	for (int i{ 0 }; i < COINNUM; ++i) {
		retval = recv(sock, (char*)temp, sizeof(int) * 2, 0);
		Coins.push_back(Coin(temp[0], temp[1], &coinImg));
	}

	char buf[BUFSIZE + 1];


	while (1) {
		retval = recv(sock, (char*)&GameData, sizeof(GameData), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) {
			break;
		}

	}

}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	hInst = hInstance;
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(
		lpszClass,
		lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		1280,
		800,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	LoadImg();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);


	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, mem1dc;
	PAINTSTRUCT ps;

	static int MouseX, MouseY;
	static RECT rect;
	static HBITMAP hBitmap;

	COLORREF color;

	static Background startBackground;
	static Background background;


	static CImage startbackgroundImg;
	static CImage backgroundImg;
	static CImage ground;

	// W 선택창 
	static Background selectBackground;
	static CImage selectBackgroundImg;
	selectBackground.Image = &selectBackgroundImg;

	static CImage readyImg;
	static Button readyButton;
	readyButton.myImage = &readyImg;

	startBackground.Image = &startbackgroundImg;
	background.Image = &backgroundImg;	

	static CImage playerImg;


	player.myImage = &playerImg;		

	//일단 코인한개만 그려보겠슴니다 - 가온(찾을때편하려고 이름 씀~~)
	static Coin TestCoin;

	TestCoin.myImage = &coinImg;


	// W 몬스터 생성
	static CMonster monster;
	monster.SetMonNum(1);
	monster.myImage= &monsterImg;

	// W 열쇠 생성
	static CImage KeyImg;
	static Key key;
	key.myImage = &KeyImg;

	// W 포탈 생성
	static CImage PortalImg;
	static Portal portal;
	portal.myImage = &PortalImg;

	time_t frame_time{};
	time_t current_time = time(NULL);
	time_t frame_rate;
	static int spriteCnt = 0;
	static USHORT curSpriteCnt = 0;

	// ID
	static bool enterID = FALSE;		// ID 입력 후 '게임 시작' 판단용
	static wchar_t wID[21] = { NULL };
	static bool containID = FALSE;		// ID 에 문자 하나라도 들어갔는지 검사

	// W character selection
	static bool bReady = FALSE;	// 캐릭터 선택 후 게임 시작 판단용
	// W
	static bool bIsPlaying = FALSE;	// 다른 접속자들이 모두 선택을 했는지 확인
	static bool bFirstSelected = FALSE;
	static bool bSecondSelected = FALSE;
	static bool bThirdSelected = FALSE;

	// semin, Background
	static int bgMove = 0;
	static int myCharacter = 0;


	switch (iMsg) {
	case WM_CREATE:
		setlocale(LC_ALL, "KOREAN");
		// PlaySound(L"start.wav", NULL, SND_ASYNC);	// 듣기 싫어서 사운드 막아둠 ㅎㅎ
		startbackgroundImg.Load(L"Image/ID입력창.png");
		backgroundImg.Load(L"Image/background.jpg");
		ground.Load(L"Image/ground2.png");
		playerImg.Load(L"Image/Cookies3.png");

		// W load character selection window image
		selectBackgroundImg.Load(L"Image/Select.png");
		selectBackground.setHeight(selectBackground.Image->GetWidth());
		selectBackground.SetWidth(selectBackground.Image->GetWidth());

		readyImg.Load(L"Image/Ready.png");

		// W load key image
		KeyImg.Load(L"Image/key.png");
		
		// W load portal image
		PortalImg.Load(L"Image/Portal2.png");

		startBackground.setHeight(startBackground.Image->GetWidth());
		startBackground.setHeight(startBackground.Image->GetHeight());
		background.SetWidth(background.Image->GetWidth());
		background.setHeight(background.Image->GetHeight());

		// 플레이어의 Width, Height 크기는 170, 148 로 고정이라 구하지 않음

		GetClientRect(hWnd, &rect);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		mem1dc = CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBitmap);
		SetStretchBltMode(mem1dc, COLORONCOLOR);

		// semin, 내 캐릭터에 대해서만 정보 따로 가지고 옴
		for (int i = 0; i < 3; i++) {
			if (PlayerData.uCharNum == GameData.player[i].charNum + 1) {
				bgMove = GameData.player[i].iBgMove;
				player.iXpos = GameData.player[i].iXpos;
				player.iYpos = GameData.player[i].iYpos;
				myCharacter = i;
			}
		}

		// W 캐릭터 선택 정보
		// 데이터 전송하는 시점을 창 띄우자마자로 바꿀 수 있는지
		bIsPlaying = GameData.bIsPlaying;
		for (int i = 0; i < 3; ++i)
		{
			if (GameData.player[i].charNum + 1 == 1) {
				bFirstSelected = TRUE;
			}
			if (GameData.player[i].charNum + 1 == 2) {
				bSecondSelected = TRUE;
			}
			if (GameData.player[i].charNum + 1 == 3) {
				bThirdSelected = TRUE;
			}
		}

		if (enterID == FALSE) {
			startBackground.Image->Draw(mem1dc, 0, 0, rect.right, rect.bottom, background.window_left, background.window_bottom, 1280, 800);

			hEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT,
				895, 385, 130, 30, hWnd, (HMENU)CHILD_ID_EDIT, hInst, NULL);
			hButtonEdit = CreateWindow(L"button", L"접속", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				890, 505, 80, 40, hWnd, (HMENU)CHILD_BUTTON, hInst, NULL);
		}
		// W render character selection window
		else if (enterID == TRUE && bIsPlaying == FALSE) {
			selectBackground.Image->Draw(mem1dc, 0, 0, rect.right, rect.bottom, 0, 0, 1280, 800);
			if (bFirstSelected) {
				readyButton.myImage->Draw(mem1dc, 45, 638, readyButton.GetWidth(), readyButton.GetHeight(), 0, 0, readyButton.GetWidth(), readyButton.GetHeight());
			}
			if (bSecondSelected) {
				readyButton.myImage->Draw(mem1dc, 463, 638, readyButton.GetWidth(), readyButton.GetHeight(), 0, 0, readyButton.GetWidth(), readyButton.GetHeight());
			}
			if (bThirdSelected) {
				readyButton.myImage->Draw(mem1dc, 883, 638, readyButton.GetWidth(), readyButton.GetHeight(), 0, 0, readyButton.GetWidth(), readyButton.GetHeight());
			}

		}
		else {
			background.Image->Draw(mem1dc, 0, 0, rect.right, rect.bottom, 200 + bgMove, 220, 2560, 1600);
			//ground.Draw(mem1dc, 0, 690, rect.right, rect.bottom, 0, 0, 2560, 1600);
			//player.myImage[0]->Draw(mem1dc, player.iXpos, player.iYpos, player.GetWidth() / 2, player.GetHeight() / 2, 0 + player.GetWidth() * player.GetSpriteX(), 0 + player.GetHeight() * player.GetSpriteY(), 170, 148);
			
			//playerImg.Draw
			for (int i = 0; i < 3; ++i) {
				if (GameData.player[i].charNum < 3) {
					if (i != myCharacter) {
						playersImag[GameData.player[i].charNum].TransparentBlt(mem1dc, GameData.player[i].iXpos - GameData.player[myCharacter].iBgMove / 2 + GameData.player[i].iBgMove / 2, GameData.player[i].iYpos, player.GetWidth() / 2, player.GetHeight() / 2,
							0 + player.GetWidth() * GameData.player[i].uSpriteX, 0 + player.GetHeight() * GameData.player[i].uSpriteY, 170, 148, RGB(255, 0, 255));
						//playersImag[GameData.player[i].charNum].Draw(mem1dc, GameData.player[i].iXpos - GameData.player[myCharacter].iBgMove / 2 + GameData.player[i].iBgMove / 2, GameData.player[i].iYpos, player.GetWidth() / 2, player.GetHeight() / 2,
						//	0 + player.GetWidth() * GameData.player[i].uSpriteX, 0 + player.GetHeight() * GameData.player[i].uSpriteY, 170, 148);
					}

					if (i == myCharacter) {
						/*playersImag[GameData.player[myCharacter].charNum].Draw(mem1dc, GameData.player[myCharacter].iXpos, GameData.player[myCharacter].iYpos, player.GetWidth() / 2, player.GetHeight() / 2,
							0 + player.GetWidth() * GameData.player[myCharacter].uSpriteX, 0 + player.GetHeight() * GameData.player[myCharacter].uSpriteY, 170, 148);*/
						playersImag[GameData.player[myCharacter].charNum].TransparentBlt(mem1dc, GameData.player[myCharacter].iXpos, GameData.player[myCharacter].iYpos, player.GetWidth() / 2, player.GetHeight() / 2,
							0 + player.GetWidth() * GameData.player[myCharacter].uSpriteX, 0 + player.GetHeight() * GameData.player[myCharacter].uSpriteY, 170, 148, RGB(255, 0, 255));
					}
				}
			}


			// W Monster Draw	
			
			//monster.myImage->Draw(mem1dc, monster.iXpos, monster.iYpos, monster.GetWidth() / 2, monster.GetHeight() / 2, 0 + monster.GetWidth() * monster.GetSpriteX(), 0 + monster.GetHeight() * monster.GetSpriteY(), 144, 138);		// W Draw Key
			
																																																										
																																																										// W Key Draw
			if (!player.GetHasKey())
			{
				key.myImage->Draw(mem1dc, key.iXpos - bgMove / 2, key.iYpos, key.GetWidth() / 2, key.GetHeight() / 2, 0, 0, 163, 148);
			}


			// 포탈
			//portal.myImage->Draw(mem1dc, portal.iXpos - bgMove / 2, portal.iYpos, portal.GetWidth() / 2, portal.GetHeight() / 2, 0 + portal.GetWidth() * portal.GetSpriteX(), 0 + portal.GetHeight() * portal.GetSpriteY(), 182, 206);
			portal.myImage->TransparentBlt(mem1dc, portal.iXpos - bgMove / 2, portal.iYpos, portal.GetWidth() / 1.5, portal.GetHeight() / 1.5, 0 + portal.GetWidth() * portal.GetSpriteX(), 0 + portal.GetHeight() * portal.GetSpriteY(), 182, 206, RGB(0, 0, 255));
			
			
			//가온-코인그리기 
			TestCoin.myImage->Draw(mem1dc, TestCoin.iXpos - bgMove / 2, TestCoin.iYpos, TestCoin.GetWidth() / 2, TestCoin.GetHeight() / 2, 0 + TestCoin.GetWidth() * TestCoin.GetSpriteX(), 0 + TestCoin.GetHeight() * TestCoin.GetSpriteY(), TestCoin.GetWidth(), TestCoin.GetHeight());

			//가온 - 플랫폼 - 위치 서버에서 바꿔줘야함 걍 대충 바갑가ㅏㅏ함
			for (Platform& temp : Platforms) {
				temp.myImage->Draw(mem1dc, temp.iXpos - bgMove / 2, temp.iYpos, temp.GetWidth() / 2, temp.GetHeight() / 2, 0, 0, temp.GetWidth(), temp.GetHeight());
			}

			// 코인
			for (Coin& temp : Coins) {
				temp.myImage->Draw(mem1dc, temp.iXpos - bgMove / 2, temp.iYpos, temp.GetWidth() / 2, temp.GetHeight() / 2, 0 + temp.GetWidth() * temp.GetSpriteX(), 0 + temp.GetHeight() * temp.GetSpriteY(), temp.GetWidth(), temp.GetHeight());
			}

			//// semin, 몬스터
			//for (int i = 0; i < MONSTERNUM; i++) {
			//	monster.myImage->TransparentBlt(mem1dc, Monsters[i].send.iXpos - bgMove / 2, Monsters[i].send.iYpos, monster.GetWidth() / 2, monster.GetHeight() / 2, 0 + monster.GetWidth() * Monsters[i].send.uSpriteX, 0 + monster.GetHeight() * Monsters[i].send.uSpriteY, monster.GetWidth(), monster.GetHeight(), RGB(0, 255, 0));
			//}

			for (int i{ 0 }; i < MONSTERNUM;++i) {
				monster.myImage->TransparentBlt(mem1dc, GameData.monsters[i].iXpos- bgMove / 2, GameData.monsters[i].iYpos,monster.GetWidth() / 2, monster.GetHeight() / 2,
					0 + monster.GetWidth() * GameData.monsters[i].uSpriteX, 0 + monster.GetHeight() * GameData.monsters[i].uSpriteY, monster.GetWidth(), monster.GetHeight(), RGB(0, 255, 0));
			}


			// W Key - Player Collision
			if (player.IsCollidedKey(key))
			{
				player.SetHasKey(TRUE);
			}
			
			// W Collision Box Test
			if (IsDebugMode) {
				SetTextAlign(mem1dc, TA_LEFT);
				TextOut(mem1dc, 10, 10, L"Debug Mode", strlen("Debug Mode"));
				RECT playerBox;
				/* semin, 여기 부분 서버 변하면서 살짝 수정했는데 모든 플레이어의 충돌박스 그릴 거라면 수정 필요함 */
				playerBox.bottom = player.iYpos + (player.GetHeight() / 2);
				playerBox.left = player.iXpos;
				playerBox.right = player.iXpos + (player.GetWidth() / 2);
				playerBox.top = player.iYpos;
				
				RECT* monsterBox;
				monsterBox = new RECT[MONSTERNUM];
				for (int i = 0; i < MONSTERNUM; i++) {
					monsterBox[i].bottom = Monsters[i].send.iYpos + (monster.GetHeight() / 2);
					monsterBox[i].left = Monsters[i].send.iXpos;
					monsterBox[i].right = Monsters[i].send.iXpos + (monster.GetWidth() / 2);
					monsterBox[i].top = Monsters[i].send.iYpos;
					//monsterBox[i] = monster.GetAABB();
				}
					
				//RECT CoinBox = TestCoin.GetAABB();
				//RECT platformbox = TestPlatform[0].GetAABB();
				//RECT platformbox1 = TestPlatform[1].GetAABB();

				HPEN MyPen, OldPen;
				HBRUSH MyBrush, OldBrush;

				if (player.IsCollidedMonster(monster) == 0)
				{
					MyPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
				}
				else
				{
					MyPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				}
				OldPen = (HPEN)SelectObject(mem1dc, MyPen);
				MyBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				OldBrush = (HBRUSH)SelectObject(mem1dc, MyBrush);

				//Rectangle(hdc, player.GetXPos(), player.GetYPos(), player.GetXPos() + player.GetWidth() / 2, player.GetYPos() + player.GetHeight() / 2);
				Rectangle(mem1dc, playerBox.left, playerBox.top, playerBox.right, playerBox.bottom);
				for ( int i = 0; i < MONSTERNUM; i++ )
					Rectangle(mem1dc, monsterBox[i].left - bgMove /2, monsterBox[i].top, monsterBox[i].right - bgMove / 2, monsterBox[i].bottom);

				if (player.IsCollidedCoin(TestCoin))
				{

				}
				//Rectangle(mem1dc, CoinBox.left, CoinBox.top, CoinBox.right, CoinBox.bottom);
				//Rectangle(mem1dc, platformbox.left, platformbox.top, platformbox.right, platformbox.bottom);

				SelectObject(mem1dc, OldPen);
				DeleteObject(MyPen);
				SelectObject(mem1dc, OldBrush);
				DeleteObject(MyBrush);
			}

			// semin, 닉네임 출력~ 제일 위에 하려고 맨 밑에 코드 씀
			SetTextAlign(mem1dc, TA_CENTER);
			SetBkMode(mem1dc, TRANSPARENT);
			for (int i = 0; i < 3; i++) {
				if (GameData.player[i].charNum < 3) {
					if (i == myCharacter)
						TextOut(mem1dc, GameData.player[i].iXpos + player.GetWidth() / 4,
							GameData.player[i].iYpos - 35, GameData.player[i].wID, wcslen((GameData.player[i].wID)));
					else {
						TextOut(mem1dc, GameData.player[i].iXpos + player.GetWidth() / 4 - GameData.player[myCharacter].iBgMove / 2 + GameData.player[i].iBgMove / 2,
							GameData.player[i].iYpos - 35, GameData.player[i].wID, wcslen((GameData.player[i].wID)));
					}
				}
			}
		}

		BitBlt(hdc, 0, 0, rect.right, rect.bottom, mem1dc, 0, 0, SRCCOPY);
		DeleteObject(hBitmap);
		DeleteDC(mem1dc);
		DeleteDC(hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		MouseX = LOWORD(lParam);
		MouseY = HIWORD(lParam);

		// W
		// 첫번째 캐릭터 선택 시: 달빛술사 쿠키
		if (bReady == FALSE && MouseX >= 80 && MouseX <= 344 && MouseY >= 637 && MouseY <= 719 && bFirstSelected == FALSE) {
			player.SetIsReady(TRUE);
			player.SetCharNum(1);
			bReady = TRUE;
			PlayerData.uCharNum = player.GetCharNum();


			retval = send(sock, (const char*)&PlayerData, sizeof(ClientToServer), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}

		// 두번째 캐릭터 선택 시: 치즈케이크맛 쿠키
		else if (bReady == FALSE && MouseX >= 510 && MouseX <= 760 && MouseY >= 646 && MouseY <= 720 && bSecondSelected == FALSE) {
			player.SetIsReady(TRUE);
			player.SetCharNum(2);
			bReady = TRUE;
			PlayerData.uCharNum = player.GetCharNum();


			retval = send(sock, (const char*)&PlayerData, sizeof(ClientToServer), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}

		// 세번째 캐릭터 선택 시: 벚꽃맛 쿠키
		else if (bReady == FALSE && MouseX >= 922 && MouseX <= 1184 && MouseY >= 643 && MouseY <= 743 && bThirdSelected == FALSE) {
			player.SetIsReady(TRUE);
			player.SetCharNum(3);
			bReady = TRUE;
			PlayerData.uCharNum = player.GetCharNum();


			retval = send(sock, (const char*)&PlayerData, sizeof(ClientToServer), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}

		
		
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:		// Sprite 의 교체 속도(업데이트 속도)를 담당하는 타이머
			//player.ChangeSprite(&spriteCnt);
			//player.UpdatePlayerLocation();					// Player 의 Move 함수는 이동을 담당
			//background.Update();
			//player.Jump(curSpriteCnt);
			//frame_time = time(NULL) - current_time;
			//frame_rate = 1.0 / frame_time;
			//current_time += frame_time;

			//monster.ChangeSprite(&spriteCnt);
			//monster.UpdateMonsterLocation();

			//TestCoin.ChangeSprite();


			for (Coin& temp : Coins) {
				temp.ChangeSprite();
			}

			portal.ChangeSprite(&spriteCnt);

			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case CHILD_BUTTON:
			if (HIWORD(wParam) == BN_CLICKED) {
				char id_send[BUFSIZE];
				GetWindowText(hEdit, wID, 20);
				wcscpy(PlayerData.wId, wID);
				ConvertWCtoC(PlayerData.wId);

				enterID = TRUE;
				DestroyWindow(hEdit);
				DestroyWindow(hButtonEdit);
				SetTimer(hWnd, 1, 16, NULL);
			}
			break;
		case CHILD_ID_EDIT:
			if (LOWORD(wParam) == EN_CHANGE) {
				GetWindowText(hEdit, wID, 20);
				SetWindowText(hEdit, wID);
			}
			if (EN_CHANGE) containID = TRUE;
			break;
		}
		// InvalidateRect(hWnd, NULL, TRUE);
		return FALSE;


	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			curSpriteCnt = 3;
			PlayerData.Input.bLeft = TRUE;
			PlayerData.Up.bLeft = FALSE;
			PlayerData.Up.bRight = FALSE;
			//bgMove = std::clamp(bgMove, -200, 4000);


			retval = send(sock, (const char*)&PlayerData, sizeof(ClientToServer), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}
		if (wParam == VK_RIGHT) {
			curSpriteCnt = 1;
			PlayerData.Input.bRight = TRUE;
			PlayerData.Up.bLeft = FALSE;
			PlayerData.Up.bRight = FALSE;

			retval = send(sock, (const char*)&PlayerData, sizeof(ClientToServer), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}
		if (wParam == VK_UP) {

		}
		if (wParam == VK_SPACE) {
			PlayerData.Input.bSpace = TRUE;
			PlayerData.Up.bSpace = FALSE;

			retval = send(sock, (const char*)&PlayerData, sizeof(ClientToServer), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}
		if (wParam == 0x44) {
			if (IsDebugMode)	IsDebugMode = false;
			else IsDebugMode = true;
		}
		break;

	case WM_KEYUP:
		if (wParam == VK_LEFT) {
			PlayerData.Input.bLeft = FALSE;
			PlayerData.Up.bLeft = TRUE;

			retval = send(sock, (const char*)&PlayerData, sizeof(ClientToServer), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}

		if (wParam == VK_RIGHT) {
			PlayerData.Input.bRight = FALSE;
			PlayerData.Up.bRight = TRUE;

			retval = send(sock, (const char*)&PlayerData, sizeof(ClientToServer), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}
		if (wParam == VK_SPACE) {
			PlayerData.Input.bSpace = FALSE;
			PlayerData.Up.bSpace = TRUE;
			retval = send(sock, (const char*)&PlayerData, sizeof(ClientToServer), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}

		}
		break;

	case WM_CHAR:

		break;

	case WM_DESTROY:
	{
		PlayerData.uCharNum = 10000;
		retval = send(sock, (const char*)&PlayerData, sizeof(ClientToServer), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		KillTimer(hWnd, 1);

		PostQuitMessage(0);

		// 소켓 닫기
		closesocket(sock);

		// 윈속 종료
		WSACleanup();
		return 0;
		break; }
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void UpdatePlayerInput(WPARAM input, Player player) {
};


char* ConvertWCtoC(wchar_t* str)
{
	//반환할 char* 변수 선언
	char* pStr;

	//입력받은 wchar_t 변수의 길이를 구함
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	//char* 메모리 할당
	pStr = new char[strSize];

	//형 변환
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);

	return pStr;
}

char* UTF8ToANSI(char* pszCode)
{
	BSTR    bstrWide;
	char* pszAnsi;
	int     nLength;
	// Get nLength of the Wide Char buffer
	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1,
		NULL, NULL);
	bstrWide = SysAllocStringLen(NULL, nLength);
	// Change UTF-8 to Unicode (UTF-16)
	MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide,
		nLength);
	// Get nLength of the multi byte buffer
	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[nLength];
	// Change from unicode to mult byte
	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);
	return pszAnsi;
}
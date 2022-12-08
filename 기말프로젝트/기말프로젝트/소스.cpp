#pragma comment(lib, "winmm")
#include "Common.h"
#include "global.h"
#include <string>
#include <sstream>

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
#include "Heart.h"
#include "SendRecvData.h"
#include "CoinEffect.h"

#include "Coin.h"
#include <time.h>


using namespace std;
char* SERVERIP = (char*)"127.0.0.1";

#define SERVERPORT 9000
#define BUFSIZE    128

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



int retval;
SOCKET sock;		// 소켓


vector<Platform> Platforms;


Player player;

ClientToServer PlayerData;
ServerToClient GameData;


CImage platformImg;
CImage coinImg;
CImage monsterImg;
CImage playersImag[6];

std::wstring s2ws(const std::string& s);


void LoadImg()
{
	
	platformImg.Load(L"Image/platform5.png");
	coinImg.Load(L"Image/coin3.png");
	monsterImg.Load(L"Image/Monster2.png");

	playersImag[0].Load(L"Image/Cookies3-2.png");	
	playersImag[1].Load(L"Image/Cookies2-2.png");
	playersImag[2].Load(L"Image/Cookies4-1.png");

	// 무적상태일 때
	playersImag[3].Load(L"Image/Cookies3-3.png");
	playersImag[4].Load(L"Image/Cookies2-3.png");
	playersImag[5].Load(L"Image/Cookies4-2.png");
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



	int temp[2];


	for (int i{ 0 }; i < PLATFORMNUM; ++i) {
		retval = recv(sock, (char*)temp, sizeof(int) * 2, 0);
		Platforms.push_back(Platform(temp[0], temp[1], &platformImg));
	}



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
	HFONT hFont2;

	static int MouseX, MouseY;
	static RECT rect;
	static HBITMAP hBitmap;

	COLORREF color;

	static Background startBackground;
	static Background background;


	static CImage startbackgroundImg;
	static CImage endingImg;
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


	// W Coin effect
	static CoinEffect ce[COINNUM];
	static CImage ceImg;
	for (int i = 0; i < COINNUM; ++i)
	{
		ce[i].myImage = &ceImg;
	}

	// W 몬스터 생성
	static CMonster monster;
	monster.SetMonNum(1);
	monster.myImage= &monsterImg;

	Coin coin;


	// W 열쇠 생성
	static CImage KeyImg;
	static Key key;
	key.myImage = &KeyImg;

	// W 포탈 생성
	static CImage PortalImg;
	static Portal portal;
	portal.myImage = &PortalImg;

	// W 하트 생성
	static CImage HeartImg;
	static Heart heart;
	heart.myImage = &HeartImg;

	time_t frame_time{};
	time_t current_time = time(NULL);
	time_t frame_rate;
	static int spriteCnt = 0;
	static int heartSpriteCnt = 0;
	static int ceSpriteCnt[COINNUM] = {};
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
		endingImg.Load(L"Image/gameClear.png");
		backgroundImg.Load(L"Image/background.jpg");
		ground.Load(L"Image/ground2.png");
		playerImg.Load(L"Image/Cookies3.png");

		// W load character selection window image
		selectBackgroundImg.Load(L"Image/Select.png");
		selectBackground.setHeight(selectBackground.Image->GetWidth());
		selectBackground.SetWidth(selectBackground.Image->GetWidth());

		readyImg.Load(L"Image/Ready.png");

		// W load coin effect image
		ceImg.Load(L"Image/coin effect.png");


		// W load key image
		KeyImg.Load(L"Image/key.png");

		// W load portal image
		PortalImg.Load(L"Image/Portal2.png");

		// W load heart image
		HeartImg.Load(L"Image/heart.png");

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

			AddFontResourceA("Rix고딕 B.ttf");
			hEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT,
				895, 385, 130, 30, hWnd, (HMENU)CHILD_ID_EDIT, hInst, NULL);
			hFont2 = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
				VARIABLE_PITCH | FF_ROMAN, L"Rix고딕 B");
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont2, (LPARAM)FALSE);
		}
		// W render character selection window
		else if (enterID == TRUE &&	bReady == FALSE) {
			// 조건문에 bReady == FALSE 대신 bIsPlaying == FALSE 해두면 3명 접속해야지 실행
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
		else if ( GameData.bGameEnd == FALSE ) {
			background.Image->Draw(mem1dc, 0, 0, rect.right, rect.bottom, 200 + bgMove, 220, 2560, 1600);

			//playerImg.Draw
			for (int i = 0; i < 3; ++i) {
				if (GameData.player[i].charNum < 3) {
					if (i != myCharacter) {
						playersImag[GameData.player[i].charNum].TransparentBlt(mem1dc, GameData.player[i].iXpos - GameData.player[myCharacter].iBgMove / 2 + GameData.player[i].iBgMove / 2, GameData.player[i].iYpos, player.GetWidth() / 2, player.GetHeight() / 2,
							0 + player.GetWidth() * GameData.player[i].uSpriteX, 0 + player.GetHeight() * GameData.player[i].uSpriteY, 170, 148, RGB(255, 0, 255));
					}

					if (i == myCharacter) {
						if (GameData.player[i].timeRecord == TRUE)
						{
							playersImag[GameData.player[myCharacter].charNum+3].TransparentBlt(mem1dc, GameData.player[myCharacter].iXpos, GameData.player[myCharacter].iYpos, player.GetWidth() / 2, player.GetHeight() / 2,
								0 + player.GetWidth() * GameData.player[myCharacter].uSpriteX, 0 + player.GetHeight() * GameData.player[myCharacter].uSpriteY, 170, 148, RGB(255, 0, 255));

						}
						else
						{
							playersImag[GameData.player[myCharacter].charNum].TransparentBlt(mem1dc, GameData.player[myCharacter].iXpos, GameData.player[myCharacter].iYpos, player.GetWidth() / 2, player.GetHeight() / 2,
								0 + player.GetWidth() * GameData.player[myCharacter].uSpriteX, 0 + player.GetHeight() * GameData.player[myCharacter].uSpriteY, 170, 148, RGB(255, 0, 255));

						}
					}
				}
			}


			// W Monster Draw	


																																																								// W Key Draw
			if (!player.GetHasKey())
			{
				key.myImage->Draw(mem1dc, key.iXpos - bgMove / 2, key.iYpos, key.GetWidth() / 2, key.GetHeight() / 2, 0, 0, 163, 148);
			}


			// 포탈
			portal.myImage->TransparentBlt(mem1dc, portal.iXpos - bgMove / 2, portal.iYpos, portal.GetWidth() / 1.5, portal.GetHeight() / 1.5, 0 + portal.GetWidth() * portal.GetSpriteX(), 0 + portal.GetHeight() * portal.GetSpriteY(), 182, 206, RGB(0, 0, 255));

			// W 내 캐릭터에 대해 맞는 체력창 출력
			for (int i = 0; i < 3; i++) {
				if (PlayerData.uCharNum == GameData.player[i].charNum + 1) {
					USHORT uHeartNum = GameData.player[i].uHeart;
					for (int j = 0; j < uHeartNum; ++j) {
						heart.myImage->Draw(mem1dc, 1200 - (j * (heart.GetWidth() / 6 + 5)), 10, heart.GetWidth() / 6, heart.GetHeight() / 6, 0 + heart.GetWidth() * heart.GetSpriteX(), 0 + heart.GetHeight() * heart.GetSpriteY(), heart.GetWidth(), heart.GetHeight());
					}
				}
			}

			//가온 - 플랫폼 - 위치 서버에서 바꿔줘야함 걍 대충 바갑가ㅏㅏ함
			for (Platform& temp : Platforms) {
				platformImg.TransparentBlt(mem1dc, temp.iXpos - bgMove / 2, temp.iYpos, temp.GetWidth() / 2, temp.GetHeight() / 2, 0, 0, temp.GetWidth(), temp.GetHeight(), RGB(255,255,0));
			}



			for (int i{ 0 }; i < MONSTERNUM; ++i) {
				monsterImg.TransparentBlt(mem1dc, GameData.monsters[i].iXpos - bgMove / 2, GameData.monsters[i].iYpos, monster.GetWidth() / 2, monster.GetHeight() / 2,
					0 + monster.GetWidth() * GameData.monsters[i].uSpriteX, 0 + monster.GetHeight() * GameData.monsters[i].uSpriteY, monster.GetWidth(), monster.GetHeight(), RGB(0, 255, 0));
			}

			// 코인
			for (int i{ 0 }; i < COINNUM; ++i) {
				if (GameData.coins[i].bIsCrush == FALSE)
					coinImg.TransparentBlt(mem1dc, GameData.coins[i].iXpos - bgMove / 2, GameData.coins[i].iYpos, coin.GetWidth() / 1.5, coin.GetHeight() / 1.5,
						0 + coin.GetWidth() * GameData.coins[i].uSpriteX, 0 + coin.GetHeight() * GameData.coins[i].uSpriteY, coin.GetWidth(), coin.GetHeight(), RGB(255, 255, 0));
			}

			// W Coin - Player Collision
			for (int i = 0; i < COINNUM; i++) {
				// 코인 위치에 출력
				if (GameData.coins[i].bIsCrush == TRUE && ce[i].GetDone() == FALSE) {
				ce[i].myImage->TransparentBlt(mem1dc, GameData.coins[i].iXpos - bgMove / 2, GameData.coins[i].iYpos, ce[i].GetWidth(), ce[i].GetHeight(),
					0 + ce[i].GetWidth() * ce[i].GetSpriteX(), 0 + ce[i].GetHeight() * ce[i].GetSpriteY(), ce[i].GetWidth(), ce[i].GetHeight(), RGB(255, 0, 0));

			}
		}

			// W Key - Player Collision
			if (player.IsCollidedKey(key))
			{
				player.SetHasKey(TRUE);
			}
			//ce.myImage->Draw(mem1dc, 800, 500, ce.GetWidth(), ce.GetHeight(), 0 + ce.GetWidth() * ce.GetSpriteX(), 0 + ce.GetHeight() * ce.GetSpriteY(), ce.GetWidth(), coin.GetHeight());

			
			// W Collision Box Test
			if (IsDebugMode) {
				SetTextAlign(mem1dc, TA_LEFT);
				TextOut(mem1dc, 10, 10, L"Debug Mode", strlen("Debug Mode"));
				RECT playerBox;

				/* semin, 여기 부분 서버 변하면서 살짝 수정했는데 모든 플레이어의 충돌박스 그릴 거라면 수정 필요함 */

				playerBox.bottom = GameData.player[myCharacter].aabb.bottom;
				playerBox.left = GameData.player[myCharacter].aabb.left;
				playerBox.right = GameData.player[myCharacter].aabb.right;
				playerBox.top = GameData.player[myCharacter].aabb.top;
	
				
				RECT* monsterBox;
				monsterBox = new RECT[MONSTERNUM];
				for (int i = 0; i < MONSTERNUM; i++) {
					monsterBox[i].bottom = GameData.monsters[i].aabb.bottom;
					monsterBox[i].left = GameData.monsters[i].aabb.left;
					monsterBox[i].right = GameData.monsters[i].aabb.right;
					monsterBox[i].top = GameData.monsters[i].aabb.top;
				}
					
				RECT* coinBox;
				coinBox = new RECT[COINNUM];
				for (int i = 0; i < COINNUM; i++) {
					coinBox[i].bottom = GameData.coins[i].aabb.bottom;
					coinBox[i].left = GameData.coins[i].aabb.left;
					coinBox[i].right = GameData.coins[i].aabb.right;
					coinBox[i].top = GameData.coins[i].aabb.top;
				}

				RECT* platformBox;
				platformBox = new RECT[PLATFORMNUM];
				for (int i = 0; i < PLATFORMNUM; i++) {
					platformBox[i].bottom =	Platforms[i].aabb.bottom;
					platformBox[i].left =	Platforms[i].aabb.left;
					platformBox[i].right=	Platforms[i].aabb.right;
					platformBox[i].top =	Platforms[i].aabb.top;
				}

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
				for (int i = 0; i < MONSTERNUM; i++) {
					if ( GameData.monsters[i].isDeath == FALSE )
						Rectangle(mem1dc, monsterBox[i].left - bgMove / 2, monsterBox[i].top, monsterBox[i].right - bgMove / 2, monsterBox[i].bottom);
				}
				for (int i = 0; i < COINNUM; i++) {
					if (GameData.coins[i].bIsCrush == FALSE)
						Rectangle(mem1dc, coinBox[i].left - bgMove / 2, coinBox[i].top, coinBox[i].right - bgMove / 2, coinBox[i].bottom);
				}
				for (int i = 0; i < PLATFORMNUM; i++) {
					Rectangle(mem1dc, platformBox[i].left - bgMove / 2, platformBox[i].top, platformBox[i].right - bgMove / 2, platformBox[i].bottom);
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

			HFONT hFont, OldFont;
			hFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Rix고딕 B"));
			OldFont = (HFONT)SelectObject(mem1dc, hFont);
			for (int i = 0; i < 3; i++) {
				if (GameData.player[i].charNum < 3) {
					if (i == myCharacter)
						TextOut(mem1dc, GameData.player[i].iXpos + player.GetWidth() / 4,
							GameData.player[i].iYpos - 30, GameData.player[i].wID, wcslen((GameData.player[i].wID)));
					else {
						TextOut(mem1dc, GameData.player[i].iXpos + player.GetWidth() / 4 - GameData.player[myCharacter].iBgMove / 2 + GameData.player[i].iBgMove / 2,
							GameData.player[i].iYpos - 30, GameData.player[i].wID, wcslen((GameData.player[i].wID)));
					}
				}
			}

			// W 텍스트 출력
			AddFontResourceA("CookieRun Bold.ttf"); 
			wstringstream time;
			wstringstream score;
			time << (int)GameData.ServerTime / CLOCKS_PER_SEC;
			//HFONT hFont, OldFont;
			hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("CookieRun Bold"));
			OldFont = (HFONT)SelectObject(mem1dc, hFont);
			SetTextColor(mem1dc, RGB(0, 0, 0));


			for (int i = 0; i < 3; i++) {
				if (PlayerData.uCharNum == GameData.player[i].charNum + 1) {
					score << GameData.player[i].uScore;
				}
			}

			TextOut(mem1dc, 1280 / 2 + 3 - 50, 13, L"SCORE", strlen("SCORE"));
			TextOut(mem1dc, 1280 / 2 + 3 + 50, 13, score.str().c_str(), wcslen(score.str().c_str()));
			TextOut(mem1dc, 30 + 3, 13, time.str().c_str(), wcslen(time.str().c_str()));

			SetTextColor(mem1dc, RGB(255, 255, 255));
			TextOut(mem1dc, 1280 / 2 - 50, 10, L"SCORE", strlen("SCORE"));
			TextOut(mem1dc, 30, 10, time.str().c_str(), wcslen(time.str().c_str()));
			SetTextColor(mem1dc, RGB(0, 0, 100));
			TextOut(mem1dc, 1280 / 2 + 50, 10, score.str().c_str(), wcslen(score.str().c_str()));
			SelectObject(mem1dc, OldFont);
			DeleteObject(hFont);

		}

		else if (GameData.bGameEnd == TRUE) {
			endingImg.Draw(mem1dc, 0, 0, rect.right, rect.bottom, background.window_left, background.window_bottom, 1280, 800);
			for (int i = 0; i < 3; i++) {
				// 닉네임도 
				if (GameData.player[i].uRank == 1) {	// semin, 내림차순으로 해서 3이 1등임... 귀찮아서 이렇게 구현함
					playersImag[GameData.player[i].charNum].TransparentBlt(mem1dc, 548, 430, player.GetWidth(), player.GetHeight(),
							player.GetWidth() * GameData.player[i].uSpriteX, player.GetHeight() * GameData.player[i].uSpriteY, 170, 148, RGB(255, 0, 255));
					TextOut(mem1dc, 548 + player.GetWidth() / 2, 456 - 60,
						GameData.player[i].wID, wcslen((GameData.player[i].wID)));
				}
				if (GameData.player[i].uRank == 2) {
					playersImag[GameData.player[i].charNum].TransparentBlt(mem1dc, 365, 490, player.GetWidth(), player.GetHeight(),
						player.GetWidth() * GameData.player[i].uSpriteX, player.GetHeight() * GameData.player[i].uSpriteY, 170, 148, RGB(255, 0, 255));
					TextOut(mem1dc, 365 + player.GetWidth() / 2, 521 - 60,
						GameData.player[i].wID, wcslen((GameData.player[i].wID)));
				}
				if (GameData.player[i].uRank == 3) {
					playersImag[GameData.player[i].charNum].TransparentBlt(mem1dc, 729, 490, player.GetWidth(), player.GetHeight(),
						player.GetWidth()* GameData.player[i].uSpriteX, player.GetHeight()* GameData.player[i].uSpriteY, 170, 148, RGB(255, 0, 255));
					TextOut(mem1dc, 729 + player.GetWidth() / 2, 521 - 60,
						GameData.player[i].wID, wcslen((GameData.player[i].wID)));
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

		// semin, 접속 버튼
		if (enterID == FALSE && MouseX >= 902 && MouseX <= 982 && MouseY >= 506 && MouseY <= 587) {
			char id_send[BUFSIZE];
			GetWindowText(hEdit, wID, 20);
			wcscpy(PlayerData.wId, wID);
			ConvertWCtoC(PlayerData.wId);

			enterID = TRUE;
			DestroyWindow(hEdit);
			SetTimer(hWnd, 1, 16, NULL);
		}

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
		case 1:		
			heart.ChangeSprite(&heartSpriteCnt);
			portal.ChangeSprite(&spriteCnt);
			for (int i = 0; i < COINNUM; ++i)
			{
				//if (GameData.coins[i].bIsCrush == TRUE)
				//ce[i].ChangeSprite(&ceSpriteCnt[i]);
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
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

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

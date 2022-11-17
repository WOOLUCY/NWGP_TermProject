#pragma comment(lib, "winmm")

#include <tchar.h>
#include <atlImage.h>
#include <mmsystem.h>

#include "Player.h"
#include "CMonster.h"
#include "Background.h"
#include "Platform.h"
#include "Collision.h"

#include "Coin.h"
#include <time.h>

using namespace std;

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512


bool IsDebugMode = false;
void UpdatePlayerInput(WPARAM Input, Player player);

LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"쿠키런 이스케이프";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst; // 인스턴스 핸들
HWND hEdit; // 에디트 컨트롤
HWND hButtonEdit; // 에디트 컨트롤


#define	CHILD_BUTTON	101
#define	CHILD_ID_EDIT	102


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

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

struct BLOCK {
	int x;
	int y;
	int width;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, mem1dc;
	PAINTSTRUCT ps;

	static int MouseX, MouseY;
	static RECT rect;
	static HBITMAP hBitmap;

	COLORREF color;

	BLOCK Block_local[25] = { 0 };

	static Background startBackground;
	static Background background;
	static CImage startbackgroundImg;
	static CImage backgroundImg;
	static CImage ground;

	startBackground.Image = &startbackgroundImg;
	background.Image = &backgroundImg;	// Background 클래스의 Image 는 CImage 를 가르킨다.
	
	static CImage playerImg;
	static Player player;
	player.myImage[0] = &playerImg;		// Player 클래스의 myImage 는 CImage 를 가르킨다.

	//일단 코인한개만 그려보겠슴니다 - 가온(찾을때편하려고 이름 씀~~)
	static CImage coinImg;
	static Coin TestCoin;
	static CImage platformImg;
	static Platform TestPlatform;

	TestCoin.myImage = &coinImg;
	TestPlatform.myImage = &platformImg;

	// W 몬스터 생성
	static CImage MonsterImg;
	static CMonster monster;
	monster.myImage[0] = &MonsterImg;

	time_t frame_time{};
	time_t current_time = time(NULL);
	time_t frame_rate;
	static int spriteCnt = 0;
	static USHORT curSpriteCnt = 0;

	// ID
	static LPWSTR id;
	static bool enterID = FALSE;
	static WCHAR wID[21] = { NULL };

	switch (iMsg) {
	case WM_CREATE:
		// PlaySound(L"start.wav", NULL, SND_ASYNC);	// 듣기 싫어서 사운드 막아둠 ㅎㅎ
		startbackgroundImg.Load(L"Image/ID입력창.png");
		backgroundImg.Load(L"Image/BackGround.png");
		ground.Load(L"Image/ground.png");
		playerImg.Load(L"Image/Cookies2-1.png");

		// W 몬스터이미지 로드
		MonsterImg.Load(L"Image/Monster.png");

		//가온 - 코인이미지 로드 
		coinImg.Load(L"Image/coin2.png");
		platformImg.Load(L"Image/Platform2.png");


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

		if (enterID == FALSE) {
			startBackground.Image->Draw(mem1dc, 0, 0, rect.right, rect.bottom, background.window_left, background.window_bottom, 1280, 800);

			hEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT,
				895, 385, 130, 30, hWnd, (HMENU)CHILD_ID_EDIT, hInst, NULL);
			hButtonEdit = CreateWindow(L"button", L"접속", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				890, 505, 80, 40, hWnd, (HMENU)CHILD_BUTTON, hInst, NULL);
			// ShowWindow(hButtonEdit, SW_HIDE);
		}

		else {
			TextOut(hdc, player.iXpos + 30 - wcslen(player.GetId()) * 2, player.iYpos - 20, player.GetId(), wcslen(wID));
			background.Image->Draw(mem1dc, 0, 0, rect.right, rect.bottom, background.window_left, background.window_bottom, 2560, 1600);
			ground.Draw(mem1dc, 0, 130, rect.right, rect.bottom, 0, 0, ground.GetWidth(), ground.GetHeight());
			player.myImage[0]->Draw(mem1dc, player.iXpos, player.iYpos, player.GetWidth() / 2, player.GetHeight() / 2, 0 + player.GetWidth() * player.GetSpriteX(), 0 + player.GetHeight() * player.GetSpriteY(), 170, 148);
			//TextOut(hdc, player.iXpos - 10, player.iYpos, (LPCWSTR)player.GetId(), wcslen(player.GetId()));
			// W Monster Draw
			monster.myImage[0]->Draw(mem1dc, monster.iXpos, monster.iYpos, monster.GetWidth() / 2, monster.GetHeight() / 2, 0 + monster.GetWidth() * monster.GetSpriteX(), 0 + monster.GetHeight() * monster.GetSpriteY(), 144, 138);
			//가온-코인그리기 
			TestCoin.myImage->Draw(mem1dc, TestCoin.iXpos, TestCoin.iYpos, TestCoin.GetWidth() / 2, TestCoin.GetHeight() / 2, 0 + TestCoin.GetWidth() * TestCoin.GetSpriteX(), 0 + TestCoin.GetHeight() * TestCoin.GetSpriteY(), TestCoin.GetWidth(), TestCoin.GetHeight());
			TestPlatform.myImage->Draw(mem1dc, TestPlatform.iXpos, TestPlatform.iYpos, TestPlatform.GetWidth() / 2, TestPlatform.GetHeight() / 2, 0 , 0, TestPlatform.GetWidth(), TestPlatform.GetHeight());

			// W Collision Box Test
			if (IsDebugMode) {
				TextOut(hdc, 10, 10, L"Debug Mode", strlen("Debug Mode"));
				RECT playerBox = player.GetAABB();
				RECT monsterBox = monster.GetAABB();
				RECT CoinBox = TestCoin.GetAABB();
				RECT platformbox = TestPlatform.GetAABB();

				HPEN MyPen, OldPen;
				HBRUSH MyBrush, OldBrush;

				if (!player.IsCollided(monster))
				{
					MyPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
				}
				else
				{
					MyPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				}
				OldPen = (HPEN)SelectObject(hdc, MyPen);
				MyBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);

				// Rectangle(hdc, player.GetXPos(), player.GetYPos(), player.GetXPos() + player.GetWidth() / 2, player.GetYPos() + player.GetHeight() / 2);
				Rectangle(hdc, playerBox.left, playerBox.top, playerBox.right, playerBox.bottom);
				Rectangle(hdc, monsterBox.left, monsterBox.top, monsterBox.right, monsterBox.bottom);
				Rectangle(hdc, CoinBox.left, CoinBox.top, CoinBox.right, CoinBox.bottom);
				Rectangle(hdc, platformbox.left, platformbox.top, platformbox.right, platformbox.bottom);

				SelectObject(hdc, OldPen);
				DeleteObject(MyPen);
				SelectObject(hdc, OldBrush);
				DeleteObject(MyBrush);
			}

		}
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, mem1dc, 0, 0, SRCCOPY);
		DeleteObject(hBitmap);
		DeleteDC(mem1dc);
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		MouseX = LOWORD(lParam);
		MouseY = HIWORD(lParam);

		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:		// Sprite 의 교체 속도(업데이트 속도)를 담당하는 타이머
			player.ChangeSprite(&spriteCnt);
			player.UpdatePlayerLocation();					// Player 의 Move 함수는 이동을 담당
			background.Update();
			player.Jump(curSpriteCnt);
			frame_time = time(NULL) - current_time;
			frame_rate = 1.0 / frame_time;
			current_time += frame_time;

			monster.ChangeSprite(&spriteCnt);
			monster.UpdateMonsterLocation();

			TestCoin.ChangeSprite();
			break;

		case 2:		// 플레이어의 '이동'을 담당하는 타이머
					// 이것도 스크롤링의 잔해... 처리 못 함
			break;

		case 3:		// 플레이어의 '점프'를 담당하는 타이머
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case CHILD_BUTTON:
			if (wID != NULL) {
				player.SetId(wID);
				GetWindowText(hEdit, wID, 20);
				enterID = TRUE;
				SetTimer(hWnd, 1, 30, NULL);
				DestroyWindow(hEdit);
				DestroyWindow(hButtonEdit);
				// SetWindowText(hEdit, L"Aaa");
			}
			break;
		case CHILD_ID_EDIT:
			switch (LOWORD(wParam)) {
				GetWindowText(hEdit, wID, 20);
				SetWindowText(hEdit, wID);
			}
			break;
		}
		break;
				

	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			player.velocity.x = -player.GetRunSpeed();
			curSpriteCnt = 3;
			player.SetSpriteY(3);
			player.input.bLeft = TRUE;
		}
		if (wParam == VK_RIGHT) {
			player.velocity.x = player.GetRunSpeed();
			curSpriteCnt = 1;
			player.SetSpriteY(1);
			player.input.bRight = TRUE;
		}
		if (wParam == VK_UP) {

		}
		if (wParam == VK_SPACE) {
			player.bJumpKeyPressed = TRUE;
			// player.SetSpriteY(2);	// Sprite 의 Y 위치임. 0 - 기본, 1 - 오른쪽 이동, 2 - 점프, 3 - 왼쪽 이동
			player.input.bSpace = TRUE;

		}
		if (wParam == 0x44) {
			if (IsDebugMode)	IsDebugMode = false;
			else IsDebugMode = true;
		}
		break;

	case WM_KEYUP:
		if (wParam == VK_LEFT) {
			player.velocity.x = 0;
			curSpriteCnt = 0;
			player.SetSpriteY(0);
			player.input.bLeft = FALSE;
		}
		if (wParam == VK_RIGHT) {
			player.velocity.x = 0;
			curSpriteCnt = 0;
			player.SetSpriteY(0);
			player.input.bLeft = FALSE;
		}
		if (wParam == VK_UP) {

		}
		break;

	case WM_CHAR:

		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);

		PostQuitMessage(0);

		break;
	
		}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void UpdatePlayerInput(WPARAM input, Player player) {
};
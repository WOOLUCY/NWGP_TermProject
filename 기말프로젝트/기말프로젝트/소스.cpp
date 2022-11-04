#pragma comment(lib, "winmm")

#include <tchar.h>
#include <atlImage.h>
#include <mmsystem.h>

#include "Player.h"
#include "Monster.h"
#include "Background.h"
#include <time.h>

using namespace std;


void UpdatePlayerInput(WPARAM Input, Player player);

LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
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

	static Background background;
	static CImage backgroundImg;
	static CImage ground;
	background.Image = &backgroundImg;	// Background 클래스의 Image 는 CImage 를 가르킨다.
	
	static CImage playerImg;
	static Player player;
	player.myImage[0] = &playerImg;		// Player 클래스의 myImage 는 CImage 를 가르킨다.

	time_t frame_time{};
	time_t current_time = time(NULL);
	time_t frame_rate;
	static int spriteCnt = 0;
	static USHORT curSpriteCnt = 0;

	switch (iMsg) {
	case WM_CREATE:
		// PlaySound(L"start.wav", NULL, SND_ASYNC);	// 듣기 싫어서 사운드 막아둠 ㅎㅎ

		backgroundImg.Load(L"Image/BackGround.png");
		ground.Load(L"Image/ground.png");
		playerImg.Load(L"Image/Cookies2-1.png");

		background.SetWidth(background.Image->GetWidth());
		background.setHeight(background.Image->GetHeight());
		// 플레이어의 Width, Height 크기는 170, 148 로 고정이라 구하지 않음

		GetClientRect(hWnd, &rect);
		SetTimer(hWnd, 1, 30, NULL);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		mem1dc = CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBitmap);

		background.Image->Draw(mem1dc, 0, 0, rect.right, rect.bottom, background.window_left, background.window_bottom, 2560, 1600);
		ground.Draw(mem1dc, 0, 130, rect.right, rect.bottom, 0, 0, ground.GetWidth(), ground.GetHeight());
		player.myImage[0]->Draw(mem1dc, player.iXpos, player.iYpos, player.GetWidth() / 2, player.GetHeight() / 2, 0 + player.GetWidth() * player.GetSpriteX(), 0 + player.GetHeight() * player.GetSpriteY(), 170, 148);

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
			break;

		case 2:		// 플레이어의 '이동'을 담당하는 타이머
					// 이것도 스크롤링의 잔해... 처리 못 함
			break;

		case 3:		// 플레이어의 '점프'를 담당하는 타이머
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			player.velocity.x = -player.GetRunSpeed();
			curSpriteCnt = 3;
			player.SetSpriteY(3);
		}
		if (wParam == VK_RIGHT) {
			player.velocity.x = player.GetRunSpeed();
			curSpriteCnt = 1;
			player.SetSpriteY(1);
		}
		if (wParam == VK_UP) {

		}
		if (wParam == VK_SPACE) {
			player.bJumpKeyPressed = TRUE;
			// player.SetSpriteY(2);	// Sprite 의 Y 위치임. 0 - 기본, 1 - 오른쪽 이동, 2 - 점프, 3 - 왼쪽 이동

		}
		break;

	case WM_KEYUP:
		if (wParam == VK_LEFT) {
			player.velocity.x = 0;
			curSpriteCnt = 0;
			player.SetSpriteY(0);
		}
		if (wParam == VK_RIGHT) {
			player.velocity.x = 0;
			curSpriteCnt = 0;
			player.SetSpriteY(0);
		}
		if (wParam == VK_UP) {

		}
		break;

	case WM_CHAR:

		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);

		PostQuitMessage(0);

		break;
	
		}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void UpdatePlayerInput(WPARAM input, Player player) {
};
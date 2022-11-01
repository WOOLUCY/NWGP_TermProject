#pragma comment(lib, "winmm")

#include <tchar.h>
#include <atlImage.h>
#include <mmsystem.h>

#include "Player.h"
#include "Monster.h"
#include "Background.h"



using namespace std;

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
	static int cx, cy = 0;

	static float movingForce = 2.f;	// 이동 속도
	static POS ForceDirection;

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
		SetTimer(hWnd, 1, 50, NULL);
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
			player.ChangeSprite();	
			break;

		case 2:		// 플레이어의 '이동'을 담당하는 타이머
			cx = player.iXpos - background.window_left;		// cx, cy 는 스크롤링 도전하려다가 포기한 것... 처리 못 함
			cy = player.iYpos - background.window_bottom;
			player.Move(ForceDirection);					// Player 의 Move 함수는 이동을 담당
			background.Update();							// 이것도 스크롤링의 잔해... 처리 못 함
			break;

		case 3:		// 플레이어의 '점프'를 담당하는 타이머
			player.SetSpriteY(2);	// Sprite 의 Y 위치임. 0 - 기본, 1 - 오른쪽 이동, 2 - 점프, 3 - 왼쪽 이동
			static float gravity = player.GetMaxJump();
			player.JumpHeight += gravity;
			player.iYpos -= gravity;
			gravity -= 0.1;	
			if (player.JumpHeight <= 0) {		// AABB 로 멈추기 필요함. 지금은 얼렁뚱땅 넣어둠.
				player.JumpHeight = 0;
				gravity = 3.f;
				player.SetSpriteY(0);
				KillTimer(hWnd, 3);
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			ForceDirection.x = -1;
			player.SetSpriteY(3);
			SetTimer(hWnd, 2, 5, NULL);
		}
		if (wParam == VK_RIGHT) {
			ForceDirection.x = 1;
			player.SetSpriteY(1);
			SetTimer(hWnd, 2, 5, NULL);
		}
		if (wParam == VK_UP) {

		}
		if (wParam == VK_SPACE) {
			SetTimer(hWnd, 3, 10, NULL);
		}
		ForceDirection.x *= movingForce;
		ForceDirection.y *= movingForce;
		break;

	case WM_KEYUP:
		if (wParam == VK_LEFT) {
			ForceDirection.x = 0.f;
			player.SetSpriteY(0);
			KillTimer(hWnd, 2);
		}
		if (wParam == VK_RIGHT) {
			ForceDirection.x = 0.f;
			player.SetSpriteY(0);
			KillTimer(hWnd, 2);
		}
		if (wParam == VK_UP) {

		}
		ForceDirection.x *= movingForce;
		ForceDirection.y *= movingForce;
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
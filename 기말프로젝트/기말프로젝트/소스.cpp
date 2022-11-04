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
	background.Image = &backgroundImg;	// Background Ŭ������ Image �� CImage �� ����Ų��.
	
	static CImage playerImg;
	static Player player;
	player.myImage[0] = &playerImg;		// Player Ŭ������ myImage �� CImage �� ����Ų��.

	time_t frame_time{};
	time_t current_time = time(NULL);
	time_t frame_rate;
	static int spriteCnt = 0;
	static USHORT curSpriteCnt = 0;

	switch (iMsg) {
	case WM_CREATE:
		// PlaySound(L"start.wav", NULL, SND_ASYNC);	// ��� �Ⱦ ���� ���Ƶ� ����

		backgroundImg.Load(L"Image/BackGround.png");
		ground.Load(L"Image/ground.png");
		playerImg.Load(L"Image/Cookies2-1.png");

		background.SetWidth(background.Image->GetWidth());
		background.setHeight(background.Image->GetHeight());
		// �÷��̾��� Width, Height ũ��� 170, 148 �� �����̶� ������ ����

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
		case 1:		// Sprite �� ��ü �ӵ�(������Ʈ �ӵ�)�� ����ϴ� Ÿ�̸�
			player.ChangeSprite(&spriteCnt);
			player.UpdatePlayerLocation();					// Player �� Move �Լ��� �̵��� ���
			background.Update();
			player.Jump(curSpriteCnt);
			frame_time = time(NULL) - current_time;
			frame_rate = 1.0 / frame_time;
			current_time += frame_time;
			break;

		case 2:		// �÷��̾��� '�̵�'�� ����ϴ� Ÿ�̸�
					// �̰͵� ��ũ�Ѹ��� ����... ó�� �� ��
			break;

		case 3:		// �÷��̾��� '����'�� ����ϴ� Ÿ�̸�
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
			// player.SetSpriteY(2);	// Sprite �� Y ��ġ��. 0 - �⺻, 1 - ������ �̵�, 2 - ����, 3 - ���� �̵�

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
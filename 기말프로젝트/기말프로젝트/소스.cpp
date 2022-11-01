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
	background.Image = &backgroundImg;	// Background Ŭ������ Image �� CImage �� ����Ų��.

	static CImage playerImg;
	static Player player;
	player.myImage[0] = &playerImg;		// Player Ŭ������ myImage �� CImage �� ����Ų��.
	static int cx, cy = 0;

	static float movingForce = 2.f;	// �̵� �ӵ�
	static POS ForceDirection;

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
		case 1:		// Sprite �� ��ü �ӵ�(������Ʈ �ӵ�)�� ����ϴ� Ÿ�̸�
			player.ChangeSprite();	
			break;

		case 2:		// �÷��̾��� '�̵�'�� ����ϴ� Ÿ�̸�
			cx = player.iXpos - background.window_left;		// cx, cy �� ��ũ�Ѹ� �����Ϸ��ٰ� ������ ��... ó�� �� ��
			cy = player.iYpos - background.window_bottom;
			player.Move(ForceDirection);					// Player �� Move �Լ��� �̵��� ���
			background.Update();							// �̰͵� ��ũ�Ѹ��� ����... ó�� �� ��
			break;

		case 3:		// �÷��̾��� '����'�� ����ϴ� Ÿ�̸�
			player.SetSpriteY(2);	// Sprite �� Y ��ġ��. 0 - �⺻, 1 - ������ �̵�, 2 - ����, 3 - ���� �̵�
			static float gravity = player.GetMaxJump();
			player.JumpHeight += gravity;
			player.iYpos -= gravity;
			gravity -= 0.1;	
			if (player.JumpHeight <= 0) {		// AABB �� ���߱� �ʿ���. ������ �󷷶׶� �־��.
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
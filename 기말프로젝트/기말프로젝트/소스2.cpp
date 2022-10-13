#include <windows.h>
#include <tchar.h>
#include <atlImage.h>
#pragma comment(lib, "winmm")
#include <mmsystem.h>

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
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, mem1dc;
	PAINTSTRUCT ps;

	static HBITMAP hBitmap;
	static CImage BackGround, imgGround;
	static CImage imgSprite[4], imgSprite_runR[4], imgSprite_runL[4], imgSprite_jump[4];
	static CImage imgSprite1[4], imgSprite1_runR[4], imgSprite1_runL[4], imgSprite1_jump[4];
	static CImage imgSprite2[4], imgSprite2_runR[4], imgSprite2_runL[4], imgSprite2_jump[4];
	static CImage Start, Dialog[6], Guide;

	static RECT rect;

	static int w_stand[10], h_stand[10];
	static int w_run[4], h_run[4];
	static int w_jump[4], h_jump[4];
	static int w1_stand[4], h1_stand[4];
	static int w1_run[4], h1_run[4];
	static int w1_jump[4], h1_jump[4];
	static int w2_stand[4], h2_stand[4];
	static int w2_run[4], h2_run[4];
	static int w2_jump[4], h2_jump[4];
	static int Image_Number = 0;
	static int w_guide, h_guide;
	static int MouseX, MouseY;

	static int bw, bh, gw, gh;

	static int x = 640;
	static int y = 620;

	static int CharX, CharY;

	static int count = 0;

	static int jump = 0;
	static int jumpcount = 0;

	static int left, right, last;

	static int CharNum = 0;

	static int click = 0;

	switch (iMsg) {
	case WM_CREATE:
		PlaySound(L"start.wav", NULL, SND_ASYNC);

		BackGround.Load(L"BackGround.png");
		imgGround.Load(L"Ground.png");

		Start.Load(L"GameStart.png");

		imgSprite[0].Load(L"AO1.png");
		imgSprite[1].Load(L"AO1.png");
		imgSprite[2].Load(L"AO2.png");
		imgSprite[3].Load(L"AO2.png");

		imgSprite1[0].Load(L"cookie1-stand1.png");
		imgSprite1[1].Load(L"cookie1-stand1.png");
		imgSprite1[2].Load(L"cookie1-stand2.png");
		imgSprite1[3].Load(L"cookie1-stand2.png");

		imgSprite1_runL[0].Load(L"cookie1-runL1.png");
		imgSprite1_runL[1].Load(L"cookie1-runL2.png");
		imgSprite1_runL[2].Load(L"cookie1-runL3.png");
		imgSprite1_runL[3].Load(L"cookie1-runL4.png");

		imgSprite1_runR[0].Load(L"cookie1-runR1.png");
		imgSprite1_runR[1].Load(L"cookie1-runR2.png");
		imgSprite1_runR[2].Load(L"cookie1-runR3.png");
		imgSprite1_runR[3].Load(L"cookie1-runR4.png");

		imgSprite1_jump[0].Load(L"cookie1-jump1.png");
		imgSprite1_jump[1].Load(L"cookie1-jump2.png");
		imgSprite1_jump[2].Load(L"cookie1-jump3.png");
		imgSprite1_jump[3].Load(L"cookie1-jump4.png");

		imgSprite2[0].Load(L"cookie2-stand1.png");
		imgSprite2[1].Load(L"cookie2-stand1.png");
		imgSprite2[2].Load(L"cookie2-stand2.png");
		imgSprite2[3].Load(L"cookie2-stand2.png");

		imgSprite2_runL[0].Load(L"cookie2-runL1.png");
		imgSprite2_runL[1].Load(L"cookie2-runL2.png");
		imgSprite2_runL[2].Load(L"cookie2-runL3.png");
		imgSprite2_runL[3].Load(L"cookie2-runL4.png");

		imgSprite2_runR[0].Load(L"cookie2-runR1.png");
		imgSprite2_runR[1].Load(L"cookie2-runR2.png");
		imgSprite2_runR[2].Load(L"cookie2-runR3.png");
		imgSprite2_runR[3].Load(L"cookie2-runR4.png");

		imgSprite2_jump[0].Load(L"cookie2-jump1.png");
		imgSprite2_jump[1].Load(L"cookie2-jump2.png");
		imgSprite2_jump[2].Load(L"cookie2-jump3.png");
		imgSprite2_jump[3].Load(L"cookie2-jump4.png");

		Guide.Load(L"팻말.png");

		Dialog[1].Load(L"Dialog1.png");
		Dialog[2].Load(L"Dialog2.png");
		Dialog[3].Load(L"Dialog3.png");
		Dialog[5].Load(L"Dialog5.png");


		GetClientRect(hWnd, &rect);

		bw = BackGround.GetWidth();
		bh = BackGround.GetHeight();
		gw = imgGround.GetWidth();
		gh = imgGround.GetHeight();

		w_guide = Guide.GetWidth();
		h_guide = Guide.GetHeight();

		for (int i = 0; i < 4; ++i) {
			w_stand[i] = imgSprite[i].GetWidth();
			h_stand[i] = imgSprite[i].GetHeight();
		}

		for (int i = 0; i < 4; ++i) {
			w1_stand[i] = imgSprite1[i].GetWidth();
			h1_stand[i] = imgSprite1[i].GetHeight();
		}

		for (int i = 0; i < 4; ++i) {
			w1_run[i] = imgSprite1_runL[i].GetWidth();
			h1_run[i] = imgSprite1_runL[i].GetHeight();
		}

		for (int i = 0; i < 4; ++i) {
			w1_jump[i] = imgSprite1_jump[i].GetWidth();
			h1_jump[i] = imgSprite1_jump[i].GetHeight();
		}

		for (int i = 0; i < 4; ++i) {
			w2_stand[i] = imgSprite2[i].GetWidth();
			h2_stand[i] = imgSprite2[i].GetHeight();
		}

		for (int i = 0; i < 4; ++i) {
			w2_run[i] = imgSprite2_runL[i].GetWidth();
			h2_run[i] = imgSprite2_runL[i].GetHeight();
		}

		for (int i = 0; i < 4; ++i) {
			w2_jump[i] = imgSprite2_jump[i].GetWidth();
			h2_jump[i] = imgSprite2_jump[i].GetHeight();
		}

		SetTimer(hWnd, 0, 100, NULL);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (count != 4) {
			count++;
		}

		if (count == 4) {
			count = 0;
		}

		hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);

		mem1dc = CreateCompatibleDC(hdc);

		SelectObject(mem1dc, hBitmap);

		BackGround.Draw(mem1dc, 0, 0, rect.right, rect.bottom, 0 + CharX, bh - 1600 + CharY, 2560, 1600);
		imgGround.Draw(mem1dc, 0 - CharX, 130 - CharY, rect.right, rect.bottom, 0, 0, gw, gh);
		imgGround.Draw(mem1dc, rect.right - CharX, 130 - CharY, rect.right, rect.bottom, 0, 0, gw, gh);

		Guide.Draw(mem1dc, 750 - CharX, 590 - CharY, w_guide * 2 / 3, h_guide * 2 / 3, 0, 0, w_guide, h_guide);      // 팻말

		if (CharNum == 0) {
			if (left == 0 && right == 0 && jump == 0) {
				imgSprite[count].Draw(mem1dc, x, y, w_stand[count], h_stand[count], 0, 0, w_stand[count], h_stand[count]);
			}
		}

		if (CharNum == 1) {
			if (left == 0 && right == 0 && jump == 0) {
				imgSprite1[count].Draw(mem1dc, x, y, w1_stand[count] / 2, h1_stand[count] / 2, 0, 0, w1_stand[count], h1_stand[count]);
			}

			if (left == 1) {
				imgSprite1_runL[count].Draw(mem1dc, x, y, w1_run[count] / 2, h1_run[count] / 2, 0, 0, w1_run[count], h1_run[count]);
			}

			if (right == 1) {
				imgSprite1_runR[count].Draw(mem1dc, x, y, w1_run[count] / 2, h1_run[count] / 2, 0, 0, w1_run[count], h1_run[count]);
			}

			if (jump == 1) {
				imgSprite1_jump[count].Draw(mem1dc, x, y, w1_jump[count] / 2, h1_jump[count] / 2, 0, 0, w1_jump[count], h1_jump[count]);
			}
		}

		if (CharNum == 2) {
			if (left == 0 && right == 0 && jump == 0) {
				imgSprite2[count].Draw(mem1dc, x, y, w2_stand[count] / 2, h2_stand[count] / 2, 0, 0, w2_stand[count], h2_stand[count]);
			}

			if (left == 1) {
				imgSprite2_runL[count].Draw(mem1dc, x, y, w2_run[count] / 2, h2_run[count] / 2, 0, 0, w2_run[count], h2_run[count]);
			}

			if (right == 1) {
				imgSprite2_runR[count].Draw(mem1dc, x, y, w2_run[count] / 2, h2_run[count] / 2, 0, 0, w2_run[count], h2_run[count]);
			}

			if (jump == 1) {
				imgSprite2_jump[count].Draw(mem1dc, x, y, w2_jump[count] / 2, h2_jump[count] / 2, 0, 0, w2_jump[count], h2_jump[count]);
			}
		}

		if (Image_Number == 0) {		// 게임 시작 이미지
			Start.Draw(mem1dc, 0, 0, rect.right, rect.bottom, 0, 0, 1280, 800);
		}

		if (Image_Number != 0 && Image_Number < 4) {
			Dialog[Image_Number].Draw(mem1dc, 0, 0, rect.right, rect.bottom, 0, 0, 1280, 800);
		}

		if (Image_Number == 5) {
			Dialog[Image_Number].Draw(mem1dc, 0, 0, rect.right, rect.bottom, 0, 0, 1280, 800);
		}


		if (jump == 1) {
			jumpcount++;

			if (jumpcount <= 5) {
				if (last == 1) {
					x -= 15;
					y -= 10;

					if (0 + CharX > 0) {
						CharX -= 10;
					}
				}

				if (last == 2) {
					x += 15;
					y -= 10;

					if (CharX < bw - 2560) {
						CharX += 10;
					}
				}

				CharY -= 10;
			}

			else {
				if (last == 1) {
					x -= 15;
					y += 10;

					if (0 + CharX > 0) {
						CharX -= 10;
					}
				}

				if (last == 2) {
					x += 15;
					y += 10;

					if (CharX < bw - 2560) {
						CharX += 10;
					}
				}

				CharY += 10;

				if (jumpcount == 10) {
					jumpcount = 0;

					jump = 0;
				}
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

		click++;

		if (click == 1) {
			PlaySound(L"OST.wav", NULL, SND_ASYNC | SND_LOOP);
		}

		if (Image_Number >= 0 && Image_Number < 4)
			Image_Number++;

		if (MouseX >= 750 + CharX && MouseX <= 750 + CharX + w_guide * 2 / 3 && MouseY >= 590 + CharY && MouseY <= 590 + CharY + h_guide * 2 / 3) {
			Image_Number = 5;
		}

		else if (Image_Number == 5)
			Image_Number = 4;

		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, FALSE);

		break;

	case WM_KEYDOWN:
		if (wParam == VK_LEFT) {
			if (x >= rect.left) {
				x -= 5;
			}

			if (0 + CharX > 0) {
				CharX -= 5;
			}

			left = 1;
			right = 0;
			last = 1;
		}

		if (wParam == VK_RIGHT) {
			if (w_stand[count] * 2 + x <= rect.right) {
				x += 5;
			}

			if (CharX - w_stand[count] < bw - 2560) {
				CharX += 5;
			}

			left = 0;
			right = 1;
			last = 2;
		}

		if (wParam == VK_SPACE) {
			jump = 1;
		}

		break;

	case WM_KEYUP:
		left = 0;
		right = 0;

		break;

	case WM_CHAR:
		if (wParam == '0') {
			CharNum = 0;

			InvalidateRect(hWnd, NULL, FALSE);
		}

		if (wParam == '1') {
			CharNum = 1;

			InvalidateRect(hWnd, NULL, FALSE);
		}

		if (wParam == '2') {
			CharNum = 2;

			InvalidateRect(hWnd, NULL, FALSE);
		}

		break;

	case WM_DESTROY:
		KillTimer(hWnd, 0);

		PostQuitMessage(0);

		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
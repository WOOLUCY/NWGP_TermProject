//#include <windows.h>
//#include <tchar.h>
//#include <math.h>
//#include <atlimage.h>
//#pragma comment(lib, "msimg32.lib")
//
//LPCTSTR lpszClass = L"Window Class Name";
//LPCTSTR lpszWindowName = L"windows program";
//
//LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
//HINSTANCE hInst;
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
//{
//	HWND hWnd;
//	MSG Message;
//	WNDCLASSEX WndClass;
//	hInst = hInstance;
//
//	WndClass.cbSize = sizeof(WndClass);
//	WndClass.style = CS_HREDRAW | CS_VREDRAW;
//	WndClass.lpfnWndProc = (WNDPROC)wndProc;
//	WndClass.cbClsExtra = 0;
//	WndClass.cbWndExtra = 0;
//	WndClass.hInstance = hInstance;
//	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
//	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	WndClass.lpszMenuName = NULL;
//	WndClass.lpszClassName = lpszClass;
//	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
//	RegisterClassEx(&WndClass);
//
//	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_SYSMENU, 0, 0, 1280, 800, NULL, (HMENU)NULL, hInstance, NULL);
//	ShowWindow(hWnd, nCmdShow);
//	UpdateWindow(hWnd);
//
//	while (GetMessage(&Message, 0, 0, 0)) {
//		TranslateMessage(&Message);
//		DispatchMessage(&Message);
//	}
//	return Message.wParam;
//}
//
//
//LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	HDC hdc, mem1dc;
//	PAINTSTRUCT ps;
//	static CImage img, imgClick, imgSprite[4], imgMonster[6], imgGround, imgBumb;
//	HBITMAP hBitmap;
//	RECT rect;
//	static int bw, bh, sw, sh, mw[6], mh[6], ch[4], cw[4], buw, buh;
//	static int Xground = 0, Xsky = 0;
//	static int Xground2 = 0, Xsky2 = 0;
//	static int CharX = 50, CharY = 520;
//	static int Rand = 0, Rand2 = 0;
//	static int SetMonster = 0, mcount = 0;
//	static int Xmonster[4] = { -100 }, Ymonster[4] = { 550 };
//	static int monsterNum = 0;
//	static int count = 0;
//	static int jump = 0, jumpdown = 0;	// 점프 판별
//	static int down = 0;
//
//	static BOOL Click = 0;		// 클릭 판별
//	static int w[12], h[12];	// 주인공 캐릭터의 높이, 크기
//	HBRUSH hBrush, oldBrush;	// 색 변화
//	static int Elie = 0;		// 시간에 따른 원 추가 생성
//	static int xPos[20] = { 0 }, yPos[20] = { 0 };	// 원 좌표(랜덤값)
//	static int R = 255, G = 255, B = 0;	// 주인공 캐릭터 색 변화
//	static int X[3], Y[3];		// 주인공 캐릭터의 좌표값
//	static int CharNum = 1;		// 주인공 캐릭터 복사개수
//	static int MouseX, MouseY = 0;	// 마우스 값
//	COLORREF Orgb;
//	COLORREF Hrgb;
//	GetClientRect(hwnd, &rect);
//
//	switch (iMsg)
//	{
//	case WM_CREATE:
//		img.Load(L"sky.png");
//
//		imgSprite[0].Load(L"AO1.png");	// 기본 모습
//		imgSprite[1].Load(L"AO2.png");
//		imgSprite[2].Load(L"Aright1.png");	// 오른쪽
//		imgSprite[3].Load(L"Aright2.png");
//		imgSprite[4].Load(L"Aright3.png");
//		imgSprite[5].Load(L"Aleft1.png");	// 왼쪽
//		imgSprite[6].Load(L"Aleft2.png");
//		imgSprite[7].Load(L"Aleft3.png");
//		imgSprite[8].Load(L"Ajump1.png");	// 점프
//		imgSprite[9].Load(L"Ajump2.png");
//
//		imgBumb.Load(L"bumb.png");
//
//		X[0] = 100, Y[0] = 100;
//
//		for (int i = 0; i < 6; i++) {
//			mw[i] = imgMonster[i].GetWidth();
//			mh[i] = imgMonster[i].GetHeight();
//		}
//
//		for (int i = 0; i < 4; i++) {
//			cw[i] = imgSprite[i].GetWidth();
//			ch[i] = imgSprite[i].GetHeight();
//		}
//
//		buw = imgBumb.GetWidth();
//		buh = imgBumb.GetHeight();
//		bw = imgGround.GetWidth();
//		bh = imgGround.GetHeight();
//		sw = img.GetWidth();
//		sh = img.GetHeight();
//
//		SetTimer(hwnd, 1, 50, NULL);
//		SetTimer(hwnd, 2, 10, NULL);
//		SetTimer(hwnd, 3, 1000, NULL);
//		SetTimer(hwnd, 4, 100, NULL);
//		SetTimer(hwnd, 7, 100, NULL);
//		break;
//
//	case WM_PAINT:
//		hdc = BeginPaint(hwnd, &ps);
//
//		if (jump != 0) {		// 점프 키 눌릴 때
//			for (int i = 0; i < jump; i++) {
//				CharY -= 3;
//				CharX += 1;
//			}
//			if (jump == 5) {
//				KillTimer(hwnd, 8);
//				SetTimer(hwnd, 9, 100, NULL);
//				jump = 0;
//			}
//		}
//
//		if (jumpdown != 0) {	// 점프했다가 다시 다운할 때
//			for (int i = 0; i < jumpdown; i++) {
//				CharY += 3;
//				CharX += 1;
//				if (CharY >= 520) {
//					KillTimer(hwnd, 9);
//					jumpdown = 0;
//				}
//			}
//			if (jumpdown == 5) {
//				KillTimer(hwnd, 9);
//				jumpdown = 0;
//			}
//		}
//
//		hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
//		mem1dc = CreateCompatibleDC(hdc);
//		SelectObject(mem1dc, hBitmap);
//
//		img.Draw(mem1dc, Xsky, 0, rect.right, rect.bottom, 0, 0, sw, sh);
//		img.Draw(mem1dc, Xsky2, 0, rect.right, rect.bottom, 0, 0, sw, sh);
//		imgGround.Draw(mem1dc, Xground, 100, rect.right, rect.bottom, 0, 0, bw, bh);
//		imgGround.Draw(mem1dc, Xground2, 100, rect.right, rect.bottom, 0, 0, bw, bh);
//
//		for (int i = 0; i < monsterNum; i++) {
//			imgMonster[mcount].Draw(mem1dc, Xmonster[i], Ymonster[i], mw[mcount], mh[mcount], 0, 0, mw[mcount], mh[mcount]);
//		}
//
//		if (down == 0)
//			imgSprite[count].Draw(mem1dc, CharX, CharY, cw[count] * 3 / 4, ch[count] * 3 / 4, 0, 0, cw[count], ch[count]);
//		else if (down == 1)
//			imgSprite[count].Draw(mem1dc, CharX, CharY + ch[count] * 3 / 4 - 50, cw[count] * 3 / 4, 50, 0, 0, cw[count], ch[count]);
//
//		for (int i = 0; i < monsterNum; i++) {
//			if (down == 0) {
//				if (CharX >= Xmonster[i] && CharX <= Xmonster[i] + mw[mcount] && CharY + ch[count] * 3 / 4 >= Ymonster[i] && CharY <= Ymonster[i] + mh[mcount]) {
//					imgBumb.Draw(mem1dc, CharX, CharY, cw[count], ch[count], 0, 0, buw, buh);
//				}
//			}
//			else if (down == 1) {
//				if (CharX >= Xmonster[i] && CharX <= Xmonster[i] + mw[mcount] && CharY + ch[count] * 3 / 4 - 50 >= Ymonster[i] && CharY + ch[count] * 3 / 4 <= Ymonster[i] + mh[mcount]) {
//					imgBumb.Draw(mem1dc, CharX, CharY, cw[count], ch[count], 0, 0, buw, buh);
//				}
//			}
//		}
//
//		BitBlt(hdc, 0, 0, rect.right, rect.bottom, mem1dc, 0, 0, SRCCOPY);
//		DeleteObject(hBitmap);
//		DeleteDC(mem1dc);
//		EndPaint(hwnd, &ps);
//		break;
//
//	case WM_LBUTTONDOWN:
//		hdc = GetDC(hwnd);
//		MouseX = LOWORD(lParam);
//		MouseY = HIWORD(lParam); -
//			ReleaseDC(hwnd, hdc);
//		break;
//
//	case WM_LBUTTONUP:
//		SetTimer(hwnd, 6, 1000, NULL);
//		break;
//
//	case WM_RBUTTONDOWN:
//		InvalidateRect(hwnd, NULL, FALSE);
//		break;
//	case WM_RBUTTONUP:
//		InvalidateRect(hwnd, NULL, FALSE);
//		break;
//
//	case WM_KEYDOWN:
//		switch (wParam) {
//		case VK_LEFT:
//			CharX -= 10;
//			break;
//		case VK_RIGHT:
//			CharX += 10;
//			break;
//		case 'J':
//			jump = 0;
//			SetTimer(hwnd, 8, 100, NULL);
//			break;
//		case 'K':
//			down = 1;
//			SetTimer(hwnd, 10, 500, NULL);
//			break;
//		case 'Q':
//			PostQuitMessage(0);
//			break;
//		}
//		InvalidateRect(hwnd, NULL, FALSE);
//		break;
//
//	case WM_TIMER:
//		switch (wParam) {
//		case 1:
//			Xground -= 10;
//			if (Xground - 10 < rect.left - rect.right) {
//				Xground = rect.left;
//			}
//			Xground2 = Xground + rect.right;
//			break;
//		case 2:
//			Xsky -= 10;
//			if (Xsky - 10 < rect.left - rect.right) {
//				Xsky = rect.left;
//			}
//			Xsky2 = Xsky + rect.right;
//			break;
//		case 3:
//			if (Rand % 2 == 0) {
//				Xmonster[monsterNum] = rect.right + 50;
//				if (monsterNum == 0) {
//					monsterNum++;
//				}
//			}
//			else {
//				Xmonster[monsterNum] = rect.right + 50;
//				Ymonster[monsterNum] = 450;
//				if (monsterNum == 0) {
//					monsterNum++;
//				}
//			}
//			SetTimer(hwnd, 5, 50, NULL);
//			break;
//		case 4:
//			if (mcount < 5)
//				mcount++;
//			else if (mcount == 5)
//				mcount = 0;
//			break;
//		case 5:
//			for (int i = 0; i < monsterNum; i++) {
//				Xmonster[i] -= 20;
//				if (Xmonster[i] < rect.left - mw[mcount]) {
//					monsterNum--;
//					Rand += 3;
//				}
//			}
//			break;
//		case 6:
//			Click = 0;
//			break;
//		case 7:
//			if (count < 3)
//				count++;
//			else if (count == 3)
//				count = 0;
//			break;
//		case 8:
//			jump++;
//			jumpdown = 0;
//			break;
//		case 9:
//			jumpdown++;
//			break;
//		case 10:
//			down = 0;
//			KillTimer(hwnd, 10);
//		}
//		InvalidateRect(hwnd, NULL, FALSE);
//		break;
//
//	case WM_DESTROY:
//		KillTimer(hwnd, 1);
//		KillTimer(hwnd, 2);
//		PostQuitMessage(0);
//		break;
//	}
//	return DefWindowProc(hwnd, iMsg, wParam, lParam);
//}
#pragma once
#include "global.h"
#include <atlImage.h>

class Button
{
private:
	int		iWidth = 337;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 97;	// ���� ũ��� sprite ������ ���̶� ������ ��

public:
	CImage* myImage;

	int		iXpos;	// POS ���� draw �Լ��� �� ���� ��� int, int �� ��������
	int		iYpos;

public:
	int GetXPos() { return iXpos; }
	void SetXPos(int in) { iXpos = in; }

	int GetYPos() { return iYpos; }
	void SetYPos(int in) { iYpos = in; }

	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

public:
	//������
	Button();
	Button(int xpos, int ypos, CImage* img);

	~Button();
};





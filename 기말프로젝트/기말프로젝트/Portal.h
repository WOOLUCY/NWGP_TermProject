#pragma once
#include "global.h"
#include <atlImage.h>

class Portal
{
private:
	int		iWidth = 500;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 800;	// ���� ũ��� sprite ������ ���̶� ������ ��

	RECT	aabb;

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

	RECT GetAABB() { return aabb; }
	void SetAABB(RECT in) { aabb = in; }

public:
	//������
	Portal();
	Portal(int xpos, int ypos, CImage* img);

	~Portal();
};





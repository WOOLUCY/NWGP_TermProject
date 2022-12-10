#pragma once
#include "global.h"
#include <atlImage.h>

class Portal
{
public:
	int		iWidth = 182;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 206;	// ���� ũ��� sprite ������ ���̶� ������ ��

	RECT	aabb;
	
	bool	bIsCrush;

	struct SendData {
		int		iXpos;
		int		iYpos;
		bool	bIsCrush;
		RECT	aabb;
	};

	Portal::SendData send;

public:
	CImage* myImage;

	int		iXpos;	// POS ���� draw �Լ��� �� ���� ��� int, int �� ��������
	int		iYpos;

	USHORT	uSpriteX;
	USHORT	uSpriteY;

public:
	int GetXPos() { return iXpos; }
	void SetXPos(int in) { iXpos = in; }

	int GetYPos() { return iYpos; }
	void SetYPos(int in) { iYpos = in; }

	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

	RECT GetAABB() { return aabb; }
	void SetAABB(RECT in) { aabb = in; }


	USHORT GetSpriteX() { return uSpriteX; }
	void SetSpriteX(USHORT in) { uSpriteX = in; }
	void AddSpriteX() { uSpriteX++; }

	USHORT GetSpriteY() { return uSpriteY; }
	void SetSpriteY(USHORT in) { uSpriteY = in; }



public:
	//������
	Portal();
	Portal(int xpos, int ypos);

	~Portal();

	void ChangeSprite(int* count);
};





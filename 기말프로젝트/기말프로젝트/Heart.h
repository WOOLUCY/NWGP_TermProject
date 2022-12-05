#pragma once
#pragma once
#include "global.h"
#include <atlImage.h>

class Heart
{
private:
	int		iWidth = 256;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 243;	// ���� ũ��� sprite ������ ���̶� ������ ��

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

	USHORT GetSpriteX() { return uSpriteX; }
	void SetSpriteX(USHORT in) { uSpriteX = in; }
	void AddSpriteX() { uSpriteX++; }

	USHORT GetSpriteY() { return uSpriteY; }
	void SetSpriteY(USHORT in) { uSpriteY = in; }

public:
	//������
	Heart();
	Heart(int xpos, int ypos, CImage* img);

	~Heart();

	void ChangeSprite(int* count);

};





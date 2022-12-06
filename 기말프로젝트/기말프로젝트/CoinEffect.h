#pragma once
#pragma once
#include "global.h"
#include <atlImage.h>

class CoinEffect
{
private:
	int		iWidth = 40;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 32;	// ���� ũ��� sprite ������ ���̶� ������ ��
	bool	bDone = FALSE;

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

	bool GetDone() { return bDone; }
	void SetDone(bool in) { bDone = in; }

public:
	//������
	CoinEffect();
	CoinEffect(int xpos, int ypos, CImage* img);

	~CoinEffect();

	void ChangeSprite(int* count);

};





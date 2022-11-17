#pragma once
#include "global.h"
#include <atlImage.h>



class Coin
{
private:
	int		iWidth = 64;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 64;	// ���� ũ��� sprite ������ ���̶� ������ ��
	int		iRange;

	RECT	aabb;

	int		iMaxX;
	int		iMinX;


public:
	CImage* myImage; //�̰ŷ� ��������Ʈ ����Ű�� �ɰͰ����� 
	// ���� ������ �� ���� ���� ���ص� ��� ���� �ֵ��� public ���� ��
	USHORT	uSpriteX = 0;
	USHORT	uSpriteY = 0;
	int		iXpos;	// POS ���� draw �Լ��� �� ���� ��� int, int �� ��������
	int		iYpos;

	bool	bIsCrush = false;


public:

	USHORT GetSpriteX() { return uSpriteX; }
	void SetSpriteX(USHORT in) { uSpriteX = in; }
	void AddSpriteX() { uSpriteX++; }

	USHORT GetSpriteY() { return uSpriteY; }
	void SetSpriteY(USHORT in) { uSpriteY = in; }

	int GetXPos() { return iXpos; }
	void SetXPos(int in) { iXpos = in; }

	int GetYPos() { return iYpos; }
	void SetYPos(int in) { iYpos = in; }


	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

	RECT GetAABB() { return aabb; }
	void SetAABB(RECT in) { aabb = in; }

	int GetRange() { return iRange; }
	void SetRange(int in) { iRange = in; }

public:
	//������
	Coin();
	Coin(int xpos, int ypos );

	~Coin();

	void ChangeSprite();
	void UpdateCoin();
};





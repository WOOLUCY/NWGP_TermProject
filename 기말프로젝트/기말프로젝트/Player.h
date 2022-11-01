#pragma once
#include "global.h"

#include <atlImage.h>

class Player
{
private:
	wchar_t wId;
	USHORT	uSpriteX;
	USHORT	uSpriteY;

	int		iWidth = 170;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 148;	// ���� ũ��� sprite ������ ���̶� ������ ��

	POS		pVel;
	USHORT	MaxJump;		// ��Ű�� �ִ� ������

	USHORT	sHeart; 
	USHORT	uCoin;

	bool	bFind;
	RECT	aabb;

		
public:
	CImage*	myImage[4]; //�̰ŷ� ��������Ʈ ����Ű�� �ɰͰ����� 
	// ���� ������ �� ���� ���� ���ص� ��� ���� �ֵ��� public ���� ��
	USHORT	uCharnum;
	int		iXpos;	// POS ���� draw �Լ��� �� ���� ��� int, int �� ��������
	int		iYpos;
	USHORT	JumpHeight;	// ���� �����ϰ� �ִ� ��ġ�� ������ �ߴµ�... AABB ���� �ʿ� ���� �� ����
	
public:
	wchar_t GetId() { return wId; }
	void SetId(wchar_t in) { wId = in; }

	USHORT GetSpriteX() { return uSpriteX; }
	void SetSpriteX(USHORT in) { uSpriteX = in; }
	void AddSpriteX() { uSpriteX++; }

	USHORT GetSpriteY() { return uSpriteY; }
	void SetSpriteY(USHORT in) { uSpriteY = in; }

	int GetXPos() { return iXpos; }
	void SetXPos(int in) { iXpos = in; }

	int GetYPos() { return iYpos; }
	void SetYPos(int in) { iYpos = in; }

	POS GetVel() { return pVel; }
	void SetVel(POS in) { pVel = in; }

	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

	USHORT GetHeart() { return sHeart; }
	void SetHeart(USHORT in) { sHeart = in; }

	USHORT GetCoin() { return uCoin; }
	void SetCoin(USHORT in) { uCoin = in; }

	bool GetFind() { return bFind; }
	void SetFind(bool in) { bFind = in; }

	RECT GetAABB() { return aabb; }
	void SetAABB(RECT in) { aabb = in; }

	wchar_t GetMaxJump() { return MaxJump; }
	void SetMaxJump(USHORT in) { MaxJump = in; }

public: 
	//������
	Player();
	Player(wchar_t id, USHORT sprite,USHORT charnum,POS position, POS Vel, USHORT heart, USHORT coin, bool find);
	
	~Player();

	void Move(POS Direction); //����,rect�� ���� �����°ŷ� �ұ�??
	void Jump();
	void ChangeSprite();
};




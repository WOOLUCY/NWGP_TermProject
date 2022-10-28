#pragma once
#include "global.h"


class Player
{
private:
	wchar_t wId;
	USHORT	uSprite;
	USHORT	uCharnum;

	POS		pPosition;
	POS		pVel;

	USHORT	sHeart; 
	USHORT	uCoin;

	bool	bFind;
	RECT	aabb;
		
public:
	CImage*	myImage[4]; //�̰ŷ� ��������Ʈ ����Ű�� �ɰͰ����� 
	
public:

	wchar_t GetId() { return wId; }
	void SetId(wchar_t in) { wId = in; }

	USHORT GetSprite() { return uSprite; }
	void SetSprite(USHORT in) { uSprite = in; }


	POS GetPosition() { return pPosition; }
	void SetPosition(POS in) { pPosition = in; }

	POS GetVel() { return pVel; }
	void SetVel(POS in) { pVel = in; }


	USHORT GetHeart() { return sHeart; }
	void SetHeart(USHORT in) { sHeart = in; }

	USHORT GetCoin() { return uCoin; }
	void SetCoin(USHORT in) { uCoin = in; }

	bool GetFind() { return bFind; }
	void SetFind(bool in) { bFind = in; }

	RECT GetAABB() { return aabb; }
	void SetAABB(RECT in) { aabb = in; }

public: 
	//������
	Player();
	Player(wchar_t id, USHORT sprite,USHORT charnum,POS position, POS Vel, USHORT heart, USHORT coin, bool find);
	

	~Player();

	void Move();//����,rect�� ���� �����°ŷ� �ұ�??
	void Jump();
	


};




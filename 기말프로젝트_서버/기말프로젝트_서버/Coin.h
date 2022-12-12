#pragma once
#include <windows.h>


class Coin
{
public:

	struct SendData {
		//int		iXpos;
		//int		iYpos;
		bool	bIsCrush;
		USHORT	uSpriteX = 0;
		USHORT	uSpriteY = 0;
		RECT	aabb;

	};


	//��ġ 
	int		iXpos;
	int		iYpos;

//	RECT	aabb; //�浹ó���� �������� �ϴϱ� �ϴ� ����

	bool	bIsCrush = FALSE;//�浹�Ǿ����� �ƴ��� ���� �̸� �ٲ㵵��

	int		iWidth = 30;
	int		iHeight = 30;

	Coin::SendData send;

public:
	Coin();
	Coin(int x, int y);


	RECT GetAABB() { return send.aabb; }
	void SetAABB(RECT in) { send.aabb = in; }
	void SetIsCrush(bool in) { send.bIsCrush = in; }
	void CoinUpdate();

};

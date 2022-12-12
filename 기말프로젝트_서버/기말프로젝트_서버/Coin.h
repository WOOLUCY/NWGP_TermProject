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


	//위치 
	int		iXpos;
	int		iYpos;

//	RECT	aabb; //충돌처리도 서버에서 하니까 일단 넣음

	bool	bIsCrush = FALSE;//충돌되었는지 아닌지 변수 이름 바꿔도됨

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

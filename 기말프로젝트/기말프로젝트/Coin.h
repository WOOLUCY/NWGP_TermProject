#pragma once
#include "global.h"
#include <atlImage.h>


class Coin
{
public:
	CImage*	 myImage;
	bool	 bIs = false; //안먹혔는지 충돌했는지

	//크기 아직 안정해졋나?
	int		iWidth=556;
	int		iHeight=470;

	//위치
	int		iXpos;
	int		iYpos;

	//스프라이트 크기
	USHORT	uSpriteCount = 0;

public:

	Coin() { iXpos = 0; iYpos = 0; }
	Coin(int x, int y) { iXpos = x; iYpos = y; }
	Coin(float x, float y) { iXpos = x; iYpos = y; }

	void ChangeSprite();


	


};


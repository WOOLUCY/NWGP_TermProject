#pragma once
#include "global.h"
#include <atlImage.h>


class Coin
{
public:
	CImage*	 myImage;
	bool	 bIs = false; //�ȸ������� �浹�ߴ���

	//ũ�� ���� �����ؠ���?
	int		iWidth=556;
	int		iHeight=470;

	//��ġ
	int		iXpos;
	int		iYpos;

	//��������Ʈ ũ��
	USHORT	uSpriteCount = 0;

public:

	Coin() { iXpos = 0; iYpos = 0; }
	Coin(int x, int y) { iXpos = x; iYpos = y; }
	Coin(float x, float y) { iXpos = x; iYpos = y; }

	void ChangeSprite();


	


};


#pragma once
#include <windows.h>


class Coin
{
public:

	struct SendData {
		int		iXpos;
		int		iYpos;
		bool	bIsCrush;

	};


	//��ġ 
	int		iXpos;
	int		iYpos;

	RECT	aabb; //�浹ó���� �������� �ϴϱ� �ϴ� ����

	bool	bIsCrush = false;//�浹�Ǿ����� �ƴ��� ���� �̸� �ٲ㵵��

	int		iWidth = 600;
	int		iHeight = 60;

	Coin::SendData send;

public:
	Coin();
	Coin(int x, int y);


};

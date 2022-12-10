#pragma once
#include "global.h"
#include <atlImage.h>

class Key
{
public:
	int		iWidth = 163;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 148;	// ���� ũ��� sprite ������ ���̶� ������ ��

	bool	bIsCrush = false;//�浹�Ǿ����� �ƴ��� ���� �̸� �ٲ㵵��
	RECT	aabb;

	struct SendData {
		int		iXpos;
		int		iYpos;
		bool	bIsCrush;
		RECT	aabb;
		bool	bIsVisible;
	};

	Key::SendData send;


public:
	CImage* myImage;

	int		iXpos;	// POS ���� draw �Լ��� �� ���� ��� int, int �� ��������
	int		iYpos;

public:
	int GetXPos() { return iXpos; }
	void SetXPos(int in) { iXpos = in; }

	int GetYPos() { return iYpos; }
	void SetYPos(int in) { iYpos = in; }

	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

	RECT GetAABB() { return aabb; }
	void SetAABB(RECT in) { aabb = in; }

public:
	//������
	Key();
	Key(int xpos, int ypos);

	~Key();
};





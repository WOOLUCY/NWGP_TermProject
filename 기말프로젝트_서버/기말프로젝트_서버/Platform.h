#pragma once

#include <windows.h>



class Platform
{
public://�ϴ� �ۺ����� �ϰڽ��ϴ�

	struct SendData {
		int		iXpos;
		int		iYpos;
		bool	bIsCrush;
		RECT	aabb;
	};

	//��ġ 
	int		iXpos;
	int		iYpos;

	//RECT	aabb; //�浹ó���� �������� �ϴϱ� �ϴ� ����

	bool	bIsCrush = false;//�浹�Ǿ����� �ƴ��� ���� �̸� �ٲ㵵��

	int		iWidth = 600;
	int		iHeight = 60;
	
	Platform::SendData send;

	

public:

	Platform();
	Platform(int x, int y);

	RECT GetAABB() { return send.aabb; }
	void SetAABB(RECT in) { send.aabb = in; }
};


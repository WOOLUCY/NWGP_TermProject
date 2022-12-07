#pragma once
#include <atlImage.h>

//���� - ������ ���� Ŭ�����Ӵϴ�. 





class Platform
{
private:
	int		iWidth = 600;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 60;	// ���� ũ��� sprite ������ ���̶� ������ ��
	int		iRange;

	RECT	aabb;

	int		iMaxX;
	int		iMinX;

	
public:
	struct SendData {
		int		iXpos;
		int		iYpos;
		bool	bIsCrush;
		RECT	aabb;
	};

	CImage* myImage;//; //�̰ŷ� ��������Ʈ ����Ű�� �ɰͰ����� 
	// ���� ������ �� ���� ���� ���ص� ��� ���� �ֵ��� public ���� ��
	int		iXpos;	// POS ���� draw �Լ��� �� ���� ��� int, int �� ��������
	int		iYpos;

	bool	bIsCrush = false;
	


public:

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
	Platform();
	Platform(int xpos, int ypos,CImage* img);

	~Platform();

};




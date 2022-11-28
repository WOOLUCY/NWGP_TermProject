#pragma once
#include "global.h"
#include <atlImage.h>

class Portal
{
private:
	int		iWidth = 500;	// draw 에 필요한 width, height 추가
	int		iHeight = 800;	// 고정 크기로 sprite 제작할 것이라서 고정해 둠

	RECT	aabb;

public:
	CImage* myImage;

	int		iXpos;	// POS 형을 draw 함수에 쓸 수가 없어서 int, int 로 나누었음
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
	//생성자
	Portal();
	Portal(int xpos, int ypos, CImage* img);

	~Portal();
};





#pragma once
#include "global.h"
#include <atlImage.h>

class Button
{
private:
	int		iWidth = 337;	// draw 에 필요한 width, height 추가
	int		iHeight = 97;	// 고정 크기로 sprite 제작할 것이라서 고정해 둠

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

public:
	//생성자
	Button();
	Button(int xpos, int ypos, CImage* img);

	~Button();
};





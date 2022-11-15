#pragma once
#include "global.h"
#include <atlImage.h>

//가온 - 발판을 위한 클래스임니다. 
class Platform
{
public:
	CImage* myImage;
	
	//위치
	int iXpos;
	int iYpos;

	RECT aabb; // 충돌검사시 필요할수도 잇으니까 넣어놈

	

	
};


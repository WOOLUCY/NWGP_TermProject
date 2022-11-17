#include "Platform.h"

Platform::Platform()
	: iXpos(340), iYpos(600), iRange(60)

{
	myImage = nullptr;

	iMaxX = iXpos + iRange;
	iMinX = iXpos - iRange;

	//일단 걍다 0으로 초기화함 

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}

Platform::Platform(int xpos, int ypos)
{
		myImage = nullptr;

	iXpos = xpos;
	iYpos = ypos;

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}



Platform::~Platform()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}




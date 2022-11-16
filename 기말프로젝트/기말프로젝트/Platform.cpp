#include "Platform.h"

Platform::Platform()
	:uSpriteX(0), uSpriteY(0), iXpos(340), iYpos(625), iRange(60)

{
	iMaxX = iXpos + iRange;
	iMinX = iXpos - iRange;

	//일단 걍다 0으로 초기화함 

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}



Platform::~Platform()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}




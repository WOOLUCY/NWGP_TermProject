#include "Portal.h"


#include "global.h"

Portal::Portal()
	:iXpos(100), iYpos(625)
{
	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}

Portal::Portal(int xpos, int ypos, CImage* img)
{
	myImage = img;

	iXpos = xpos;
	iYpos = ypos;

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}

Portal::~Portal()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}

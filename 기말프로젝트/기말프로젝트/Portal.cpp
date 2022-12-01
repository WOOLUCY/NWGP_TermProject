#include "Portal.h"


#include "global.h"

Portal::Portal()
	:iXpos(1000), iYpos(500)
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

void Portal::ChangeSprite(int* count)
{
	if (*count == 2) {
		uSpriteX = (uSpriteX + 1) % 16;
		*count = 0;
	}
	*count += 1;
}

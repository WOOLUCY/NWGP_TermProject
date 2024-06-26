#include "Portal.h"


#include "global.h"

Portal::Portal()
	:iXpos(1000), iYpos(500)
{
	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;

	send.aabb.bottom = aabb.bottom;
	send.aabb.left = aabb.left;
	send.aabb.right = aabb.right;
	send.aabb.top = aabb.top;

	send.iXpos = iXpos;
	send.iYpos = iYpos;
	send.bIsCrush = bIsCrush;
}

Portal::Portal(int xpos, int ypos, CImage* img)
	:iXpos(xpos), iYpos(ypos), myImage(img)
{
	send.iXpos = iXpos;
	send.iYpos = iYpos;

	send.aabb.bottom = iYpos + (iHeight / 2);
	send.aabb.left = iXpos;
	send.aabb.right = iXpos + (iWidth / 2);
	send.aabb.top = iYpos + 3;

	send.bIsCrush = bIsCrush;
	printf("portal aabb: %d, %d, %d, %d\n", send.aabb.left, send.aabb.right,
		send.aabb.top, send.aabb.bottom);
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

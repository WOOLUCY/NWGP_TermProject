#include "Key.h"
#include "global.h"

Key::Key()
	:iXpos(300), iYpos(300)
{
	send.bIsVisible = FALSE;
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

Key::Key(int xpos, int ypos)
	:iXpos(xpos), iYpos(ypos)
{
	send.bIsVisible = FALSE;
	send.iXpos = iXpos;
	send.iYpos = iYpos;

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;

	send.aabb.bottom = aabb.bottom;
	send.aabb.left = aabb.left;
	send.aabb.right = aabb.right;
	send.aabb.top = aabb.top;

	send.bIsCrush = bIsCrush;
	//printf("key aabb: %d, %d, %d, %d\n", send.aabb.left, send.aabb.right, send.aabb.top, send.aabb.bottom);
}

Key::~Key()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}
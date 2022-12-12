#include "Platform.h"
#include <stdio.h>

Platform::Platform()
	:iXpos(0), iYpos(0)

{
	send.aabb.bottom = iYpos + (iHeight / 2);
	send.aabb.left = iXpos;
	send.aabb.right = iXpos + (iWidth / 2);
	send.aabb.top = iYpos + 3;


	send.iXpos = iXpos;
	send.iYpos = iYpos;
	send.bIsCrush = bIsCrush;

}

Platform::Platform(int x, int y) 
	:iXpos(x),iYpos(y)
{
	send.iXpos = iXpos;
	send.iYpos = iYpos;

	send.aabb.bottom = iYpos + (iHeight / 2);
	send.aabb.left = iXpos;
	send.aabb.right = iXpos + (iWidth / 2);
	send.aabb.top = iYpos + 3;

	send.bIsCrush = bIsCrush;
}

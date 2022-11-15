#include "Coin.h"

void Coin::ChangeSprite()
{
	//스프라이트 돌리는 함수
	
	++uSpriteCount;
	if (uSpriteCount == 4) {
		uSpriteCount = 0;
	}
	
}


////일단 플레이어랑 똑같이함 
//void Coin::ChangeSprite(t)
//{
//	if (*count == 3) {
//		uSpriteX = (uSpriteX + 1) % 4;
//		*count = 0;
//	}
//	*count += 1;
//}

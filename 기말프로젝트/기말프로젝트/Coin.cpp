#include "Coin.h"

void Coin::ChangeSprite()
{
	//��������Ʈ ������ �Լ�
	
	++uSpriteCount;
	if (uSpriteCount == 4) {
		uSpriteCount = 0;
	}
	
}


////�ϴ� �÷��̾�� �Ȱ����� 
//void Coin::ChangeSprite(t)
//{
//	if (*count == 3) {
//		uSpriteX = (uSpriteX + 1) % 4;
//		*count = 0;
//	}
//	*count += 1;
//}

#include "Monster.h"


Monster::Monster()
	:pPosition(0, 0), pVel(0, 0), uSprite(0),iHp(0),bAlive(true)
{


	//���ʱ�ȭ ������...

	aabb.bottom = 0;
	aabb.left = 0;
	aabb.right = 0;
	aabb.top = 0;

	myImage[0] = nullptr;
	myImage[1] = nullptr;

	++iTotal;


}

Monster::Monster(POS pos,POS vel, USHORT sprite, UINT hp, bool alive, RECT aabb)
	:pPosition(pos), pVel(vel), uSprite(0), iHp(hp), bAlive(true)
{

	//�� ���� ũ�� �־��ָ�ɵ� 
	aabb.bottom = 0;
	aabb.left = 0;
	aabb.right = 0;
	aabb.top = 0;

	myImage[0] = nullptr;
	myImage[1] = nullptr;

	++iTotal;

}


void Monster::update()
{
	//���⼭ ������ ����+��Ʈ
		//���⼭ ���� ������ �����ϸ� �ɵ�, ������ �����ϸ鼭 aabb������ �ٲ�����ҵ�
	POS temp = GetPosition();

	temp.x += 0.1f; //y�� �ȿ����̴ϱ� �ѱ�

	RECT rTemp{ temp.x, temp.y,temp.x + 1,temp.y + 1 };

	//���� �� ������ �̷������� �ѱ�� �ɵ�??
	SetAABB(rTemp);
	SetPosition(temp);

	
	//���⼭ �ǰ�ó���ϸ�ɵ�??






}

#include "Monster.h"


Monster::Monster()
	:pPosition(0, 0), pVel(0, 0), uSprite(0),iHp(0),bAlive(true)
{


	//아초기화 기찮다...

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

	//그 몬스터 크기 넣어주면될듯 
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
	//여기서 움직임 구현+렉트
		//여기서 몬스터 움직임 구현하면 될듯, 움직임 구현하면서 aabb범위도 바꿔줘야할듯
	POS temp = GetPosition();

	temp.x += 0.1f; //y는 안움직이니까 넘김

	RECT rTemp{ temp.x, temp.y,temp.x + 1,temp.y + 1 };

	//숫자 다 예시임 이런식으로 넘기면 될듯??
	SetAABB(rTemp);
	SetPosition(temp);

	
	//여기서 피격처리하면될듯??






}

#pragma once
#include "global.h"
#include <ctime>
#include "Player.h"
#include "CMonster.h"

using namespace std;
/* ------------------------- ���� ��ſ� ------------------------- */

struct ClientToServer {
	 wchar_t		 wId[21] = { '\0' };
	 USHORT          uCharNum = 10;
	 KeyInput        Input = { 0 };
	 KeyInput        Up = { 0 };
};

struct ServerToClient {
	double          ServerTime;	 // semin, clock_t -> double �ڷ��� �ٲ�
	bool			bGameEnd = FALSE;
	bool            bWin;
	bool            bIsPlaying = FALSE;
	

	Player::SendData player[3];
	CMonster::SendData monsters[MONSTERNUM];
	Coin::SendData coins[COINNUM];


};


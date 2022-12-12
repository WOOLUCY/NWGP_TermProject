#pragma once
#include "global.h"
#include <ctime>
#include <vector>
#include "Player.h"
using namespace std;
/* ------------------------- ���� ��ſ� ------------------------- */

struct ClientToServer {
	 wchar_t		 wId[21] = { '\0' };
	 USHORT          uCharNum = 0;
	 KeyInput        Input = { 0 };
	 KeyInput        Up = { 0 };
};

struct ServerToClient {
	double          ServerTime;	 // semin, clock_t -> double �ڷ��� �ٲ�
	bool			bGameEnd = FALSE;
	bool            bWin;
	bool            bIsPlaying = FALSE;
	int				iTotalCoinNum;


	Player::SendData player[3];
	CMonster::SendData monsters[MONSTERNUM];
	Coin::SendData coins[COINNUM];
	Key::SendData key;
	Portal::SendData portal;
};

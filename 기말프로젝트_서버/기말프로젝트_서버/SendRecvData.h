#pragma once
#include <ctime>
// #include <vector>
#include "Player.h"
using namespace std;
/* ------------------------- ���� ��ſ� ------------------------- */

struct RecvPlayerData {
	Player			pPlayer = {};
	// USHORT          uCharNum = 0;
	// KeyInput        Input = { 0 };
};

struct SendGameData {
	clock_t         ServerTime;
	bool            bWin;
	bool            bIsPlaying;

	// vector<CMonster> monsters;
	// vector<Coin>    coins;
};
//
//struct PlayerMgr {
//	DWORD     portnum;
//	Player    player;
//};

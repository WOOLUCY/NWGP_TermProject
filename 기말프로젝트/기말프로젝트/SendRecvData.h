#pragma once
#include <ctime>
#include <vector>
#include "Player.h"
using namespace std;
/* ------------------------- 서버 통신용 ------------------------- */

struct ClientToServer {
	 wchar_t		 wId[21];
	 USHORT          uCharNum = 0;
	 KeyInput        Input = { 0 };
};

struct ServerToClient {
	clock_t         ServerTime;
	bool            bWin;
	bool            bIsPlaying;


	Player::SendData player[3];

};

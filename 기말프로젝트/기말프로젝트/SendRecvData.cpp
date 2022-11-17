#include <ctime>
#include <vector>
#include "CMonster.h"
#include "Coin.h"
#include "Player.h"
using namespace std;
/* ------------------------- 서버 통신용 ------------------------- */


struct SendPlayerData {
	USHORT          uID;
	USHORT          uCharNum;
	KeyInput        Input;
};

struct RecvGameData {
	clock_t         ServerTime;
	bool            bWin;
	bool            bIsPlaying;

	vector<CMonster> monsters;
	vector<Coin>    coins;
};

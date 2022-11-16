#include <ctime>
#include <vector>
#include "Monster.h"
#include "Coin.h"
#include "Player.h"
using namespace std;
/* ------------------------- ���� ��ſ� ------------------------- */


struct SendPlayerData {
	USHORT          uID;
	USHORT          uCharNum;
	KeyInput        Input;
};

struct RecvGameData {
	clock_t         ServerTime;
	bool            bWin;
	bool            bIsPlaying;

	vector<Monster> monsters;
	vector<Coin>    coins;
};

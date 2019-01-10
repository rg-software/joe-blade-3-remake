//---------------------------------------------------------------------------
#ifndef GlobalsH
#define GlobalsH

#include <vcl.h>
#include <SDL.h>
#include <vector>
#include <map>

using namespace std;

class Bullet;
class Room;
class JoeBlade;
//class JukeBox;
class SimpleEnemy;
class Smoke;

enum TGameStatus { MAINMENU, GAME, LIFTMENU, GAMEMENU, SECURITY_SUBGAME, COMPUTER_SUBGAME, CRAX_SUBGAME, GAMEOVER, GAMEVICTORY, EXIT };
//---------------------------------------------------------------------------
extern TGameStatus GameStatus;
extern int Score;
extern int DynamiteTimer;
extern bool DynamiteTimerEnabled;
extern int GameTimerTicks;

extern JoeBlade *Joe;
//extern JukeBox jukebox;
//extern vector<SimpleEnemy *> enemies;

extern map<AnsiString, Room *> Rooms;                  // ������ ������ (��� �������, �������)
extern map<int, int> Inventory;                          // "inventory" - <id_�������, ����������>
extern Bullet *bullet;                                  // ���� ��� ������ (�� ������ ����� ���� ������ ����, ��� � ���������)
extern Bullet *enemybullet;                                  
extern vector<Smoke *> SmokeObjects;

// objectives
extern bool CraxCaptured;
extern int CraxFloor;
extern int DynamitesRigged;
extern int HostagesFound;

extern char* objectives[];
extern char* curobjs[4];

struct HiEntry
{
    AnsiString name; int score;
    HiEntry() {}
    HiEntry(const AnsiString& n, int s) : name(n), score(s) {}
    bool operator<(const HiEntry& rhs) const { return score < rhs.score; }
};
extern vector<HiEntry> HiTable;

//---------------------------------------------------------------------------

#endif


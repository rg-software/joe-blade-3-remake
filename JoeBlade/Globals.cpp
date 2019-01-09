//---------------------------------------------------------------------------
#pragma hdrstop

#include "Globals.h"
#include "Bullet.h"
#include "JoeBlade.h"
#include "JukeBox.h"
#include "SimpleEnemy.h"
#include "Smoke.h"
#include <stdio.h>

#pragma package(smart_init)

using namespace std;

//---------------------------------------------------------------------------
int Score = 0;
int DynamiteTimer;
bool DynamiteTimerEnabled;
int GameTimerTicks;

TGameStatus GameStatus;
JoeBlade *Joe;
//JukeBox jukebox;
//vector<SimpleEnemy *> enemies(Config::MaxEnemies);

map<AnsiString, Room *> Rooms;
map<int, int> Inventory;
Bullet *bullet;
Bullet *enemybullet;
vector<Smoke *> SmokeObjects(Config::MaxEnemies);

bool CraxCaptured;
int CraxFloor;
int DynamitesRigged;
int HostagesFound;

char* objectives[] = { "now rescue all hostages", "now capture crax bloodfinger", "now rig all explosives", "now escape via the roof" };
char* curobjs[4];

vector<HiEntry> HiTable(Config::HiTableSize);
//---------------------------------------------------------------------------


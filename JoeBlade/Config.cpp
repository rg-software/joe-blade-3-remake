//---------------------------------------------------------------------------
#pragma hdrstop

#include "Config.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

namespace Config {

// системные настройки
const int TimerInterval = 1000 / 25;   // 25 FPS - стандартная скорость
const int TrackChangeInterval = 1000;
const int N_of_layers = 6;
const int SystemLayer = 5;
const int ObjectsLayer = 3;
const int N_of_slots = 7;
const int HiTableSize = 8;
const float ScrollZone = 1 / 2.0;	
	
// экран

const int GridSize = 32;
const int Xoffset = 64 - GridSize;
const int Yoffset = 121;
const int ColorKey[] = { 240, 0, 240 };
const int ScreenParams[] = { 640, 480, 16 };
const int StartX = 110;

// комната
const int RoomHeight = GridSize * 6;
const int RoomWidth = GridSize * 16 + GridSize * 1; // плюс системное поле слева
const int FloorCoord = RoomHeight - 20;
/*const int StartRoomX = 0;
const int StartRoomY = 0;
const int StartRoomZ = -1;*/
const int StartRoomX = -5;
const int StartRoomY = -2;
const int StartRoomZ = -2;

// панель статуса
const int StatusPanelX = (640 - 514) / 2;
const int StatusPanelY = 330;
const int BulletW = 4;
const int HeartW = 16;

// панель сообщений
const int MessagePanelY = 80;
char* FloorNames[] = { "", "", "basement level", "lower ground floor", "ground floor", "floor B", "floor C",
                       "floor D", "floor E", "floor F", "floor G", "floor H", "floor I", "floor J", "floor BA",
                       "floor BB", "floor BC", "floor BD", "floor BE", "floor BF", "floor BG", "floor BH",
                       "floor BI", "floor BJ", "floor CA", "floor CB", "floor CC", "floor CD", "floor CE",
                       "floor CF", "floor CG", "floor CH", "floor CI", "the roof", "" };

// спрайты
const int SpriteLeftMinFrame = 4;
const int SpriteLeftMaxFrame = 7;
const int SpriteRightMinFrame = 0;
const int SpriteRightMaxFrame = 3;

// время
const int MinTimeToWait = 2000;            // миллисекунды
const int MaxTimeToWait = 8000;
const int DoorInterval = 3000;
const int ChDirInterval = 3000;
const int BulletInterval = 5000;
const int MenuDelay = 170;
const int ShieldTime = 45000;
const unsigned MsgWaitTime = 5000;
const int PreGameOverDelay = 4000;
const int DynamiteTimerTime = 10 * 1000 * 60;

// объекты
const int MaxEnemies = 4;
const int MaxDistanceFromJoe = 2;
const int DoorEnterProb = 30;     // 30%
const int BulletProb = 30;     // 30%
const int ChDirProb = 20;         // 20%
const int LiftCardFloors[] = { 3, 7, 11, 15, 19, 23, 28 };
const int sizeofLCF = sizeof(LiftCardFloors);
const int LowerFloor = -2;
const int EnergyFactor = 10;
const int InitialEnergy = 12 * EnergyFactor;
const int EnergyPackSize = 6 * EnergyFactor;
const int InitialAmmo = 48;
const int AmmoPackSize = 24;
const int FloorDistance = 100;
const int ObjectiveItemScore = 5000;
const int ItemScore = 255;
const int CleverEnemyLowerFloor = 3;
const int ArmedEnemyLowerFloor = 10;
const int CraxFloorL = 24;
const int CraxFloorH = 26;               // CRAX COORDS ARE INITIALLY ALWAYS JOE_X, JOE_Y + 1

int DynamitesTotal;
int HostagesTotal;

// cheats
Uint32 SDL_ScrnFlag;
bool InfEnergy;
bool InfAmmo;
bool TimeStop;
bool AllLiftCards;
bool InfKeysDisks;

bool ShowFPS;
bool JukeBoxDisabled;
bool ScrollEnabled;
bool NoSound;
}


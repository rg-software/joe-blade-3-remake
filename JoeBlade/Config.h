//---------------------------------------------------------------------------

#ifndef ConfigH
#define ConfigH

#include <SDL.h>

//---------------------------------------------------------------------------
namespace Config {

// системные настройки
extern const int TimerInterval;        // интервал между прерываниями таймера (миллисекунды)
extern const int TrackChangeInterval;  // минимальное время между треками
extern const int N_of_layers;          // количество слоёв в каждом уровне
extern const int SystemLayer;          // номер системного слоя
extern const int ObjectsLayer;         // номер слоя с объектами (ключи, дискеты...)
extern const int N_of_slots;
extern const int HiTableSize;
extern const float ScrollZone;	

// экран
extern const int GridSize;             // шаг "сетки", т.е. минимальное расстояние между соседними тайлами
extern const int Xoffset;              // координаты левого верхнего угла
extern const int Yoffset;              // комнаты относительно угла экрана
extern const int ColorKey[];           // RGB значения прозрачного цвета
extern const int ScreenParams[];       // параметры экрана (ширина, высота, глубина)
extern const int StartX;               // стартовая координата Джо Блейда
extern SDL_Surface *BackBuffer;

// комната
extern const int RoomHeight;           // линейные
extern const int RoomWidth;            // размеры комнаты
extern const int FloorCoord;           // Y-координата пола комнаты (т.е. где размещаются спрайты героев)
extern const int StartRoomX;           // 3D координаты
extern const int StartRoomY;           // стартовой
extern const int StartRoomZ;           // комнаты

// панель статуса
extern const int StatusPanelX;         // координаты панели
extern const int StatusPanelY;         // статуса на экране
extern const int BulletW;
extern const int HeartW;

// панель сообщений
extern const int MessagePanelY;        // Y-координата панели сообщений (ширина панели - весь экран)
extern char* FloorNames[];

// спрайты
extern const int SpriteLeftMinFrame;   // номер первого кадра спрайта, движущегося налево
extern const int SpriteLeftMaxFrame;   // номер последнего кадра спрайта, движущегося налево
extern const int SpriteRightMinFrame;  // номер первого кадра спрайта, движущегося направо
extern const int SpriteRightMaxFrame;  // номер последнего кадра спрайта, движущегося направо

// время
extern const int MinTimeToWait;        // минимальный промежуток времени перед появлением нового врага
extern const int MaxTimeToWait;        // максимальный промежуток
extern const int DoorInterval;
extern const int ChDirInterval;
extern const int BulletInterval;
extern const int MenuDelay;
extern const int ShieldTime;
extern const unsigned MsgWaitTime;
extern const int PreGameOverDelay;
extern const int DynamiteTimerTime;

// объекты
extern const int MaxEnemies;
extern const int MaxDistanceFromJoe;
extern const int DoorEnterProb;
extern const int ChDirProb;
extern const int BulletProb;
extern const int LiftCardFloors[];
extern const int sizeofLCF;
extern const int LowerFloor;
extern const int EnergyFactor;
extern const int InitialEnergy;
extern const int EnergyPackSize;
extern const int InitialAmmo;
extern const int AmmoPackSize;
extern const int FloorDistance;
extern const int ObjectiveItemScore;
extern const int ItemScore;
extern const int CleverEnemyLowerFloor;
extern const int ArmedEnemyLowerFloor;
extern const int CraxFloorL;
extern const int CraxFloorH;

extern int DynamitesTotal;
extern int HostagesTotal;

// cheats
extern Uint32 SDL_ScrnFlag;
extern bool DoubleScreen;
extern bool InfEnergy;
extern bool InfAmmo;
extern bool TimeStop;
extern bool AllLiftCards;
extern bool InfKeysDisks;

extern bool ShowFPS;
extern bool JukeBoxDisabled;
extern bool ScrollEnabled;
extern bool NoSound;
}
//---------------------------------------------------------------------------
#endif

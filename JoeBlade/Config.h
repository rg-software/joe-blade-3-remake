//---------------------------------------------------------------------------

#ifndef ConfigH
#define ConfigH

#include <SDL.h>

//---------------------------------------------------------------------------
namespace Config {

// ��������� ���������
extern const int TimerInterval;        // �������� ����� ������������ ������� (������������)
extern const int TrackChangeInterval;  // ����������� ����� ����� �������
extern const int N_of_layers;          // ���������� ���� � ������ ������
extern const int SystemLayer;          // ����� ���������� ����
extern const int ObjectsLayer;         // ����� ���� � ��������� (�����, �������...)
extern const int N_of_slots;
extern const int HiTableSize;
extern const float ScrollZone;	

// �����
extern const int GridSize;             // ��� "�����", �.�. ����������� ���������� ����� ��������� �������
extern const int Xoffset;              // ���������� ������ �������� ����
extern const int Yoffset;              // ������� ������������ ���� ������
extern const int ColorKey[];           // RGB �������� ����������� �����
extern const int ScreenParams[];       // ��������� ������ (������, ������, �������)
extern const int StartX;               // ��������� ���������� ��� ������
extern SDL_Surface *BackBuffer;

// �������
extern const int RoomHeight;           // ��������
extern const int RoomWidth;            // ������� �������
extern const int FloorCoord;           // Y-���������� ���� ������� (�.�. ��� ����������� ������� ������)
extern const int StartRoomX;           // 3D ����������
extern const int StartRoomY;           // ���������
extern const int StartRoomZ;           // �������

// ������ �������
extern const int StatusPanelX;         // ���������� ������
extern const int StatusPanelY;         // ������� �� ������
extern const int BulletW;
extern const int HeartW;

// ������ ���������
extern const int MessagePanelY;        // Y-���������� ������ ��������� (������ ������ - ���� �����)
extern char* FloorNames[];

// �������
extern const int SpriteLeftMinFrame;   // ����� ������� ����� �������, ����������� ������
extern const int SpriteLeftMaxFrame;   // ����� ���������� ����� �������, ����������� ������
extern const int SpriteRightMinFrame;  // ����� ������� ����� �������, ����������� �������
extern const int SpriteRightMaxFrame;  // ����� ���������� ����� �������, ����������� �������

// �����
extern const int MinTimeToWait;        // ����������� ���������� ������� ����� ���������� ������ �����
extern const int MaxTimeToWait;        // ������������ ����������
extern const int DoorInterval;
extern const int ChDirInterval;
extern const int BulletInterval;
extern const int MenuDelay;
extern const int ShieldTime;
extern const unsigned MsgWaitTime;
extern const int PreGameOverDelay;
extern const int DynamiteTimerTime;

// �������
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

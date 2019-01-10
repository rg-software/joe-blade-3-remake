//---------------------------------------------------------------------------
#ifndef UtilsH
#define UtilsH

#include <SDL.h>
#include <vector>
#include <vcl.h>
#include "Sprite.h"

using namespace std;

//---------------------------------------------------------------------------
enum Objective { HOSTAGES_FOUND = 0, CRAX_CAPTURED = 1, DYNAMITE_RIGGED = 2 };

void ShowNextObjective(Objective obj);
void RenewObjectives();
bool ObjectivesPending();
void ClearRoomBox();
bool DefaultExitCondition();
void MyMessageBox(char *messages, void (*fun)() = 0, bool (*exitcond)() = DefaultExitCondition);
const char* GetFloorName(int floorNo);
const char* GetObjectMessage(int object);
SDL_Surface *LoadSurfaceFromGIFResource(char *gifname);
void InitializeNewGame();
int ShowMenu(const vector<AnsiString>& items, int min_idx, int max_idx);
void ClearRooms();
void LoadGame();
void SaveGame();
void ShowSplash();
void RemoveSplash();
void ShowInfo();
void LoadHiTable();
void SaveHiTable();
void ShowHiTable();
int My_SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect);
int My_SDL_Flip();
int GetScrollDx();
int GetGlobalX(Sprite *p);
int GetGlobalY(Sprite *p);
int OneRoomLeft();
int OneRoomRight();
bool IsFarFromJoe(Sprite *p);
Sprite::RoomPtr GetNeighboringRoom(Sprite::RoomPtr currentRoom, int dx, int stopper);


//void AddSDLDestructor(void (*fun)());
//void CallSDLDestructors();
//---------------------------------------------------------------------------
#endif

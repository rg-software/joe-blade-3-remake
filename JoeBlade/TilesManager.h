//---------------------------------------------------------------------------
#ifndef TilesManagerH
#define TilesManagerH

#include <vcl.h>
#include "SDL.h"
#include <vector>

using namespace std;

//---------------------------------------------------------------------------
namespace TTiles          // ����� ������ �����, ������� ���� ����� �� ������ :)
{
    extern int LEFT_STOP, RIGHT_STOP, UP_ARROW, DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, UP_ARROW_CLST, UP_ARROW_SCRT, LIFT_ENTRANCE, COMPUTER, UP_FLOOR, DOWN_FLOOR, FINISH;
    extern int KEY, GRENADE, DISK, HOSTAGE, DETONATOR, DYNAMITE, DYNAMITE_ARMED, AMMUNITION, ARMOUR, MASK, MINE, SECURITYCARD, LIFTCARD, FOOD, ANYOBJECT, NOTHING;
}
//---------------------------------------------------------------------------
class TTilesManager
{
    vector<SDL_Surface *> Tiles;       // ������ ������. Tiles[i] - ���� � id = i
    vector<AnsiString> TilePaths;      // (TilePaths[i] - ��� ������� ����� � id=i)

    void LoadTilePaths();
    void AssignSystemTiles();
    void LoadTiles();
    
    public:
        TTilesManager();
        SDL_Surface *GetTileByID(int id) { return Tiles[id]; }
};
//---------------------------------------------------------------------------
TTilesManager& TilesManager();
//---------------------------------------------------------------------------
#endif

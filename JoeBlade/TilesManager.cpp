//---------------------------------------------------------------------------
#pragma hdrstop

#include "TilesManager.h"
#include "config.h"
#include "Globals.h"
#include <vector>
#include <vcl.h>
#include <stdio.h>
#include "Utils.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
namespace TTiles
{
    int LEFT_STOP, RIGHT_STOP, UP_ARROW, DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, UP_ARROW_CLST, UP_ARROW_SCRT, LIFT_ENTRANCE, COMPUTER, UP_FLOOR, DOWN_FLOOR, FINISH;
    int KEY, GRENADE, DISK, HOSTAGE, DETONATOR, DYNAMITE, DYNAMITE_ARMED, AMMUNITION, ARMOUR, MASK, MINE, SECURITYCARD, LIFTCARD, FOOD, ANYOBJECT, NOTHING;
}
//---------------------------------------------------------------------------
TTilesManager& TilesManager()
{
    static TTilesManager manager;

    return manager;
}
//---------------------------------------------------------------------------
void TTilesManager::LoadTilePaths()             // загрузить массив путей к тайлам
{
    FILE *f = fopen("joeblade3.tiles", "rt");

    int id;
    char sid[10], path[255];

    for(;;)
    {
        fgets(sid, 10, f);                         // id
        fgets(path, 255, f);                       // path

        if(feof(f))
            break;

        id = atoi(sid);
        path[strlen(path) - 1] = 0;                 // обрезаем \n
        if(TilePaths.size() < unsigned(id + 1))     // расширяем массив
            TilePaths.resize(id + 1);

        TilePaths[id] = path;                       // дописываем путь в TilePaths
    }

    fclose(f);
}
//---------------------------------------------------------------------------
void TTilesManager::AssignSystemTiles()
{
    TTiles::ANYOBJECT = -1;
    
    for(unsigned i = 0; i < TilePaths.size(); i++)                            // важно: имена системных тайлов
        if(TilePaths[i].LowerCase() == "tiles\\system\\left_stop.gif")
            TTiles::LEFT_STOP = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\right_stop.gif")
            TTiles::RIGHT_STOP = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\up_arrow.gif")
            TTiles::UP_ARROW = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\down_arrow.gif")
            TTiles::DOWN_ARROW = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\left_arrow.gif")
            TTiles::LEFT_ARROW = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\right_arrow.gif")
            TTiles::RIGHT_ARROW = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\up_arrow_clst.gif")
            TTiles::UP_ARROW_CLST = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\up_arrow_scrt.gif")
            TTiles::UP_ARROW_SCRT = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\lift_entrance.gif")
            TTiles::LIFT_ENTRANCE = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\computer.gif")
            TTiles::COMPUTER = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\up_floor.gif")
            TTiles::UP_FLOOR = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\down_floor.gif")
            TTiles::DOWN_FLOOR = i;
        else if(TilePaths[i].LowerCase() == "tiles\\system\\finish.gif")
            TTiles::FINISH = i;

        else if(TilePaths[i].LowerCase() == "tiles\\objects\\key.gif")
            TTiles::KEY = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\grenade.gif")
            TTiles::GRENADE = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\disk.gif")
            TTiles::DISK = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\hostage.gif")
            TTiles::HOSTAGE = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\detonator.gif")
            TTiles::DETONATOR = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\dynamite.gif")
            TTiles::DYNAMITE = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\dynamite_armed.gif")
            TTiles::DYNAMITE_ARMED = i;

        else if(TilePaths[i].LowerCase() == "tiles\\objects\\ammunition.gif")
            TTiles::AMMUNITION = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\food.gif")
            TTiles::FOOD = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\armour.gif")
            TTiles::ARMOUR = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\mask.gif")
            TTiles::MASK = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\mine.gif")
            TTiles::MINE = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\nothing.gif")
            TTiles::NOTHING = i;

        else if(TilePaths[i].LowerCase() == "tiles\\objects\\liftcard.gif")
            TTiles::LIFTCARD = i;
        else if(TilePaths[i].LowerCase() == "tiles\\objects\\securitycard.gif")
            TTiles::SECURITYCARD = i;
}
//---------------------------------------------------------------------------
void TTilesManager::LoadTiles()                             // загрузить все тайлы
{
    Tiles.resize(TilePaths.size());                         // расширить массив до требуемого размера

    for(unsigned i = 1; i < TilePaths.size(); i++)          // для каждого тайла:
        Tiles[i] = LoadSurfaceFromGIFResource(TilePaths[i].c_str());
}
//---------------------------------------------------------------------------
TTilesManager::TTilesManager()
{
    LoadTilePaths();
    AssignSystemTiles();
    LoadTiles();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


#pragma hdrstop

#include "ResourceManager.h"
#include "StatusPanel.h"
#include <memory>
#include <vcl.h>
#include "MyGIFStructure.h"
#include "TilesManager.h"
#include "JoeBlade.h"
#include "Utils.h"
#include "Globals.h"
#include "Config.h"
#include "ScreenPrinter.h"
#include <algorithm>
//---------------------------------------------------------------------------

#pragma package(smart_init)

TMyStatusPanel& StatusPanel()
{
    static TMyStatusPanel panel;

    return panel;
}
//---------------------------------------------------------------------------
TMyStatusPanel::TMyStatusPanel()
{
    coords[TTiles::KEY] = pair<int, int>(257, 9);
    coords[TTiles::DISK] = pair<int, int>(257, 73);
    coords[TTiles::GRENADE] = pair<int, int>(371, 9);
    coords[TTiles::HOSTAGE] = pair<int, int>(371, 73);
    coords[TTiles::DYNAMITE] = pair<int, int>(481, 9);
    coords[TTiles::DETONATOR] = pair<int, int>(481, 73);

    numbers = LoadSurfaceFromGIFResource("Common\\numbers-yg.gif");
    bullets = LoadSurfaceFromGIFResource("Common\\bullets.gif");
    hearts = LoadSurfaceFromGIFResource("Common\\hearts.gif");
}
//---------------------------------------------------------------------------
void TMyStatusPanel::PrintNumber(int number, int x, int y, int width)
{
    char str[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    char format[] = { '%', '0', char(width + '0'), 'd'} ;

    sprintf(str, format, number);
    ScreenPrinter().Print(Config::StatusPanelX + x, Config::StatusPanelY + y, str);
}
//---------------------------------------------------------------------------
void TMyStatusPanel::Show()
{
    for(map<int, pair<int, int> >::iterator p = coords.begin(); p != coords.end(); p++)
        PrintNumber(Inventory[p->first], p->second.first, p->second.second, 2);

    PrintNumber(Score, 16*6, 9, 6);
    int timeleft = (DynamiteTimerEnabled ? (Config::DynamiteTimerTime - (GameTimerTicks - DynamiteTimer)) : Config::DynamiteTimerTime) / 1000;
    PrintNumber(timeleft / 60, 16*7 + 6, 73, 2);
    PrintNumber(timeleft % 60, 16*10, 73, 2);


    SDL_Rect src, dest;
    src.x = src.y = 0;
    src.h = bullets->h; src.w = Config::BulletW * Joe->Ammo;
    dest.x = Config::StatusPanelX; dest.y = 52 + Config::StatusPanelY;

    My_SDL_BlitSurface(bullets, &src, SDL_GetVideoSurface(), &dest);

    src.x = src.y = 0;
    src.h = hearts->h; src.w = Joe->Energy * Config::HeartW / Config::EnergyFactor;
    dest.x = Config::StatusPanelX; dest.y = 32 + Config::StatusPanelY;

    My_SDL_BlitSurface(hearts, &src, SDL_GetVideoSurface(), &dest);
}
//---------------------------------------------------------------------------

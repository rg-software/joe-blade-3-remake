//---------------------------------------------------------------------------
#include "Background.h"
#include "Utils.h"
#include "Config.h"
#include "ScreenPrinter.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
TBackground::TBackground()
{
    graphics = LoadSurfaceFromGIFResource("Common\\background-with-objects.gif");
}
//---------------------------------------------------------------------------
void TBackground::Show()
{
    My_SDL_BlitSurface(graphics, NULL, SDL_GetVideoSurface(), NULL);
}
//---------------------------------------------------------------------------
/*void TBackground::FastShow()
{
    SDL_Rect r1, r2, r3;

    r1.x = 0; r1.y = Config::Yoffset; r1.w = Config::Xoffset - 2 * Config::GridSize; r1.h = Config::RoomHeight;
    r2.x = Config::Xoffset + Config::RoomWidth; r2.y = Config::Yoffset; r2.w = SDL_GetVideoSurface()->w - r2.x; r2.h = Config::RoomHeight;
    r3.x = 0; r3.y = Config::MessagePanelY; r3.w = SDL_GetVideoSurface()->w, r3.h = ScreenPrinter().CharH();

    Show(&r1);
    Show(&r2);
    Show(&r3);
}  */
//---------------------------------------------------------------------------
void TBackground::Show(SDL_Rect *r)
{
    My_SDL_BlitSurface(graphics, r, SDL_GetVideoSurface(), r);
}
//---------------------------------------------------------------------------
TBackground& Background()
{
    static TBackground background;

    return background;
}
//---------------------------------------------------------------------------


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
    My_SDL_BlitSurface(graphics, NULL, Config::BackBuffer, NULL);
}
//---------------------------------------------------------------------------
void TBackground::Show(SDL_Rect *r)
{
    My_SDL_BlitSurface(graphics, r, Config::BackBuffer, r);
}
//---------------------------------------------------------------------------
TBackground& Background()
{
    static TBackground background;

    return background;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#pragma hdrstop

#include "MessagePanel.h"
#include <memory>
#include <SDL.h>
#include <vcl.h>
#include "Globals.h"
#include "Config.h"
#include "Background.h"
#include "ScreenPrinter.h"

using namespace std;

#pragma package(smart_init)

//---------------------------------------------------------------------------
Uint32 MessagePanelTimerFun(Uint32 interval, void *param)
{
    MessagePanel().Update();

    return interval;
}
//---------------------------------------------------------------------------
TMessagePanel& MessagePanel()
{
    static TMessagePanel panel;

    return panel;
}
//---------------------------------------------------------------------------
TMessagePanel::TMessagePanel()
{
    InitializeCriticalSection(&MessageCS);
    panel = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_ANYFORMAT, SDL_GetVideoSurface()->w, ScreenPrinter().CharH() * 3,
                    SDL_GetVideoSurface()->format->BitsPerPixel, SDL_GetVideoSurface()->format->Rmask,
                    SDL_GetVideoSurface()->format->Gmask, SDL_GetVideoSurface()->format->Bmask, SDL_GetVideoSurface()->format->Amask);

    SDL_SetColorKey(panel, SDL_SRCCOLORKEY, SDL_MapRGB(panel->format, Config::ColorKey[0], Config::ColorKey[1], Config::ColorKey[2]));
    SDL_FillRect(panel, NULL, SDL_MapRGB(panel->format, Config::ColorKey[0], Config::ColorKey[1], Config::ColorKey[2]));
    MsgY = ScreenPrinter().CharH();
    messages.push_back("");
    timer = SDL_AddTimer(Config::TimerInterval, MessagePanelTimerFun, NULL);

}
//---------------------------------------------------------------------------
void TMessagePanel::EnqueueMessage(const char *message)
{
   lastchanged = SDL_GetTicks();
   messages.push_back(message);
}
//---------------------------------------------------------------------------
void TMessagePanel::Show()
{
    EnterCriticalSection(&MessageCS);

    SDL_Rect src, dest;
    src.x = 0; src.y = ScreenPrinter().CharH();
    src.h = ScreenPrinter().CharH(); src.w = panel->w;
    dest.x = 0; dest.y = Config::MessagePanelY;
    dest.h = ScreenPrinter().CharH(); dest.w = panel->w;

    Background().Show(&dest);
    My_SDL_BlitSurface(panel, &src, SDL_GetVideoSurface(), &dest);

    LeaveCriticalSection(&MessageCS);
}
//---------------------------------------------------------------------------
void TMessagePanel::Update()
{
    EnterCriticalSection(&MessageCS);

    SDL_FillRect(panel, NULL, SDL_MapRGB(panel->format, Config::ColorKey[0], Config::ColorKey[1], Config::ColorKey[2]));
    if(MsgY == 0)
    {
        MsgY = ScreenPrinter().CharH();
        messages.pop_front();
    }

    list<AnsiString>::iterator p = messages.begin();

    ScreenPrinter().PrintCAligned(MsgY, p->c_str(), panel);
    p++;
    ScreenPrinter().PrintCAligned(MsgY + ScreenPrinter().CharH(), p->c_str(), panel);

    if(messages.size() > 1)
        MsgY--;

    LeaveCriticalSection(&MessageCS);
}
//---------------------------------------------------------------------------


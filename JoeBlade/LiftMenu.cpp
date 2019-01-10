//---------------------------------------------------------------------------
#pragma hdrstop

#include <algorithm>

#include "LiftMenu.h"
#include "Config.h"
#include "Globals.h"
#include "JoeBlade.h"
#include "Utils.h"
#include "ScreenPrinter.h"
#include "SoundManager.h"
#include "TilesManager.h"
#include "EnemiesManager.h"
#include "MessagePanel.h"
#include "Room.h"
#include "JukeBox.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
int FNIndex(int floorNo)
{
    return floorNo - Config::LowerFloor + 2;
}
//---------------------------------------------------------------------------
void LiftMenu()
{
    int maxcards = Config::sizeofLCF / sizeof(Config::LiftCardFloors[0]) - 1;   // карточек не может быть больше, чем элементов в массиве минус 1
    int cards = Config::AllLiftCards ? maxcards : Inventory[TTiles::LIFTCARD];
    int CurFloorIdx = Joe->CurrentRoom.Z;

    SoundManager().PlaySound(Sounds::LIFT);
    MessagePanel().EnqueueMessage((Config::Yoffset + ScreenPrinter().CharH(), (AnsiString("access to ") +
                                  (cards == maxcards ? "all floors" : GetFloorName(Config::LiftCardFloors[cards]))).c_str()));

    SDL_Delay(Config::MenuDelay);
    while(GameStatus == LIFTMENU)
    {
        SDL_Event event;
        SDL_PollEvent(&event);

        Uint8 *keys = SDL_GetKeyState(NULL);
        if(keys[SDLK_DOWN] == SDL_PRESSED)
        {
			CurFloorIdx = std::max(CurFloorIdx - 1, Config::LowerFloor);
            SoundManager().PlaySound(Sounds::LIFTBUTTON);
            SDL_Delay(Config::MenuDelay);
        }
        else if(keys[SDLK_UP] == SDL_PRESSED)
        {
			CurFloorIdx = std::min(CurFloorIdx + 1, Config::LiftCardFloors[cards]);
            SoundManager().PlaySound(Sounds::LIFTBUTTON);
            SDL_Delay(Config::MenuDelay);
        }
        else if(keys[SDLK_SPACE] == SDL_PRESSED)
        {
            GameStatus = GAME;
            Joe->SetRoom(CurFloorIdx, Joe->CurrentRoom.X, Joe->CurrentRoom.Y);
            SDL_Delay(Config::MenuDelay);
        }
        else if(keys[SDLK_ESCAPE] == SDL_PRESSED)
        {
            SDL_Delay(Config::MenuDelay);
            GameStatus = GAME;
        }

        /*SDL_Rect r;
        r.x = Config::Xoffset + Config::GridSize; r.y = Config::Yoffset;
        r.w = Config::RoomWidth - Config::GridSize; r.h = Config::RoomHeight;
        SDL_FillRect(SDL_GetVideoSurface(), &r, 0);*/
        ClearRoomBox();

        int MsgY = Config::Yoffset + 3*ScreenPrinter().CharH();

        for(int i = FNIndex(CurFloorIdx) + 2; i >= FNIndex(CurFloorIdx) - 2; i--)
        {
            ScreenPrinter().PrintCAligned(MsgY, Config::FloorNames[i]);
            if(i == FNIndex(CurFloorIdx))
            {
                ScreenPrinter().Print(Config::Xoffset + Config::GridSize, MsgY, ">");
                ScreenPrinter().Print(Config::Xoffset + Config::RoomWidth - ScreenPrinter().CharW(), MsgY, "<");
            }
            MsgY += ScreenPrinter().CharH();
        }

        MessagePanel().Show();
        JukeBox().Show();
        My_SDL_Flip();
    }

    SoundManager().PlaySound(Sounds::LIFT);

    for(map<AnsiString, Room *>::iterator p = Rooms.begin(); p != Rooms.end(); p++)
        p->second->DestroyCache();
}
//---------------------------------------------------------------------------

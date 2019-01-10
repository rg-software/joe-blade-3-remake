//---------------------------------------------------------------------------
#pragma hdrstop

#include "Subgame.h"
#include <algorithm>
#include <math.hpp>
#include "SoundManager.h"
#include "Config.h"
#include "Utils.h"
#include "SubgameIndicator.h"
#include "ScreenPrinter.h"
#include "MessagePanel.h"
#include "JukeBox.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
void Subgame::PrepareNumbers()
{
    numbers.clear();
    numbers.push_back(1);
    numbers.push_back(2);
    numbers.push_back(3);
    numbers.push_back(4);
    random_shuffle(numbers.begin(), numbers.end());
    numbers.push_back(1);
    numbers.push_back(2);
    numbers.push_back(3);
    numbers.push_back(4);

    int operations = RandomRange(MinOperations, MaxOperations + 1);
    for(int i = 0; i < operations; i++)
        switch(RandomRange(0, 3))
        {
            case 0: LeftOperation();  break;
            case 1: RightOperation(); break;
            case 2: FireOperation();  break;
        }

    fill(highlights.begin(), highlights.end(), false);
}
//---------------------------------------------------------------------------
bool Subgame::RunKernel()
{
    int time = SDL_GetTicks();
    int /*elapsed, lastelapsed = -1,*/ halfelapsed, lasthalfelapsed = -1;
    TGameStatus OldStatus = GameStatus;

    while(GameStatus == OldStatus)
    {
        SDL_Event event;

        if(SDL_PollEvent(&event) && event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_LEFT:  SoundManager().PlaySound(Sounds::SUBGAME_ACTION);
                                 LeftOperation();
                                 break;

                case SDLK_RIGHT: SoundManager().PlaySound(Sounds::SUBGAME_ACTION);
                                 RightOperation();
                                 break;

                case SDLK_SPACE: SoundManager().PlaySound(Sounds::SUBGAME_ACTION);
                                 FireOperation();
                                 break;
            }
        }
        /*Uint8 *keys = SDL_GetKeyState(NULL);

        if(keys[SDLK_LEFT] == SDL_PRESSED)
        {
            SoundManager().PlaySound(Sounds::SUBGAME_ACTION);
            LeftOperation();
            SDL_Delay(Config::MenuDelay);
        }

        if(keys[SDLK_RIGHT] == SDL_PRESSED)
        {
            SoundManager().PlaySound(Sounds::SUBGAME_ACTION);
            RightOperation();
            SDL_Delay(Config::MenuDelay);
        }

        if(keys[SDLK_SPACE] == SDL_PRESSED)
        {
            SoundManager().PlaySound(Sounds::SUBGAME_ACTION);
            FireOperation();
            SDL_Delay(Config::MenuDelay);
        } */

//        elapsed = Config::TimeStop ? 0 : (SDL_GetTicks() - time) / 1000;
        halfelapsed = Config::TimeStop ? 0 : (SDL_GetTicks() - time) / 500;

        if(halfelapsed != lasthalfelapsed)
        {
            lasthalfelapsed = halfelapsed;
            SubgameTimerFun();
        }

/*        if(elapsed != lastelapsed)
        {
            lastelapsed = elapsed;
            SoundManager().PlaySound(Sounds::TIMERTICK);
        }*/

        char time_msg[50];
        sprintf(time_msg, "%02d seconds", TotalTime / 1000 - halfelapsed / 2);

        ClearRoomBox();
        SubgameIndicator().Print(numbers, highlights);
        ScreenPrinter().PrintCAligned(Config::Yoffset + 1 * ScreenPrinter().CharH(), GetKeysMessage());
        ScreenPrinter().PrintCAligned(Config::Yoffset + 9 * ScreenPrinter().CharH(), time_msg);
        MessagePanel().Show();
        JukeBox().Show();

        My_SDL_Flip();

        if(IsWinningConfiguration())
            return true;
        else if(TotalTime / 1000 - halfelapsed / 2 <= 0)
            return false;
    }

    return false;
}
//---------------------------------------------------------------------------

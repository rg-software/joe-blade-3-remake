//---------------------------------------------------------------------------
#pragma hdrstop

#include "CraxSubgame.h"
#include "SubgameIndicator.h"
#include <algorithm>
#include <vector>
#include <math.hpp>
#include "Globals.h"
#include "ScreenPrinter.h"
#include "Utils.h"
#include "SoundManager.h"
#include "EnemiesManager.h"
#include "Utils.h"
#include "Config.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
void CraxSubgame()
{
    static TCraxSubgame subgame;

    subgame.Run();
}
//---------------------------------------------------------------------------
void TCraxSubgame::LeftOperation()
{
    int temp = numbers[0];
    numbers[0] = numbers[1];
    numbers[1] = numbers[2];
    numbers[2] = numbers[3];
    numbers[3] = temp;
}
//---------------------------------------------------------------------------
void TCraxSubgame::RightOperation()
{
    int temp = numbers[0];
    numbers[0] = numbers[3];
    numbers[3] = numbers[2];
    numbers[2] = numbers[1];
    numbers[1] = temp;
}
//---------------------------------------------------------------------------
void TCraxSubgame::FireOperation()
{
    int hidx;
    for(int i = 0; i < 4; i++)
        if(highlights[i])
        {
            hidx = i;
            break;
        }

    numbers[hidx] = RandomRange(1, 5);
    numbers[7 - hidx] = RandomRange(1, 5);
}
//---------------------------------------------------------------------------
void TCraxSubgame::SubgameTimerFun()
{
    SoundManager().PlaySound(Sounds::TIMERTICK);
    int oldidx;
    for(oldidx = 0; !highlights[oldidx]; oldidx++)
        ;

    int newidx;
    while((newidx = RandomRange(0, 4)) == oldidx)
        ;

    fill(highlights.begin(), highlights.end(), false);
    highlights[newidx] = highlights[7 - newidx] = true;
}
//---------------------------------------------------------------------------
void TCraxSubgame::Run()
{
    highlights[0] = highlights[7] = true;
    PrepareNumbers();
    highlights[0] = highlights[7] = true;

    SoundManager().PlaySound(Sounds::CRAX_SUBGAME);
    MyMessageBox("-1 3>sixty seconds to capture crax|-1 4>before his evil henchmen arrive!|-1 6>press space to continue|\0");

    if(RunKernel())
    {
        GameStatus = GAME;
        CraxCaptured = true;
        SoundManager().PlaySound(Sounds::CRAX_SUBGAME_VICTORY);
        MyMessageBox("-1 4>well played, mr. blade!|-1 6>press space to continue|\0");
        CraxCaptured = true;
        EnemiesManager().RemoveCrax();
        ShowNextObjective(CRAX_CAPTURED);
        Score += Config::ObjectiveItemScore;
    }
    else
    {
        GameStatus = GAMEOVER;
        SoundManager().PlaySound(Sounds::CRAX_SUBGAME_DEFEAT);

        ClearRoomBox();
        ScreenPrinter().PrintCAligned(Config::Yoffset + 5 * ScreenPrinter().CharH(), "too late! game over!");
    }
}
//---------------------------------------------------------------------------


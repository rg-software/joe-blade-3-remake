//---------------------------------------------------------------------------
#pragma hdrstop

#include "SecuritySubgame.h"
#include <algorithm>
#include <vector>
#include <math.hpp>
#include "Globals.h"
#include "ScreenPrinter.h"
#include "Utils.h"
#include "SoundManager.h"
#include "Config.h"

#pragma package(smart_init)

using namespace std;

//---------------------------------------------------------------------------
void SecuritySubgame()
{
    static TSecuritySubgame subgame;

    subgame.Run();
}
//---------------------------------------------------------------------------
void TSecuritySubgame::LeftOperation()
{
    int first = numbers[0];
    for(int i = 0; i < 7; i++)
        numbers[i] = numbers[i + 1];
    numbers[7] = first;
}
//---------------------------------------------------------------------------
void TSecuritySubgame::RightOperation()
{
    int last = numbers[7];
    for(int i = 7; i >= 1; i--)
        numbers[i] = numbers[i - 1];
    numbers[0] = last;
}
//---------------------------------------------------------------------------
void TSecuritySubgame::FireOperation()
{
    swap(numbers[0], numbers[7]);
}
//---------------------------------------------------------------------------
void TSecuritySubgame::Run()
{
    AnsiString msg;

    PrepareNumbers();
    highlights[0] = true;
    highlights[7] = true;

    SoundManager().PlaySound(Sounds::SECURITY_SUBGAME);
    MyMessageBox((msg.sprintf("-1 %d>sixty seconds to crack code|-1 %d>before security alarms goes off|-1 %d>press space to continue|\0", 3, 4, 6)).c_str());

    if(RunKernel())
    {
        GameStatus = GAME;
        SoundManager().PlaySound(Sounds::SECURITY_SUBGAME_VICTORY);
        MyMessageBox((msg.sprintf("-1 %d>well played, mr. blade!|-1 %d>press space to continue|\0", 4, 6)).c_str());
    }
    else
    {
        GameStatus = GAMEOVER;
        SoundManager().PlaySound(Sounds::SECURITY_SUBGAME_DEFEAT);
        ClearRoomBox();
        ScreenPrinter().PrintCAligned(Config::Yoffset + 5 * ScreenPrinter().CharH(), "too late! game over!");
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma hdrstop

#include <sdl.h>
#include "GameVictory.h"
#include "SoundManager.h"
#include "Globals.h"
#include "Config.h"
#include "Utils.h"
#include "MessagePanel.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
void GameVictory()
{
    SoundManager().PlaySound(Sounds::HELICOPTER);
    if(ObjectivesPending())
    {
        MyMessageBox("-1 3>well done! you have escaped!|-1 4>but you have not completed|-1 5>all mission objectives|-1 7>press space to continue|\0");
        Score += Config::ObjectiveItemScore;
    }
    else
    {
        MyMessageBox("-1 3>congratulations!|-1 4>you have completed your mission!|-1 6>press space to continue|\0");
        Score += 2 * Config::ObjectiveItemScore;
    }

    GameStatus = MAINMENU;
}
//---------------------------------------------------------------------------

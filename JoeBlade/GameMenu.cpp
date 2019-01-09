//---------------------------------------------------------------------------
#pragma hdrstop

#include "GameMenu.h"
#include "Globals.h"
#include "Utils.h"
#include "SoundManager.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
void GameMenu()
{
    vector<AnsiString> items;
    items.push_back("");
    items.push_back("in-game menu");
    items.push_back("");
    items.push_back("resume game");
    items.push_back("save game");
    items.push_back("quit game");

    vector<AnsiString> qitems;
    qitems.push_back("");
    qitems.push_back("really quit");
    qitems.push_back("");
    qitems.push_back("no");
    qitems.push_back("yes");


    SoundManager().PlaySound(Sounds::MENU_CLICK);
    switch(ShowMenu(items, 3, 5))
    {
        case -1:
        case  3: GameStatus = GAME; break;
        case  4: SaveGame(); GameStatus = GAME; break;
        case  5: GameStatus = (ShowMenu(qitems, 3, 4) == 4 ? MAINMENU : GAME);
                 break;
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma hdrstop

#include "MainMenu.h"
#include "Globals.h"
#include "Utils.h"
#include "Background.h"
#include "StatusPanel.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
void MainMenu()
{
    Background().Show();
    StatusPanel().Show();
    SDL_Flip(SDL_GetVideoSurface());
    Background().Show();
    StatusPanel().Show();

    vector<AnsiString> items;
    items.push_back("");
    items.push_back("joe blade D remake");
    items.push_back("");
    items.push_back("new game");
    items.push_back("load saved game");
    items.push_back("information");
    items.push_back("hiscores");
    items.push_back("exit to windows");
    items.push_back("");
    items.push_back("use cursor keys and space");

    int menuresult;
    while((menuresult = ShowMenu(items, 3, 7)) == -1)
        ;

    switch(menuresult)
    {
        case 3: InitializeNewGame();
                GameStatus = GAME;
                break;
        case 4: LoadGame(); break;
        case 5: ShowInfo();  break;
        case 6: ShowHiTable();  break;
        case 7: GameStatus = EXIT; break;
    }
}
//---------------------------------------------------------------------------

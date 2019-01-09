//---------------------------------------------------------------------------
#pragma hdrstop

#include <sdl.h>
#include "GameOver.h"
#include "Globals.h"
#include "Config.h"
#include "MessagePanel.h"
#include "Background.h"
#include "StatusPanel.h"
#include "ScreenPrinter.h"
#include "JukeBox.h"

#pragma package(smart_init)

AnsiString PlayerName;
//---------------------------------------------------------------------------
bool ExitCondition()
{
    static bool ReturnWasPressed = true;

    SDL_Event event;
    SDL_PollEvent(&event);
    Uint8 *keys = SDL_GetKeyState(NULL);
    if(!ReturnWasPressed && keys[SDLK_RETURN] == SDL_PRESSED)
    {
        ReturnWasPressed = true;
        return true;
    }
    ReturnWasPressed = (keys[SDLK_RETURN] == SDL_PRESSED);
    return false;
}
//---------------------------------------------------------------------------
void InputFun()
{
    static int KeyWasPressed[255];

    SDL_Event event;
    SDL_PollEvent(&event);
    Uint8 *keys = SDL_GetKeyState(NULL);

    ScreenPrinter().PrintCAligned(Config::Yoffset + ScreenPrinter().CharH() * 5, PlayerName.c_str());
    if(keys[SDLK_RETURN] == SDL_PRESSED)
        return;

    if(!KeyWasPressed[SDLK_BACKSPACE] && keys[SDLK_BACKSPACE] == SDL_PRESSED)
        PlayerName = PlayerName.SubString(1, PlayerName.Length() - 1);

    if(!KeyWasPressed[SDLK_SPACE] && keys[SDLK_SPACE] == SDL_PRESSED)
        PlayerName += ' ';

    for(int i = SDLK_a; i <= SDLK_z; i++)
        if(!KeyWasPressed[keys[i]] && keys[i] == SDL_PRESSED)
        {
            KeyWasPressed[keys[i]] = 1;
            if(PlayerName.Length() < 22)
                PlayerName += char('a' + (i - SDLK_a));
        }

    memset(KeyWasPressed, 0, sizeof(KeyWasPressed));
    for(int i = SDLK_a; i <= SDLK_z; i++)
        if(keys[i] == SDL_PRESSED)
            KeyWasPressed[keys[i]] = 1;//true;

    if(keys[SDLK_BACKSPACE] == SDL_PRESSED)
        KeyWasPressed[SDLK_BACKSPACE] = 1;
    if(keys[SDLK_SPACE] == SDL_PRESSED)
        KeyWasPressed[SDLK_SPACE] = 1;
}
//---------------------------------------------------------------------------
void InputPlayerName()
{
    PlayerName = "";
    MyMessageBox("-1 0>congratulations!|-1 1>a new hiscore!|-1 3>input your name:|", InputFun, ExitCondition);
}
//---------------------------------------------------------------------------
void GameOver()
{
    Uint32 time = SDL_GetTicks();

    while(SDL_GetTicks() < time + Config::PreGameOverDelay)
    {
        SDL_Event event;
        SDL_PollEvent(&event);

        Background().Show();
        StatusPanel().Show();
        JukeBox().Show();
        MessagePanel().Show();
        SDL_Flip(SDL_GetVideoSurface());
    }

    GameStatus = MAINMENU;

    if(Score <= HiTable[Config::HiTableSize - 1].score)
        return;

     HiTable.pop_back();
     vector<HiEntry>::iterator p = HiTable.begin();
     while(p->score >= Score)
        p++;
     InputPlayerName();
     HiTable.insert(p, HiEntry(PlayerName, Score));
     SaveHiTable();
}
//---------------------------------------------------------------------------

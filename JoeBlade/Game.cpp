//---------------------------------------------------------------------------
#pragma hdrstop

//#include <fstream>

#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "Room.h"
#include "Globals.h"
#include "Config.h"
#include "TilesManager.h"
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <math.hpp>
#include "JoeBlade.h"
#include "SimpleEnemy.h"
#include "Bullet.h"
#include "JukeBox.h"
#include "Smoke.h"
#include "Utils.h"
#include "Background.h"
#include "StatusPanel.h"
#include "ScreenPrinter.h"
#include "MessagePanel.h"
#include "EnemiesManager.h"

#pragma package(smart_init)

using namespace std;

//---------------------------------------------------------------------------
Uint32 TimerFun(Uint32 interval, void *param)
{
    GameTimerTicks += interval;
    
    Joe->Update();

    bullet->Update();
    enemybullet->Update();
    //jukebox.Update();

/*    for(vector<SimpleEnemy *>::iterator p = enemies.begin(); p != enemies.end(); p++)
        (*p)->Update();*/
    EnemiesManager().UpdateEnemiesList();

    for(vector<Smoke *>::iterator p = SmokeObjects.begin(); p != SmokeObjects.end(); p++)
        if((*p)->IsValid())
            (*p)->AdvanceFrame();

    return interval;
}
//---------------------------------------------------------------------------
void ShowNeighborhood(Sprite::RoomPtr currentRoom)
{
    Sprite::RoomPtr left = GetNeighboringRoom(currentRoom, -1, TTiles::LEFT_STOP);
    Sprite::RoomPtr right = GetNeighboringRoom(currentRoom, 1, TTiles::RIGHT_STOP);

    if(!Config::ScrollEnabled)
    {
        currentRoom.pointer->Show();
    }
    else // use scrolling
    {
        int D = GetScrollDx();

        if(D < 0 && left.pointer != NULL)
        {
            // текущая комната сдвигается на D точек вправо
            D = -D;

            SDL_Rect src;
            src.x = Config::GridSize; src.y = 0;
            src.w = Config::RoomWidth - D; src.h = Config::RoomHeight;

            currentRoom.pointer->Show(&src, Config::GridSize + D, 0);

            // слева отображается правый край левой комнаты
            src.x = Config::RoomWidth - D;
            src.w = D;

            left.pointer->Show(&src, Config::GridSize, 0);

        }
        else if(D > 0 && right.pointer != NULL) // right part
        {
            SDL_Rect src;
            src.x = D; src.y = 0;
            src.w = Config::RoomWidth - D; src.h = Config::RoomHeight;

            currentRoom.pointer->Show(&src, 0, 0);

            src.x = Config::GridSize;
            src.w = D;

            right.pointer->Show(&src, Config::RoomWidth - D, 0);

        }
        else
        {
            currentRoom.pointer->Show();
        }
    }
}
//---------------------------------------------------------------------------
void Game()
{
//    jukebox.LoadMusic("joeblade3.xm");
//    jukebox.Play();

    unsigned frames = 0;
    int ticks = GameTimerTicks;

    if(strcmp(MessagePanel().GetTopMessage(), GetFloorName(Joe->CurrentRoom.Z)))
        MessagePanel().EnqueueMessage(GetFloorName(Joe->CurrentRoom.Z));

    SDL_TimerID timer = SDL_AddTimer(Config::TimerInterval, TimerFun, NULL);  // реально отсюда начинается анимация

    while(GameStatus == GAME)
    {
        SDL_Event event;
        SDL_PollEvent(&event);

        Uint8 *keys = SDL_GetKeyState(NULL);
        if(keys[SDLK_ESCAPE] == SDL_PRESSED)
            GameStatus = GAMEMENU;

        ShowNeighborhood(Joe->CurrentRoom);
        Joe->Show();

        //for(vector<SimpleEnemy *>::iterator p = enemies.begin(); p != enemies.end(); p++)
        //    (*p)->Show();

        bullet->Show();
        enemybullet->Show();

        EnemiesManager().ShowEnemies();

        for(vector<Smoke *>::iterator p = SmokeObjects.begin(); p != SmokeObjects.end(); p++)
            (*p)->Show();

        if(SDL_GetTicks() - MessagePanel().LastChanged() > Config::MsgWaitTime)
        {
            if(strcmp(GetFloorName(Joe->CurrentRoom.Z), MessagePanel().GetTopMessage()))
                MessagePanel().EnqueueMessage(GetFloorName(Joe->CurrentRoom.Z));
        }

        Background().Show();
        MessagePanel().Show();
        StatusPanel().Show();
        JukeBox().Show();

        if(Config::ShowFPS)
        {
            char buffer[20];
            if(GameTimerTicks != ticks)
                sprintf(buffer, "%1.2f fps", frames / (0.001 * (GameTimerTicks - ticks)));
            ScreenPrinter().Print(Config::Xoffset + Config::GridSize, Config::Yoffset, buffer);
        }

        My_SDL_Flip();
        frames++;
    }

    SDL_RemoveTimer(timer);

    //    jukebox.Stop();

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <SDL.h>
#include <vcl.h>
#include <string>
#include <fstream>

#pragma hdrstop

#include "Bullet.h"
#include "TilesManager.h"
#include "SoundManager.h"
#include "StatusPanel.h"
#include "JoeBlade.h"
#include "Smoke.h"
#include "Config.h"
#include "MainMenu.h"
#include "LiftMenu.h"
#include "Game.h"
#include "GameVictory.h"
#include "GameMenu.h"
#include "GameOver.h"
#include "SecuritySubgame.h"
#include "ComputerSubgame.h"
#include "CraxSubgame.h"
#include "JukeBox.h"
#include "Utils.h"

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "bass.lib")
#pragma comment(lib, "SDL_Image.lib")
#pragma comment(lib, "ResFile.lib")
#pragma comment(lib, "MyGifLoader.lib")

//---------------------------------------------------------------------------
void SetOptions(const char *cmdline)
{
    AnsiString s = AnsiString(cmdline).LowerCase() + " ";

    Config::SDL_ScrnFlag = (strstr(s.c_str(), "/f ") == NULL) ? 0 : (SDL_FULLSCREEN | SDL_DOUBLEBUF);
    Config::InfEnergy = (strstr(s.c_str(), "/ie ") != NULL);
    Config::InfAmmo = (strstr(s.c_str(), "/ia ") != NULL);
    Config::TimeStop = (strstr(s.c_str(), "/t ") != NULL);
    Config::AllLiftCards = (strstr(s.c_str(), "/l ") != NULL);
    Config::InfKeysDisks = (strstr(s.c_str(), "/kd ") != NULL);
    Config::ShowFPS = (strstr(s.c_str(), "/fps ") != NULL);
	Config::JukeBoxDisabled = (strstr(s.c_str(), "/dj ") != NULL);
	Config::ScrollEnabled = (strstr(s.c_str(), "/se ") != NULL);
	Config::NoSound = (strstr(s.c_str(), "/ns ") != NULL);
	Config::DoubleScreen  = (strstr(s.c_str(), "/ds ") != NULL);
}
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
 try{

    // на всякий случай: использовать только один CPU :)
    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);
    if(SystemInfo.dwNumberOfProcessors > 1)
    {
        HANDLE hProcess = GetCurrentProcess();
        SetProcessAffinityMask( hProcess, 1L );
    }

    std::string e_cmdline = GetCommandLineA();//cmdline;
    std::string s;
    std::ifstream is("JoeBlade3.cfg");
    while(is >> s)
        e_cmdline += " " + s;
    SetOptions(e_cmdline.c_str());
    if(Config::NoSound)
		Config::JukeBoxDisabled = true;

	int ScreenW = Config::DoubleScreen ? Config::ScreenParams[0] * 2 : Config::ScreenParams[0];
	int ScreenH = Config::DoubleScreen ? Config::ScreenParams[1] * 2 : Config::ScreenParams[1];

	SDL_Init(SDL_INIT_VIDEO | /*SDL_INIT_AUDIO | */SDL_INIT_TIMER);
	SDL_SetVideoMode(ScreenW, ScreenH, Config::ScreenParams[2], SDL_HWSURFACE | SDL_ANYFORMAT | Config::SDL_ScrnFlag);
    SDL_WM_SetCaption("Joe Blade 3 Remake", NULL);
    SDL_ShowCursor(SDL_DISABLE);
    SDL_Surface *ico = LoadSurfaceFromGIFResource("Common\\joeblade-ico.gif");
    SDL_WM_SetIcon(ico, NULL);
    SDL_FreeSurface(ico);

	SDL_Surface *v = SDL_GetVideoSurface();
	Config::BackBuffer = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_ANYFORMAT, Config::ScreenParams[0], Config::ScreenParams[1],
					v->format->BitsPerPixel, v->format->Rmask, v->format->Gmask, v->format->Bmask, v->format->Amask);
	SDL_SetColorKey(Config::BackBuffer, SDL_SRCCOLORKEY, SDL_MapRGB(v->format, Config::ColorKey[0], Config::ColorKey[1], Config::ColorKey[2]));

	ShowSplash();
	Uint32 ticks = SDL_GetTicks();

	randomize();
	Randomize();
	GameTimerTicks = 0;
	TilesManager();    // инициализация
	SoundManager();
	Joe = new JoeBlade("Sprites\\joeblade.gif");
	StatusPanel();
	LoadHiTable();
	bullet = new Bullet("Sprites\\bullet.gif");
	enemybullet = new Bullet("Sprites\\bullet.gif");

	for(vector<Smoke *>::iterator p = SmokeObjects.begin(); p != SmokeObjects.end(); p++)
		*p = new Smoke("Sprites\\smoke.gif");

	while(SDL_GetTicks() - ticks < 3000)  // 6000
        ;

    RemoveSplash();

    if(!Config::JukeBoxDisabled)
        JukeBox().EnableDisable();

    GameStatus = MAINMENU;

    while(GameStatus != EXIT)
        switch(GameStatus)
        {
            case MAINMENU:         MainMenu();        break;
            case GAME:             Game();            break;
            case GAMEMENU:         GameMenu();        break;
            case LIFTMENU:         LiftMenu();        break;
            case GAMEOVER:         GameOver();        break;
            case GAMEVICTORY:      GameVictory();     break;
            case SECURITY_SUBGAME: SecuritySubgame(); break;
            case COMPUTER_SUBGAME: ComputerSubgame(); break;
            case CRAX_SUBGAME:     CraxSubgame();     break;
        }


    ClearRooms();

    delete bullet;
    delete enemybullet;
    delete Joe;
/*    for(vector<SimpleEnemy *>::iterator p = enemies.begin(); p != enemies.end(); p++)
        delete *p;*/

    for(vector<Smoke *>::iterator p = SmokeObjects.begin(); p != SmokeObjects.end(); p++)
        delete *p;

//    DeleteCriticalSection(&CriticalSection);

//    CallSDLDestructors();
//    SDL_Quit();

 }
 catch(...) {}

 return 0;
}
//---------------------------------------------------------------------------

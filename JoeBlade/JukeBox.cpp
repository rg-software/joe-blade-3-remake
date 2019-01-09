//---------------------------------------------------------------------------
#pragma hdrstop

#include "JukeBox.h"
#include "ScreenPrinter.h"
#include "Background.h"
#include "StatusPanel.h"
#include <algorithm>
#include <Math.hpp>
#include "Config.h"
#include "MessagePanel.h"
#include "ResourceManager.h"
#include <memory>

#pragma package(smart_init)

//---------------------------------------------------------------------------
TJukeBox& JukeBox()
{
    static TJukeBox jukebox;

    return jukebox;
}
//---------------------------------------------------------------------------
Uint32 JukeBoxTimerFun(Uint32 interval, void *param)
{
    JukeBox().TimerFun();

    return interval;
}
//---------------------------------------------------------------------------
void TJukeBox::TimerFun()
{
    static bool inside = false;
    static int delay = 25;
    static bool Playing = true;
    static int LastTrackChange = SDL_GetTicks();


    if(inside)
        return;

    inside = true;
    --delay;
    if(delay == 0 && Playing && BASS_ChannelIsActive(JukeBox().all_music[index]) != BASS_ACTIVE_PLAYING && Enabled)
    {
        Stop();
        index = RandomRange(0, FileName.size());
        Playing = false;
        delay = 25;
    }

    if(delay == 0 && !Playing && Enabled)
    {
        Play();
        Playing = true;
        delay = 25;
    }

    if(delay == 0)
        delay = 25;

    SDL_Event event;
    SDL_PollEvent(&event);

    Uint8 *keys = SDL_GetKeyState(NULL);
    if((keys[SDLK_LCTRL] == SDL_PRESSED || keys[SDLK_RCTRL] == SDL_PRESSED) && !JWasPressed)
        EnableDisable();

    if(Enabled && (keys[SDLK_LSHIFT] == SDL_PRESSED || keys[SDLK_RSHIFT] == SDL_PRESSED) && SDL_GetTicks() - LastTrackChange > Config::TrackChangeInterval)// && !NWasPressed)
    {
        LastTrackChange = SDL_GetTicks();
        //BASS_ChannelStop(music);
        Stop();
        delay = 1;
        //TimerFun();
    }

//    NWasPressed = (keys[SDLK_LSHIFT] == SDL_PRESSED || keys[SDLK_RSHIFT] == SDL_PRESSED) && (SDL_GetTicks() - LastNPressed > Config::TrackChangeInterval);
    JWasPressed = (keys[SDLK_LCTRL] == SDL_PRESSED || keys[SDLK_RCTRL] == SDL_PRESSED);

  //  if(NWasPressed)
   //     LastNPressed = SDL_GetTicks();

   inside = false;
}
//---------------------------------------------------------------------------
void TJukeBox::EnableDisable()
{
    if(Config::NoSound)
        return;

    Enabled = !Enabled;
    if(!Enabled)
    {
        //BASS_ChannelStop(music);
        Stop();
        MessagePanel().EnqueueMessage("jukebox disabled");
    }
    else
    {
        //BASS_ChannelStop(music);
        //Stop();
        MessagePanel().EnqueueMessage("jukebox enabled");
        Play();
    }
}
//---------------------------------------------------------------------------
TJukeBox::TJukeBox()
{
    index = 0;
    JWasPressed = /*NWasPressed =*/ false;
    Enabled = false;

    FILE *f = fopen("joeblade3.music", "rt");

    char path[255];
    char message[255];
    char empty[50];
    char ampl[20];
    for(;;)
    {
        fgets(path, 255, f);
        fgets(message, 255, f);
        fgets(ampl, 20, f);
        fgets(empty, 50, f);

        if(feof(f))
            break;

        path[strlen(path) - 1] = 0;                 // обрезаем \n
        message[strlen(message) - 1] = 0;                 // обрезаем \n
        ampl[strlen(ampl) - 1] = 0;                 // обрезаем \n

        Amplification.push_back(atoi(ampl));
        Title.push_back(message);
        FileName.push_back(path);
    }

    fclose(f);

    LoadAllMusic();

    timer = SDL_AddTimer(Config::TimerInterval, JukeBoxTimerFun, NULL);
}
//---------------------------------------------------------------------------
TJukeBox::~TJukeBox()
{
    SDL_RemoveTimer(timer);
}
//---------------------------------------------------------------------------
void TJukeBox::LoadAllMusic()
{
    all_music.resize(FileName.size());

    for(int i = 0; i < FileName.size(); ++i)
    {
        all_music[i] = LoadMusic(FileName[i].c_str());
        BASS_MusicSetAmplify(all_music[i], Amplification[i]);
    }
}
//---------------------------------------------------------------------------
HMUSIC TJukeBox::LoadMusic(char *filename)
{
    std::auto_ptr<TStream> stream(ResourceManager().GetResourceStream(filename));
    char *buffer = new char[stream->Size];
    stream->ReadBuffer(buffer, stream->Size);

    HMUSIC music = BASS_MusicLoad(true, buffer, 0, 0, BASS_MUSIC_RAMPS, 0);
    delete[] buffer;

    return music;
}
//---------------------------------------------------------------------------
void TJukeBox::Play()
{
    //LoadMusic(FileName[index].c_str());
    //BASS_MusicSetAmplify(music, Amplification[index]);
    BASS_MusicPlay(all_music[index]);
    Show();
}
//---------------------------------------------------------------------------
void TJukeBox::Show()
{
    SDL_Rect r;
    r.x = 0;
    r.y = 450;
    r.w = Config::ScreenParams[0];
    r.h = Config::ScreenParams[1] - 450 + 1;
    Background().Show(&r);
    ScreenPrinter().PrintCAligned(450, Title[index].c_str());
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

#ifndef JukeBoxH
#define JukeBoxH

#include <bass.h>
#include <vcl.h>
#include <SDL.h>
#include <vector>

//---------------------------------------------------------------------------
class TJukeBox
{
    bool Enabled;
    std::vector<AnsiString> Title;
    std::vector<AnsiString> FileName;
    std::vector<int> Amplification;
    SDL_TimerID timer;
//    HMUSIC music;
    std::vector<HMUSIC> all_music;
    
    int index;
    bool JWasPressed;//, NWasPressed;

    void LoadAllMusic();
    HMUSIC LoadMusic(char *filename);

    public:
        TJukeBox();
        ~TJukeBox();

        void EnableDisable();
        void TimerFun();
        void Play();
        void Show();
        void Stop()
        {
            for(int i = 0; i < all_music.size(); ++i)
                BASS_ChannelStop(all_music[i]);
            /*if(music != NULL) BASS_MusicFree(music);*/
        }

};
//---------------------------------------------------------------------------
TJukeBox& JukeBox();
//---------------------------------------------------------------------------

#endif

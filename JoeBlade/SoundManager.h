//---------------------------------------------------------------------------
#ifndef SoundManagerH
#define SoundManagerH

#include <bass.h>
#include <vector>

using namespace std;

//---------------------------------------------------------------------------
namespace Sounds
{
    extern int GUNSHOT, ENTERDOOR, ENTERDOOR2, GETOBJECT, LIFT, LIFTBUTTON, ZEROENERGY, GRENADEEXPLOSION, MINEEXPLOSION, OUTOFTIME,
               MENU_CLICK, SUBGAME_ACTION, SECURITY_SUBGAME, TIMERTICK, SECURITY_SUBGAME_VICTORY, SECURITY_SUBGAME_DEFEAT,
               COMPUTER_SUBGAME, COMPUTER_SUBGAME_VICTORY, COMPUTER_SUBGAME_DEFEAT, CRAX_SUBGAME, CRAX_SUBGAME_VICTORY, CRAX_SUBGAME_DEFEAT, HELICOPTER;
}
//---------------------------------------------------------------------------
class TSoundManager
{
    vector<HSAMPLE> sounds;
    
    void LoadSounds();
    int LoadSound(char *filename);

    public:
        TSoundManager();
        ~TSoundManager()
        {
            for(vector<HSAMPLE>::iterator p = sounds.begin(); p != sounds.end(); p++)
                BASS_SampleFree(*p);
        }
        
        void PlaySound(int id)
        {
            BASS_SampleStop(sounds[id]);
            BASS_SamplePlay(sounds[id]);
        }
};
//---------------------------------------------------------------------------
TSoundManager& SoundManager();
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#pragma hdrstop

#include "Config.h"
#include "SoundManager.h"
#include <memory>
#include <vcl.h>
#include "ResourceManager.h"

#pragma package(smart_init)

const int MAXSAMPLES = 20;
//---------------------------------------------------------------------------
namespace Sounds
{
    int GUNSHOT, ENTERDOOR, ENTERDOOR2, GETOBJECT, LIFT, LIFTBUTTON, ZEROENERGY, GRENADEEXPLOSION, MINEEXPLOSION, OUTOFTIME,
        MENU_CLICK, SUBGAME_ACTION, SECURITY_SUBGAME, TIMERTICK, SECURITY_SUBGAME_VICTORY, SECURITY_SUBGAME_DEFEAT,
        COMPUTER_SUBGAME, COMPUTER_SUBGAME_VICTORY, COMPUTER_SUBGAME_DEFEAT, CRAX_SUBGAME, CRAX_SUBGAME_VICTORY, CRAX_SUBGAME_DEFEAT, HELICOPTER;
}
//---------------------------------------------------------------------------
TSoundManager& SoundManager()
{
    static TSoundManager manager;

    return manager;
}
//---------------------------------------------------------------------------
int TSoundManager::LoadSound(char *filename)
{
    std::auto_ptr<TStream> stream(ResourceManager().GetResourceStream(filename));
    char *buffer = new char[stream->Size];
    stream->ReadBuffer(buffer, stream->Size);
    sounds.push_back(BASS_SampleLoad(true, buffer, 0, stream->Size, MAXSAMPLES, 0));
    delete[] buffer;
    
    return (int)sounds.size() - 1;
}
//---------------------------------------------------------------------------
void TSoundManager::LoadSounds()
{
    Sounds::HELICOPTER = LoadSound("Sounds\\helicopter.wav");
    Sounds::GUNSHOT = LoadSound("Sounds\\gunshot.wav");
    Sounds::ENTERDOOR = LoadSound("Sounds\\enterdoor2.wav");
    Sounds::ENTERDOOR2 = LoadSound("Sounds\\enterdoor.wav");
    Sounds::GETOBJECT = LoadSound("Sounds\\getobject.wav");
    Sounds::LIFT = LoadSound("Sounds\\lift.wav");
    Sounds::LIFTBUTTON = LoadSound("Sounds\\liftbutton.wav");
    Sounds::ZEROENERGY = LoadSound("Sounds\\zeroenergy.wav");
    Sounds::GRENADEEXPLOSION = LoadSound("Sounds\\grenadeexplosion.wav");
    Sounds::MINEEXPLOSION = LoadSound("Sounds\\mineexplosion.wav");
    Sounds::OUTOFTIME = LoadSound("Sounds\\outoftime.wav");
    Sounds::SUBGAME_ACTION = LoadSound("Sounds\\subgame_action.wav");
    Sounds::SECURITY_SUBGAME = LoadSound("Sounds\\security_subgame.wav");
    Sounds::TIMERTICK = LoadSound("Sounds\\timertick.wav");
    Sounds::SECURITY_SUBGAME_VICTORY = LoadSound("Sounds\\security_subgame_victory.wav");
    Sounds::SECURITY_SUBGAME_DEFEAT = LoadSound("Sounds\\security_subgame_defeat.wav");
    Sounds::COMPUTER_SUBGAME = LoadSound("Sounds\\computer_subgame.wav");
    Sounds::COMPUTER_SUBGAME_VICTORY = LoadSound("Sounds\\computer_subgame_victory.wav");
    Sounds::COMPUTER_SUBGAME_DEFEAT = LoadSound("Sounds\\computer_subgame_defeat.wav");
    Sounds::CRAX_SUBGAME = LoadSound("Sounds\\crax_subgame.wav");
    Sounds::CRAX_SUBGAME_VICTORY = LoadSound("Sounds\\crax_subgame_victory.wav");
    Sounds::CRAX_SUBGAME_DEFEAT = LoadSound("Sounds\\crax_subgame_defeat.wav");
    Sounds::MENU_CLICK = LoadSound("Sounds\\menuclick.wav");
}
//---------------------------------------------------------------------------
TSoundManager::TSoundManager()
{
    if(!Config::NoSound)
        BASS_Init(1, 2*22050, 0, 0, NULL);
    LoadSounds();
}
//---------------------------------------------------------------------------


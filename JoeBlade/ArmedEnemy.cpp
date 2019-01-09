//---------------------------------------------------------------------------
#pragma hdrstop

#include "ArmedEnemy.h"
#include "TilesManager.h"
#include "SoundManager.h"
#include "Globals.h"
#include "Bullet.h"
#include "Smoke.h"
#include "Room.h"
#include "JoeBlade.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
void ArmedEnemy::MakeDecision()
{
    if(CurrentRoom == Joe->CurrentRoom && !enemybullet->IsValid() &&
       GameTimerTicks > LastBulletTryTime + Config::BulletInterval &&
       RandomRange(0, 100) < Config::BulletProb)
    {
        enemybullet->Init(Direction == LEFT ? X : X + GetW(), Y + 30, Direction == LEFT ? -32 : 32);    // инициализация пули...
        LastBulletTryTime = GameTimerTicks;
        SoundManager().PlaySound(Sounds::GUNSHOT);
    }

    CleverEnemy::MakeDecision();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma hdrstop

#include "CleverEnemy.h"
#include "TilesManager.h"
#include "Globals.h"
#include "Bullet.h"
#include "Smoke.h"
#include "Room.h"
#include "JoeBlade.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
/*void CleverEnemy::ProcessBulletHit()
{
    Enemy::ProcessBulletHit();
}*/
//---------------------------------------------------------------------------
/*void CleverEnemy::ProcessFarFromJoeEvent()
{
    Enemy::ProcessFarFromJoeEvent();
} */
//---------------------------------------------------------------------------
void CleverEnemy::MakeDecision()
{
    if(Joe->CurrentRoom == CurrentRoom && Direction != LEFT &&
       (Joe->GetX() + Joe->GetW() < X + GetW()) &&
       (GameTimerTicks > LastChDirTryTime + Config::ChDirInterval) &&
       (RandomRange(0, 100) < Config::ChDirProb))
    {
        Direction = LEFT;
        UpdateFrame();
        LastChDirTryTime = GameTimerTicks;
    }
    else if(Joe->CurrentRoom == CurrentRoom && Direction != RIGHT &&
            (Joe->GetX() > X + GetW() + GetW()) &&
            (GameTimerTicks > LastChDirTryTime + Config::ChDirInterval) &&
            (RandomRange(0, 100) < Config::ChDirProb))
    {
        Direction = RIGHT;
        UpdateFrame();
        LastChDirTryTime = GameTimerTicks;
    }

    SimpleEnemy::MakeDecision();
}
//---------------------------------------------------------------------------

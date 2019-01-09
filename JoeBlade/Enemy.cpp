//---------------------------------------------------------------------------
#pragma hdrstop

#include "Enemy.h"
#include "Bullet.h"
#include "Globals.h"
#include "Smoke.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
void Enemy::ProcessBulletHit()
{
    IsValid = false;
    SetResurrectTime();
    bullet->Remove();

    for(int i = 0; i < Config::MaxEnemies; i++)
        if(!SmokeObjects[i]->IsValid())
        {
            SmokeObjects[i]->Init(X, Y);
            SmokeObjects[i]->CurrentRoom = CurrentRoom;
            break;
        }
}
//---------------------------------------------------------------------------
void Enemy::ProcessFarFromJoeEvent()
{
    if(DistanceToJoe() > Config::MaxDistanceFromJoe)
    {
        IsValid = false;
        SetResurrectTime();
    }
}
//---------------------------------------------------------------------------
void Enemy::Update()
{
    if(DistanceToJoe() > Config::MaxDistanceFromJoe)
        ProcessFarFromJoeEvent();

    if(bullet->Hits(this))
        ProcessBulletHit();

    MakeDecision();
}
//---------------------------------------------------------------------------


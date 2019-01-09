//---------------------------------------------------------------------------
#pragma hdrstop

#include "SimpleEnemy.h"
#include "TilesManager.h"
#include "Globals.h"
#include "Bullet.h"
#include "Smoke.h"
#include "Room.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
/*void SimpleEnemy::ProcessBulletHit()
{
    Enemy::ProcessBulletHit();
}
//---------------------------------------------------------------------------
void SimpleEnemy::ProcessFarFromJoeEvent()
{
    Enemy::ProcessFarFromJoeEvent();
} */
//---------------------------------------------------------------------------
void SimpleEnemy::MakeDecision()
{
    if(!CurrentRoom.pointer->Intersects(this, Direction == LEFT ? TTiles::LEFT_STOP : TTiles::RIGHT_STOP, Config::SystemLayer, 0))
    {
        AdvanceFrame();           // просто переходим к следующему кадру
        MakeStep();               // и делаем шаг
    }
    else
    {
        Direction = (Direction == LEFT ? RIGHT : LEFT);        // меняем направление на противоположное
        UpdateFrame();
    }

    UpdateRoomLR();

    if(GameTimerTicks > LastDoorTryTime + Config::DoorInterval)
    {
        if(CurrentRoom.pointer->Intersects(this, TTiles::UP_ARROW, Config::SystemLayer, 0.5) ||
           CurrentRoom.pointer->Intersects(this, TTiles::UP_ARROW_CLST, Config::SystemLayer, 0.5) ||
           CurrentRoom.pointer->Intersects(this, TTiles::UP_ARROW_SCRT, Config::SystemLayer, 0.5))
        {
            if(RandomRange(0, 100) < Config::DoorEnterProb)
                ChangeRoom(0, 0, -1);
            LastDoorTryTime = GameTimerTicks;
        }
        else if(CurrentRoom.pointer->Intersects(this, TTiles::DOWN_ARROW, Config::SystemLayer, 0.5))
        {
            if(RandomRange(0, 100) < Config::DoorEnterProb)
                ChangeRoom(0, 0, 1);
            LastDoorTryTime = GameTimerTicks;
        }
    }
}
//---------------------------------------------------------------------------

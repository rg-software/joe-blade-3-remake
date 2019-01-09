//---------------------------------------------------------------------------
#pragma hdrstop

#include "CraxBloodfinger.h"
#include "TilesManager.h"
#include "Globals.h"
#include "Bullet.h"
#include "Smoke.h"
#include "Room.h"
#include "JoeBlade.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
void CraxBloodfinger::ProcessBulletHit()
{
}
//---------------------------------------------------------------------------
void CraxBloodfinger::ProcessFarFromJoeEvent()
{
    if(DistanceToJoe() > Config::FloorDistance)
    {
        IsValid = false;
        SetResurrectTime();
    }
}
//---------------------------------------------------------------------------
void CraxBloodfinger::ProcessStep()
{
    if(!CurrentRoom.pointer->Intersects(this, Direction == LEFT ? TTiles::LEFT_STOP : TTiles::RIGHT_STOP, Config::SystemLayer, 0))
    {
        if(Joe->CurrentRoom == CurrentRoom && Direction == LEFT &&
           (X - (Joe->GetX() + Joe->GetW()) > 0) &&
           (X - (Joe->GetX() + Joe->GetW()) < Joe->GetW() / 3))
        {
            Direction = RIGHT;
            UpdateFrame();
        }
        else if(Joe->CurrentRoom == CurrentRoom && Direction == RIGHT &&
                (Joe->GetX() - (X + GetW()) > 0) &&
                (Joe->GetX() - (X + GetW()) < Joe->GetW() / 3))
        {
            Direction = LEFT;
            UpdateFrame();
        }
        else
        {
            AdvanceFrame();           // просто переходим к следующему кадру
            MakeStep();               // и делаем шаг
        }
    }
    else
    {
        Direction = (Direction == LEFT ? RIGHT : LEFT);        // меняем направление на противоположное
        UpdateFrame();
    }

    UpdateRoomLR();
}
//---------------------------------------------------------------------------
void CraxBloodfinger::MakeDecision()
{
    ProcessStep();

    if(GameTimerTicks > LastDoorTryTime + Config::DoorInterval)
    {
        if(CurrentRoom.pointer->Intersects(this, TTiles::UP_ARROW, Config::SystemLayer, 0.5) ||
           CurrentRoom.pointer->Intersects(this, TTiles::UP_ARROW_CLST, Config::SystemLayer, 0.5) ||
           CurrentRoom.pointer->Intersects(this, TTiles::UP_ARROW_SCRT, Config::SystemLayer, 0.5))
        {
            //if(RandomRange(0, 100) < Config::DoorEnterProb)
                ChangeRoom(0, 0, -1);
            LastDoorTryTime = GameTimerTicks;
        }
        else if(CurrentRoom.pointer->Intersects(this, TTiles::DOWN_ARROW, Config::SystemLayer, 0.5))
        {
            //if(RandomRange(0, 100) < Config::DoorEnterProb)
                ChangeRoom(0, 0, 1);
            LastDoorTryTime = GameTimerTicks;
        }
    }
}
//---------------------------------------------------------------------------

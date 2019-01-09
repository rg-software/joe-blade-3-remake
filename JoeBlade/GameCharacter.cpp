//---------------------------------------------------------------------------
#pragma hdrstop

#include "GameCharacter.h"
#include "Config.h"
#include "TilesManager.h"
#include "Room.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
void GameCharacter::UpdateFrame()        // если текущий кадр не соответствует текущему направлению,
{                                          // меняем набор кадров
    if(CurrentFrame > MaxFrameIndex())
        CurrentFrame -= MaxFrameIndex() - MinFrameIndex() + 1;
    else if(CurrentFrame < MinFrameIndex())
        CurrentFrame += MaxFrameIndex() - MinFrameIndex() + 1;
}
//---------------------------------------------------------------------------
void GameCharacter::UpdateRoomLR()
{
    Sprite *SJoe = (Sprite *)Joe;

    if(CurrentRoom.pointer->Intersects(this, TTiles::LEFT_ARROW, Config::SystemLayer, 1))       // если Джо полностью перекрыл "стрелку влево"
    {                                                                                   // переходим в левую комнату
        ChangeRoom(0, -1, 0);
        X = Config::RoomWidth - GetW() + Config::GridSize - 1;

        if(Config::ScrollEnabled)
        {
            X -= 22; //GetW() / 2 - 1; //Config::GridSize / 2 + 3 // $$
            if(this != SJoe)
                X -= +(SJoe->GetW() - GetW());
        }

        // if(Config::ScrollEnabled) X -= Config::GridSize / 2 + 3;

    }
    if(CurrentRoom.pointer->Intersects(this, TTiles::RIGHT_ARROW, Config::SystemLayer, 1))      // аналогично для правой стрелки
    {
        ChangeRoom(0, 1, 0);
        X = Config::GridSize - Config::GridSize + 1;

        if(Config::ScrollEnabled)
        {
            X += 22; //GetW() / 2 - 1;             //  $$
            if(this != SJoe)
                X += +(SJoe->GetW() - GetW());
        }
        // if(Config::ScrollEnabled) X += Config::GridSize / 2 + 3;
    }
}
//---------------------------------------------------------------------------
void GameCharacter::SetTimeToWait(int msecs)
{
    newticks = GameTimerTicks + msecs;
}
//---------------------------------------------------------------------------
void GameCharacter::Show()
{
    //if(CharacterState == ACTIVE)
    if(IsValid)
        Sprite::Show();
}
//---------------------------------------------------------------------------
int GameCharacter::DistanceToJoe()
{
    const RoomPtr& JoeRoom = ((Sprite *)Joe)->CurrentRoom;

    /*return max(JoeRoom.X, CurrentRoom.X) - min(JoeRoom.X, CurrentRoom.X) +
           max(JoeRoom.Y, CurrentRoom.Y) - min(JoeRoom.Y, CurrentRoom.Y) +
           100 * (max(JoeRoom.Z, CurrentRoom.Z) - min(JoeRoom.Z, CurrentRoom.Z));*/

    return abs(JoeRoom.X - CurrentRoom.X) + abs(JoeRoom.Y - CurrentRoom.Y) +
           Config::FloorDistance * abs(JoeRoom.Z - CurrentRoom.Z);
}
//---------------------------------------------------------------------------


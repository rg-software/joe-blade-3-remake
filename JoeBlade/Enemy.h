//---------------------------------------------------------------------------
#ifndef EnemyH
#define EnemyH

#include "GameCharacter.h"
#include <SDL.h>
#include <math.hpp>

//---------------------------------------------------------------------------
class Enemy : public GameCharacter
{
    protected:
        int FramesetIdx;         // for save/load

        virtual void MakeDecision() = 0;
        virtual void ProcessFarFromJoeEvent();
        int ResurrectTime;
        void SetResurrectTime() { ResurrectTime = GameTimerTicks + RandomRange(Config::MinTimeToWait, Config::MaxTimeToWait + 1); }

    public:
        virtual char *GetSignature() = 0;
        int GetResurrectTime() { return ResurrectTime; }
        int GetFramesetIdx() { return FramesetIdx; }
        virtual void ProcessBulletHit();

        Enemy(Sprite *framesource, DirType dir, bool isvalid, int framesetidx = -1) : GameCharacter(framesource, dir, isvalid)
        {
            FramesetIdx = framesetidx;
            SetResurrectTime();
        }
//        Enemy(GameCharacter *source) : GameCharacter(source)//Enemy(const Sprite *source, DirType dir, StateType state, int RoomZ = 0, int RoomX = 0, int RoomY = 0) : GameCharacter(name, dir, RoomZ, RoomX, RoomY), LastDoorTryTime(0)
  //      {
     //   }

        void Update();
        //void Resurrect();
};
//---------------------------------------------------------------------------
#endif

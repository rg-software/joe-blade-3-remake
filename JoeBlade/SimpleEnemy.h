//---------------------------------------------------------------------------
#ifndef SimpleEnemyH
#define SimpleEnemyH

#include "Enemy.h"
#include "GameCharacter.h"
#include <Math.hpp>
//---------------------------------------------------------------------------
class SimpleEnemy : public Enemy
{
    private:
        int LastDoorTryTime;

    protected:
        virtual void MakeDecision();

    public:
        virtual char *GetSignature()
        {
            static char* Signature = "SimpleEnemy";
            return Signature;
        }
        SimpleEnemy(Sprite *framesource, DirType dir, bool isvalid, int framesetidx) : Enemy(framesource, dir, isvalid, framesetidx), LastDoorTryTime(GameTimerTicks) {}
};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#ifndef ArmedEnemyH
#define ArmedEnemyH
//---------------------------------------------------------------------------
#include "CleverEnemy.h"
#include "GameCharacter.h"
#include <Math.hpp>
//---------------------------------------------------------------------------
class ArmedEnemy : public CleverEnemy
{
    private:
        int LastBulletTryTime;

    protected:
        virtual void MakeDecision();

    public:
        ArmedEnemy(Sprite *framesource, DirType dir, bool isvalid, int framesetidx) : CleverEnemy(framesource, dir, isvalid, framesetidx) {}
        virtual char *GetSignature()
        {
            static char *Signature = "ArmedEnemy";
            return Signature;
        }
};
//---------------------------------------------------------------------------
#endif

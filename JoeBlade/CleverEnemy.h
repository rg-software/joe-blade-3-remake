//---------------------------------------------------------------------------
#ifndef CleverEnemyH
#define CleverEnemyH
//---------------------------------------------------------------------------
#include "SimpleEnemy.h"
#include "GameCharacter.h"
#include <Math.hpp>
//---------------------------------------------------------------------------
class CleverEnemy : public SimpleEnemy
{
    private:
        int LastChDirTryTime;

    protected:
        virtual void MakeDecision();
//        virtual void ProcessFarFromJoeEvent();

    public:
        virtual char *GetSignature()
        {
            static char *Signature = "CleverEnemy";
            return Signature;
        }
        
        CleverEnemy(Sprite *framesource, DirType dir, bool isvalid, int framesetidx) : SimpleEnemy(framesource, dir, isvalid, framesetidx), LastChDirTryTime(GameTimerTicks) {}
};
//---------------------------------------------------------------------------
#endif

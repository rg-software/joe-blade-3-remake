//---------------------------------------------------------------------------

#ifndef CraxBloodfingerH
#define CraxBloodfingerH
//---------------------------------------------------------------------------
#include "Enemy.h"
#include "GameCharacter.h"
#include <Math.hpp>
//---------------------------------------------------------------------------
class CraxBloodfinger : public Enemy
{
    private:
        int LastDoorTryTime;
        void ProcessStep();

    protected:
        virtual void MakeDecision();
        virtual void ProcessFarFromJoeEvent();

    public:
        virtual char *GetSignature()
        {
            static char *Signature = "CraxBloodfinger";
            return Signature;
        }
        virtual void ProcessBulletHit();
        CraxBloodfinger(Sprite *framesource, DirType dir, bool isvalid) : Enemy(framesource, dir, isvalid), LastDoorTryTime(GameTimerTicks) {}
};
//---------------------------------------------------------------------------
#endif

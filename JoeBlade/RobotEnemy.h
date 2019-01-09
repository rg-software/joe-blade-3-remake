//---------------------------------------------------------------------------

#ifndef RobotEnemyH
#define RobotEnemyH

#include "CleverEnemy.h"

//---------------------------------------------------------------------------
class RobotEnemy : public CleverEnemy
{
    public:
        RobotEnemy(Sprite *framesource, DirType dir, bool isvalid, int framesetidx) : CleverEnemy(framesource, dir, isvalid, framesetidx) {}
        virtual char *GetSignature()
        {
            static char *Signature = "RobotEnemy";
            return Signature;
        }
};
//---------------------------------------------------------------------------
#endif

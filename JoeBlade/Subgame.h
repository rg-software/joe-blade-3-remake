//---------------------------------------------------------------------------
#ifndef SubgameH
#define SubgameH

#include <vector>
#include "Globals.h"

using namespace std;

//---------------------------------------------------------------------------
const int MaxOperations = 30;
const int MinOperations = 10;
const int TotalTime = 60000;
//---------------------------------------------------------------------------
class Subgame
{
    protected:
        vector<int> numbers;
        vector<bool> highlights;

        Subgame() : highlights(8) {}

        void PrepareNumbers();
        bool RunKernel();

        virtual char *GetKeysMessage() = 0;
        virtual bool IsWinningConfiguration() = 0;
        virtual void SubgameTimerFun() = 0;
        virtual void LeftOperation() = 0;
        virtual void RightOperation() = 0;
        virtual void FireOperation() = 0;
};
//---------------------------------------------------------------------------
#endif

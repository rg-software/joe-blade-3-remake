//---------------------------------------------------------------------------
#ifndef CraxSubgameH
#define CraxSubgameH

#include "Subgame.h"
//---------------------------------------------------------------------------
class TCraxSubgame : public Subgame
{
        virtual char *GetKeysMessage() {  static char s[] = "use left, right and fire"; return s; }
        virtual bool IsWinningConfiguration() { return (numbers[0] == 1 && numbers[1] == 2 && numbers[2] == 3 && numbers[3] == 4); }
        virtual void SubgameTimerFun();
        virtual void LeftOperation();
        virtual void RightOperation();
        virtual void FireOperation();

    public:
        void Run();
};
//---------------------------------------------------------------------------
void CraxSubgame();
//---------------------------------------------------------------------------
#endif


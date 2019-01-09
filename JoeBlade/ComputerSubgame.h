//---------------------------------------------------------------------------
#ifndef ComputerSubgameH
#define ComputerSubgameH

#include "Subgame.h"

//---------------------------------------------------------------------------
class TComputerSubgame : public Subgame
{
        virtual char *GetKeysMessage() {  static char s[] = "use left and right"; return s; }
        virtual bool IsWinningConfiguration() { return (numbers[0] == 1 && numbers[1] == 2 && numbers[2] == 3 && numbers[3] == 4) ||
                                                       (numbers[7] == 1 && numbers[6] == 2 && numbers[5] == 3 && numbers[4] == 4); }
        virtual void SubgameTimerFun();
        virtual void LeftOperation();
        virtual void RightOperation();
        virtual void FireOperation();

        void ShowObjectsMenu();
    public:
        void Run();
};
//---------------------------------------------------------------------------
void ComputerSubgame();
//---------------------------------------------------------------------------
#endif

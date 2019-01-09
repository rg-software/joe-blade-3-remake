//---------------------------------------------------------------------------
#ifndef SecuritySubgameH
#define SecuritySubgameH

#include "Subgame.h"
//---------------------------------------------------------------------------
class TSecuritySubgame : public Subgame
{
        virtual char *GetKeysMessage() {  static char s[] = "use left, right and fire"; return s; }
        virtual TGameStatus GetDefeatStatus() { return GAMEOVER; }
        virtual bool IsWinningConfiguration() { return (numbers[0] == 1 && numbers[1] == 2 && numbers[2] == 3 && numbers[3] == 4) ||
                                                       (numbers[7] == 1 && numbers[6] == 2 && numbers[5] == 3 && numbers[4] == 4); }
        virtual void SubgameTimerFun() { }
        virtual void LeftOperation();
        virtual void RightOperation();
        virtual void FireOperation();

    public:
        void Run();
};
//---------------------------------------------------------------------------
void SecuritySubgame();
//---------------------------------------------------------------------------
#endif

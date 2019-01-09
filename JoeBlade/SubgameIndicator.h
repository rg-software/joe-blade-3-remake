//---------------------------------------------------------------------------
#ifndef SubgameIndicatorH
#define SubgameIndicatorH

#include <vector>
#include <sdl.h>
#include "Utils.h"

using namespace std;
//---------------------------------------------------------------------------
class TSubgameIndicator
{
    SDL_Surface *normal_nums;
    SDL_Surface *light_nums;
    int IndicatorX, IndicatorY;

    public:
        TSubgameIndicator();
        ~TSubgameIndicator()
        {
            SDL_FreeSurface(normal_nums);
            SDL_FreeSurface(light_nums);
        }
        int CharW() { return normal_nums->w / 4; }
        int CharH() { return normal_nums->h; }

        void Print(const vector<int>& numbers, const vector<bool>& highlights);
};
//---------------------------------------------------------------------------
TSubgameIndicator& SubgameIndicator();
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
#pragma hdrstop

#include "SubgameIndicator.h"
#include "Config.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
TSubgameIndicator::TSubgameIndicator()
{
    normal_nums = LoadSurfaceFromGIFResource("Common\\normal_nums.gif");
    light_nums = LoadSurfaceFromGIFResource("Common\\light_nums.gif");

    IndicatorX = Config::Xoffset + Config::GridSize + (Config::RoomWidth - Config::GridSize) / 2 - 2 * CharW();
    IndicatorY = Config::Yoffset + Config::RoomHeight / 2 - CharH();
}
//---------------------------------------------------------------------------
void TSubgameIndicator::Print(const vector<int>& numbers, const vector<bool>& highlights)
{
    SDL_Rect r;
    r.x = Config::Xoffset + Config::GridSize; r.y = Config::Yoffset;
    r.w = Config::RoomWidth - Config::GridSize; r.h = Config::RoomHeight;
    SDL_FillRect(Config::BackBuffer, &r, 0);

    int x[] = { IndicatorX, IndicatorX + CharW(), IndicatorX + 2 * CharW(), IndicatorX + 3 * CharW(),
                IndicatorX + 3 * CharW(), IndicatorX + 2 * CharW(), IndicatorX + CharW(), IndicatorX };
    int y[] = { IndicatorY, IndicatorY, IndicatorY, IndicatorY,
                IndicatorY + CharH(), IndicatorY + CharH(), IndicatorY + CharH(), IndicatorY + CharH() };

    SDL_Rect src, dest;

    for(unsigned i = 0; i < numbers.size(); i++)
    {
        SDL_Surface *p = (highlights[i] == true) ? light_nums : normal_nums;

        src.x = (numbers[i] - 1) * CharW();
        src.y = 0;
        src.w = CharW();
        src.h = CharH();
        dest.x = x[i]; dest.y = y[i];

        My_SDL_BlitSurface(p, &src, Config::BackBuffer, &dest);
    }
}
//---------------------------------------------------------------------------
TSubgameIndicator& SubgameIndicator()
{
    static TSubgameIndicator indicator;

    return indicator;
}
//---------------------------------------------------------------------------

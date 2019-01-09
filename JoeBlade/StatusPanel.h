//---------------------------------------------------------------------------
#ifndef StatusPanelH
#define StatusPanelH

#include <sdl.h>
#include <map>
#include <utility>

using namespace std;

//---------------------------------------------------------------------------
class TStatusPanel
{
    SDL_Surface *numbers;
    SDL_Surface *bullets;
    SDL_Surface *hearts;
    map<int, pair<int, int> > coords;

    void PrintNumber(int number, int x, int y, int width);

    public:
        TStatusPanel();
        void Show();
        ~TStatusPanel()
        {
            SDL_FreeSurface(numbers);
            SDL_FreeSurface(bullets);
            SDL_FreeSurface(hearts);
        }
};
//---------------------------------------------------------------------------
TStatusPanel& StatusPanel();
//---------------------------------------------------------------------------
#endif

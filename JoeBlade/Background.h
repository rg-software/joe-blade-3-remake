//---------------------------------------------------------------------------
#ifndef BackgroundH
#define BackgroundH

#include <SDL.h>
#include "Utils.h"

//---------------------------------------------------------------------------
class TBackground
{
    SDL_Surface *graphics;

    public:
        TBackground();
        ~TBackground() { SDL_FreeSurface(graphics); }

        void Show();
//        void FastShow();
        void Show(SDL_Rect *r);
};
//---------------------------------------------------------------------------
TBackground& Background();
//---------------------------------------------------------------------------
#endif

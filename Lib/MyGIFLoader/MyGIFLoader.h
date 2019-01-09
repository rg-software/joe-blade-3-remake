//---------------------------------------------------------------------------

#ifndef MyGIFLoaderH
#define MyGIFLoaderH

#include <sdl.h>
#include <vcl.h>
//---------------------------------------------------------------------------

class MyGIFStructure
{
    private:
        void Initialize(TStream *stream);

    public:
        int NumberOfFrames;
        int *FrameDelay;
        SDL_Surface **Frames;

        MyGIFStructure(TStream *stream);
        MyGIFStructure(char *FileName);
        ~MyGIFStructure();
};
//---------------------------------------------------------------------------
#endif

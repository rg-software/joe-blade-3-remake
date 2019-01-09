#include "MyGIFLoader.h"
#include <GIFImage.hpp>
#include <SDL_Image.h>
//---------------------------------------------------------------------------
MyGIFStructure::MyGIFStructure(TStream *stream)
{
    Initialize(stream);
}
//---------------------------------------------------------------------------
MyGIFStructure::MyGIFStructure(char *FileName)
{
    TFileStream *stream = new TFileStream(FileName, fmOpenRead);
    Initialize(stream);
    delete stream;
}
//---------------------------------------------------------------------------
MyGIFStructure::~MyGIFStructure()
{
    delete[] FrameDelay;
    for(int i = 0; i < NumberOfFrames; i++)
        SDL_FreeSurface(Frames[i]);
    delete[] Frames;
}
//---------------------------------------------------------------------------
void MyGIFStructure::Initialize(TStream *stream)
{
    TGIFImage *gif = new TGIFImage;

    gif->LoadFromStream(stream);
    NumberOfFrames = gif->Images->Count;
    FrameDelay = new int[NumberOfFrames];
    Frames = new SDL_Surface*[NumberOfFrames];

    TMemoryStream *s = new TMemoryStream();
    for(int i = 0; i < NumberOfFrames; i++)
    {
        FrameDelay[i] = (NumberOfFrames == 1) ? 0 : (10 * gif->Images->SubImages[i]->GraphicControlExtension->Delay);

        gif->Images->SubImages[i]->Bitmap->SaveToStream(s);
        char *buffer = new char[s->Size];
        s->Seek(0, soFromBeginning);
        s->ReadBuffer(buffer, s->Size);

        SDL_RWops *rwop = SDL_RWFromMem(buffer, s->Size);
        Frames[i] = IMG_LoadBMP_RW(rwop);
        SDL_FreeRW(rwop);

        delete[] buffer;
        s->Clear();
    }

    delete gif;
    delete s;
}
//---------------------------------------------------------------------------

/*
int main(int argc, char*argv[])
{
    MyGIFStructure r = MyGIFLoadFromFile("grass1.gif");
    return 0;
} */
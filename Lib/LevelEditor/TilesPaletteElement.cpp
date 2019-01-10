//---------------------------------------------------------------------------


#pragma hdrstop

#include "TilesPaletteElement.h"
#include <GIFimg.hpp>
#include "constants.h"
#include <set>

using namespace std;
//---------------------------------------------------------------------------

#pragma package(smart_init)

extern TilesPaletteElement *ActiveElement;
extern set<TilesPaletteElement *> AnimatedTiles;
//---------------------------------------------------------------------------
TilesPaletteElement::TilesPaletteElement(int GroupID, const AnsiString& FileName, TComponent *AOwner)  : TSpeedButton(AOwner), Path(FileName)
{
    TGIFImage *gif = new TGIFImage;
    TMemoryStream *p = new TMemoryStream;
    theimage = new TImage(this);

    gif->LoadFromFile(FileName);
    gif->Images->Frames[0]->Bitmap->SaveToStream(p);

    Height = gif->Height + 10;
    Width = gif->Width + 10;
    Glyph->Height = gif->Height;
    Glyph->Width = gif->Width;
    p->Seek(0, soFromBeginning);
    Glyph->LoadFromStream(p);
    Glyph->Transparent = true;
    Glyph->TransparentMode = tmFixed;
    Glyph->TransparentColor = TRANSPARENTCOLOR;
    p->Seek(0, soFromBeginning);
    theimage->Picture->Bitmap->LoadFromStream(p);
    theimage->Transparent = true;
    theimage->Picture->Bitmap->TransparentColor = TRANSPARENTCOLOR;
    Flat = true;
    GroupIndex = GroupID;
    Hint = Path.SubString(1, Path.Length() - 4);
    ShowHint = true;
    OnClick = ClickOccured;

    //---- load frames
    numberofframes = gif->Images->Count;
    if(numberofframes > 1)
    {
        currentframe = 0;
        frames = new TImage*[numberofframes];
        for(int i = 0; i < numberofframes; i++)
        {
            frames[i] = new TImage(Application);
            frames[i]->Transparent = true;
            frames[i]->Picture->Bitmap->TransparentMode = tmFixed;
            frames[i]->Picture->Bitmap->TransparentColor = TRANSPARENTCOLOR;
            frames[i]->Picture->Bitmap->Assign(gif->Images->Frames[i]->Bitmap);
        }
        AnimatedTiles.insert(this);
    }
    //----------------

    delete gif;
    delete p;
}
//---------------------------------------------------------------------------
__fastcall TilesPaletteElement::~TilesPaletteElement()
{
/*    if(numberofframes > 1)
    {
        for(int i = 0; i < numberofframes; i++)
            delete frames[i];
        delete[] frames;
    }*/
    delete theimage;
}
//---------------------------------------------------------------------------
void __fastcall TilesPaletteElement::ClickOccured(TObject *Sender)
{
    ActiveElement = this;
}
//---------------------------------------------------------------------------
Graphics::TBitmap *TilesPaletteElement::GetBitmap()
{
    return theimage->Picture->Bitmap;
}
//---------------------------------------------------------------------------
void TilesPaletteElement::SwitchFrame()
{
    if(numberofframes > 1)
    {
        if(++currentframe == numberofframes)
            currentframe = 0;
        theimage->Picture->Bitmap->Assign(frames[currentframe]->Picture->Bitmap);
    }
}
//---------------------------------------------------------------------------


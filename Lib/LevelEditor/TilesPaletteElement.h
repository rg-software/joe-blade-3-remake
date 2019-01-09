//---------------------------------------------------------------------------

#ifndef TilesPaletteElementH
#define TilesPaletteElementH

#include <vcl.h>
#include <buttons.hpp>
//---------------------------------------------------------------------------
class TilesPaletteElement : public TSpeedButton
{
    private:
        TImage **frames;
        int numberofframes;
        int currentframe;
        TImage *theimage;
        void __fastcall ClickOccured(TObject *Sender);
        
    public:
        AnsiString Path;
        TilesPaletteElement(int GroupID, const AnsiString& FileName, TComponent *AOwner);
        __fastcall ~TilesPaletteElement();
        Graphics::TBitmap *GetBitmap();
        void SwitchFrame();
};
//---------------------------------------------------------------------------
#endif

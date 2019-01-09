//---------------------------------------------------------------------------

#ifndef MyPaintBoxH
#define MyPaintBoxH

#include <vcl.h>
#include "MainUnit.h"
//---------------------------------------------------------------------------
class MyPaintBox : public TPaintBox
{
    private:
        bool MouseOverControl;
        void __fastcall MyPaintBoxOnMouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int Y);
        void __fastcall MyPaintBoxOnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

    public:
        MyPaintBox();
        void __fastcall MyPaintBoxOnPaint(System::TObject* Sender);
        void __fastcall WndProc(TMessage& Message);
        bool IsMouseOverControl();
};
//---------------------------------------------------------------------------
#endif

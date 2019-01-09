//---------------------------------------------------------------------------


#pragma hdrstop

#include "MyPaintBox.h"
#include "TilesPalette.h"
#include "TilesPaletteElement.h"
#include "MainUnit.h"
#include "constants.h"
#include <vector>

using namespace std;

//---------------------------------------------------------------------------

#pragma package(smart_init)

extern void RepaintBackBuffer();
extern int MouseX, MouseY;
extern bool CacheIsValid;
extern vector<vector<vector<TilesPaletteElement *> > > Level;   // The Level[layer][x][y]
extern void RepaintLayers();
extern void UpdateScrollBars();
extern void ResizeLevel(int layer, int width, int height);
//---------------------------------------------------------------------------
void __fastcall MyPaintBox::MyPaintBoxOnPaint(System::TObject* Sender)  // On Paint event handler
{
    RepaintBackBuffer();
    Canvas->CopyRect(Rect(0, 0, Width, Height), MainForm->BackBuffer->Canvas, Rect(0, 0, Width, Height));
}
//---------------------------------------------------------------------------
// mouse down handler
void __fastcall MyPaintBox::MyPaintBoxOnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int MapX = (MouseX >> 5);
    int MapY = (MouseY >> 5);
    int layer = MainForm->LayersListBox->ItemIndex;

    int x = MapX + MainForm->HScrollBar->Position;
    int y = MapY + MainForm->VScrollBar->Position;

    if(x >= Level[layer].size())
        ResizeLevel(layer, x + 1, Level[layer][0].size());
    if(y >= Level[layer][0].size())
        ResizeLevel(layer, Level[layer].size(), y + 1);
    UpdateScrollBars();

    TilesPaletteElement *ActivePaletteElement = TilesPalette::GetActiveElement();

    if(Button == mbLeft && !TilesPalette::IsArrow(ActivePaletteElement)) // some element selected
    {
        Level[layer][x][y] = ActivePaletteElement;
    }
    else if(Button == mbLeft && TilesPalette::IsArrow(ActivePaletteElement) && Level[layer][x][y] != NULL)
    {
        TilesPalette::SetActiveElement(Level[layer][x][y]);
        Level[layer][x][y] = NULL;
    }
    else if(Button == mbRight && TilesPalette::IsArrow(ActivePaletteElement) && Level[layer][x][y] != NULL) // something under the cursor - remove it!
    {
        Level[layer][x][y] = NULL;
    }
    else if(Button == mbRight && !TilesPalette::IsArrow(ActivePaletteElement))
    {
        TilesPalette::SelectArrow();
    }

    CacheIsValid = false;
    MyPaintBoxOnPaint(0);
}
//---------------------------------------------------------------------------
void __fastcall MyPaintBox::MyPaintBoxOnMouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int Y)
{
    MouseX = X; MouseY = Y;
    MyPaintBoxOnPaint(Sender);
}
//---------------------------------------------------------------------------
MyPaintBox::MyPaintBox() : TPaintBox(Application), MouseOverControl(false)
{
    Parent = MainForm;
    Width = MainForm->BackBuffer->Width;
    Height = MainForm->BackBuffer->Height;
    OnPaint = MyPaintBoxOnPaint;
    OnMouseMove = MyPaintBoxOnMouseMove;
    OnMouseDown = MyPaintBoxOnMouseDown;
}
//---------------------------------------------------------------------------
void __fastcall MyPaintBox::WndProc(TMessage& Message)         // OnMouseEnter/OnMouseLeave handler
{
    if(Message.Msg == CM_MOUSELEAVE)
    {
        MouseOverControl = false;
        MyPaintBoxOnPaint(0);
    }
    else if(Message.Msg == CM_MOUSEENTER)
    {
        MouseOverControl = true;
        MyPaintBoxOnPaint(0);
    }
    else
        TPaintBox::WndProc(Message);
}
//---------------------------------------------------------------------------
bool MyPaintBox::IsMouseOverControl()
{
    return MouseOverControl;
}
//---------------------------------------------------------------------------



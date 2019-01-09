//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"
#include "TilesPalette.h"
#include "TilesPaletteElement.h"
#include "MyPaintBox.h"
#include "constants.h"
#include <vector>
#include <map>
#include <list>
#include <set>
#include <stdio.h>

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TMainForm *MainForm;
MyPaintBox *PaintArea;                                                // main screen
vector<vector<vector<TilesPaletteElement *> > > Level(N_of_layers);   // The Level[layer][x][y]
vector<double> Coeffs;                                                // Scroll coeffs
bool CacheIsValid = false;                                            // cache validity flag
TImage *cache = new TImage(Application);                              // cache screen
int MouseX, MouseY;                                                   // current mouse coords
extern set<TilesPaletteElement *> AnimatedTiles;
extern vector<list<TilesPaletteElement *> > PaletteImages;      // palette tabs (vector of tabs)
//---------------------------------------------------------------------------
void ResizeLevel(int layer, int width, int height)         // resize given layer of the level
{
    Level[layer].resize(width);

    for(int i = 0; i < width; i++)
        Level[layer][i].resize(height);
}
//---------------------------------------------------------------------------
void UpdateScrollBars()                                    // resize scrollbars in accordance to layer size
{
    MainForm->HScrollBar->Max = Level[MainForm->LayersListBox->ItemIndex].size() + 640 / 32;
    MainForm->VScrollBar->Max = Level[MainForm->LayersListBox->ItemIndex][0].size() + 480 / 32;
}
//---------------------------------------------------------------------------
void RepaintLayers()
{
    if(CacheIsValid)
        MainForm->BackBuffer->Canvas->CopyRect(Rect(0, 0, PaintArea->Width, PaintArea->Height), cache->Canvas, Rect(0, 0, PaintArea->Width, PaintArea->Height));
    else
    {
        int CurrentLayer = MainForm->LayersListBox->ItemIndex;
        
        for(int curlayer = 0; curlayer < MainForm->LayersListBox->Items->Count; curlayer++)
            if(MainForm->LayersListBox->Checked[curlayer])  // draw this layer
            {
                double denom = Coeffs[MainForm->LayersListBox->ItemIndex];
                double CurCoeff = Coeffs[curlayer] /  (denom ? denom : 0.01);

                int dx = CurCoeff * MainForm->HScrollBar->Position;
                int dy = CurCoeff * MainForm->VScrollBar->Position;
                int odx = 32 * (CurCoeff * MainForm->HScrollBar->Position - dx);
                int ody = 32 * (CurCoeff * MainForm->VScrollBar->Position - dy);

                int minx = (dx - Xmargin) > 0 ? (dx - Xmargin) : 0;
                int miny = (dy - Ymargin) > 0 ? (dy - Ymargin) : 0;
                int xr = dx + 640 / 32 + 1;
                int yr = dy + 480/32 + 1;

                int maxx = xr < Level[curlayer].size() ? xr : Level[curlayer].size() - 1;
                int maxy = yr < Level[curlayer][0].size() ? yr : Level[curlayer][0].size() - 1;

                MainForm->BackBuffer->Canvas->Brush->Color = TRANSPARENTCOLOR;
                bool highlighting = (curlayer == CurrentLayer) && MainForm->HighlightCheckBox->Checked;

                for(int x = minx; x <= maxx; x++)
                    for(int y = miny; y <= maxy; y++)
                        if(Level[curlayer][x][y] != NULL)
                        {
                            int cx = ((x - dx) << 5) - odx;
                            int cy = ((y - dy) << 5) - ody;

                            MainForm->BackBuffer->Canvas->Draw(cx, cy, Level[curlayer][x][y]->GetBitmap());
                            if(highlighting)
                                MainForm->BackBuffer->Canvas->FrameRect(Rect(cx, cy, cx + Level[curlayer][x][y]->GetBitmap()->Width, cy + Level[curlayer][x][y]->GetBitmap()->Height));
                        }
            }

            CacheIsValid = true;
            cache->Canvas->CopyRect(Rect(0, 0, PaintArea->Width, PaintArea->Height), MainForm->BackBuffer->Canvas, Rect(0, 0, PaintArea->Width, PaintArea->Height));
    }
}
//---------------------------------------------------------------------------
void RepaintBackBuffer()
{
    MainForm->BackBuffer->Canvas->Brush->Color = clWhite;   // clear backbuffer
    MainForm->BackBuffer->Canvas->FillRect(Rect(0, 0, MainForm->BackBuffer->Width, MainForm->BackBuffer->Height));

    RepaintLayers();

    if(PaintArea->IsMouseOverControl())
    {
        TilesPaletteElement *ActivePaletteElement = TilesPalette::GetActiveElement();

        if(!TilesPalette::IsArrow(ActivePaletteElement)) // draw current element
        {
            int X = (MouseX >> 5) << 5;
            int Y = (MouseY >> 5) << 5;
            MainForm->BackBuffer->Canvas->Draw(X, Y, ActivePaletteElement->GetBitmap());
            MainForm->BackBuffer->Canvas->Brush->Color = TRANSPARENTCOLOR;
            MainForm->BackBuffer->Canvas->FrameRect(Rect(X, Y, X + ActivePaletteElement->GetBitmap()->Width + 1, Y + ActivePaletteElement->GetBitmap()->Height + 1));
        }
        else    // draw a frame around the element beneath the cursor
        {
            int MapX = (MouseX >> 5);
            int MapY = (MouseY >> 5);
            int layer = MainForm->LayersListBox->ItemIndex;
            int x = MapX + MainForm->HScrollBar->Position;
            int y = MapY + MainForm->VScrollBar->Position;

            if(x < Level[layer].size() && y < Level[layer][0].size() && Level[layer][x][y] != NULL)
            {
                Graphics::TBitmap *p = Level[layer][x][y]->GetBitmap();

                MainForm->BackBuffer->Canvas->Brush->Color = TRANSPARENTCOLOR;
                MainForm->BackBuffer->Canvas->FrameRect(Rect(MapX << 5, MapY << 5, (MapX << 5) + p->Width,  (MapY << 5) + p->Height));
            }
        }
    }

    // show grid
    if(MainForm->ShowGridCheckBox->Checked)
        for(int i = 0; i <= MainForm->BackBuffer->Width; i += 32)
            for(int j = 0; j <= MainForm->BackBuffer->Height; j += 32)
            {
                MainForm->BackBuffer->Canvas->Pixels[i][j] = TRANSPARENTCOLOR;
                MainForm->BackBuffer->Canvas->Pixels[i + 1][j] = TRANSPARENTCOLOR;
                MainForm->BackBuffer->Canvas->Pixels[i][j + 1] = TRANSPARENTCOLOR;
                MainForm->BackBuffer->Canvas->Pixels[i + 1][j + 1] = TRANSPARENTCOLOR;
            }
}
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
    TilesPalette::LoadFromDir("Tiles");
    TilesPalette::SetActiveTab(0);
    PaintArea = new MyPaintBox;
    LayersListBox->Items->Clear();
    Coeffs.resize(N_of_layers);
    for(int i = 0; i < N_of_layers; i++)
    {
        LayersListBox->Items->Add("слой " + AnsiString(i));
        LayersListBox->Checked[i] = true;
        Coeffs[i] = 1;
        ResizeLevel(i, 1, 1);
    }
    LayersListBox->ItemIndex = 0;
    LayersListBox->ClientHeight = LayersListBox->ItemHeight * LayersListBox->Items->Count;
    ::UpdateScrollBars();
    cache->Width = BackBuffer->Width;
    cache->Height = BackBuffer->Height;
    AnimatedTilesTimer->Interval = AnimationDelay;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TilesPaletteCtrlChange(TObject *Sender)
{
    int hpos = TilesScrollBox->HorzScrollBar->ScrollPos;
    int vpos = TilesScrollBox->VertScrollBar->ScrollPos;
    TilesScrollBox->ScrollBy(hpos, vpos);

    TilesPalette::SetActiveTab(MainForm->TilesPaletteCtrl->TabIndex);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::HScrollBarChange(TObject *Sender)
{
    CacheIsValid = false;
    PaintArea->MyPaintBoxOnPaint(0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::VScrollBarChange(TObject *Sender)
{
    CacheIsValid = false;
    PaintArea->MyPaintBoxOnPaint(0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LayersListBoxClickCheck(TObject *Sender)
{
    LayersListBoxClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LayersListBoxClick(TObject *Sender)
{
    CacheIsValid = false;
    ::UpdateScrollBars();
    PaintArea->MyPaintBoxOnPaint(0);
    CoeffLabel->Text = Coeffs[LayersListBox->ItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MinusButtonClick(TObject *Sender)
{
    double v = CoeffLabel->Text.ToDouble();

    v -= 0.1;
    if(v < 0)
        v = 0;

    CoeffLabel->Text = v;
    Coeffs[LayersListBox->ItemIndex] = v;
    CacheIsValid = false;
    PaintArea->MyPaintBoxOnPaint(0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::PlusButtonClick(TObject *Sender)
{
    double v = CoeffLabel->Text.ToDouble();

    v += 0.1;

    CoeffLabel->Text = v;
    Coeffs[LayersListBox->ItemIndex] = v;
    CacheIsValid = false;
    PaintArea->MyPaintBoxOnPaint(0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OpenButtonClick(TObject *Sender)
{
    if(!OpenDialog->Execute())
        return;

    int n_of_tiles;
    map<WORD, TilesPaletteElement*> Elements;
    FILE* infile = fopen(OpenDialog->FileName.c_str(), "rb");
    char str[256];

    fread(&n_of_tiles, sizeof(int), 1, infile);     // number of tiles
    for(int i = 0; i < n_of_tiles; i++)        // convert IDs to pointers
    {
        int k = 0;
        WORD id;

        while((str[k++] = fgetc(infile)) != 0)
            ;
        str[k] = 0;

        fread(&id, sizeof(WORD), 1, infile);

        if(Elements[id] == NULL)
            Elements[id] = TilesPalette::GetElementByPath(str);
    }

    // load layers
    for(int layer = 0; layer < N_of_layers; layer++)
    {
        int s, s2;

        fread(&Coeffs[layer], sizeof(double), 1, infile);   // coeff
        fread(&s, sizeof(int), 1, infile);
        fread(&s2, sizeof(int), 1, infile);

        ResizeLevel(layer, s, s2);

        for(int i = 0; i < Level[layer].size(); i++)
            for(int j = 0; j < Level[layer][0].size(); j++)
            {
                int c = fgetc(infile);

                Level[layer][i][j] = (c == 0) ? 0 : Elements[c];
            }
    }

    fclose(infile);
    CacheIsValid = false;

    LayersListBox->ItemIndex = 0;
    ::UpdateScrollBars();

    PaintArea->MyPaintBoxOnPaint(0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveButtonClick(TObject *Sender)
{
    if(!SaveDialog->Execute())
        return;

    map<AnsiString, WORD> IDs;
    WORD lastid = 1;

    for(int layer = 0; layer < N_of_layers; layer++)        // for all layers
    {
        for(int i = 0; i < Level[layer].size(); i++)
            for(int j = 0; j < Level[layer][0].size(); j++)      // exam all elements
                if(Level[layer][i][j] != NULL)
                    if(IDs[Level[layer][i][j]->Path] == 0)         // insert Path to IDs map
                        IDs[Level[layer][i][j]->Path] = lastid++;
    }

    FILE* outfile = fopen(SaveDialog->FileName.c_str(), "wb");

    int n_of_tiles = IDs.size();
    fwrite(&n_of_tiles, sizeof(int), 1, outfile);     // number of tiles

    for(map<AnsiString, WORD>::iterator p = IDs.begin(); p != IDs.end(); p++)  // write <string, word> pairs
    {
        fwrite(p->first.c_str(), p->first.Length() + 1, 1, outfile);
        fwrite(&(p->second), sizeof(WORD), 1, outfile);
    }

//    fwrite(&N_of_layers, sizeof(int), 1, outfile);       // Number of layers

    for(int layer = 0; layer < N_of_layers; layer++)
    {
        int s = Level[layer].size();
        int s2 = Level[layer][0].size();

        fwrite(&Coeffs[layer], sizeof(double), 1, outfile);   // coeff
        fwrite(&s, sizeof(int), 1, outfile);
        fwrite(&s2, sizeof(int), 1, outfile);

        for(int i = 0; i < Level[layer].size(); i++)
            for(int j = 0; j < Level[layer][0].size(); j++)
                if(Level[layer][i][j] == NULL)
                    fputc(0, outfile);
                else
                    fputc(IDs[Level[layer][i][j]->Path], outfile);
    }

    fclose(outfile);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ShowGridCheckBoxClick(TObject *Sender)
{
    PaintArea->MyPaintBoxOnPaint(0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HighlightCheckBoxClick(TObject *Sender)
{
    CacheIsValid = false;
    PaintArea->MyPaintBoxOnPaint(0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormKeyPress(TObject *Sender, char &Key)
{
    if(Key == VK_ESCAPE)
    {
        TilesPalette::SelectArrow();
        PaintArea->MyPaintBoxOnPaint(0);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AnimationCheckBoxClick(TObject *Sender)
{
    AnimatedTilesTimer->Enabled = AnimationCheckBox->Checked;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::AnimatedTilesTimerTimer(TObject *Sender)
{
    for(set<TilesPaletteElement *>::iterator p = AnimatedTiles.begin(); p != AnimatedTiles.end(); p++)
        (*p)->SwitchFrame();
    CacheIsValid = false;
    PaintArea->MyPaintBoxOnPaint(0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    AnimatedTilesTimer->Enabled = false;    
}
//---------------------------------------------------------------------------




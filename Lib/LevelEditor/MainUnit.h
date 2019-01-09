//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <CheckLst.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TImage *BackBuffer;
    TScrollBar *VScrollBar;
    TScrollBar *HScrollBar;
    TTabControl *TilesPaletteCtrl;
    TScrollBox *TilesScrollBox;
    TCheckListBox *LayersListBox;
    TEdit *CoeffLabel;
    TLabel *Label1;
    TCheckBox *ShowGridCheckBox;
    TButton *PlusButton;
    TButton *MinusButton;
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
    TButton *OpenButton;
    TButton *SaveButton;
    TCheckBox *HighlightCheckBox;
    TTimer *AnimatedTilesTimer;
    TCheckBox *AnimationCheckBox;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall TilesPaletteCtrlChange(TObject *Sender);
    void __fastcall HScrollBarChange(TObject *Sender);
    void __fastcall VScrollBarChange(TObject *Sender);
    void __fastcall LayersListBoxClickCheck(TObject *Sender);
    void __fastcall LayersListBoxClick(TObject *Sender);
    void __fastcall MinusButtonClick(TObject *Sender);
    void __fastcall PlusButtonClick(TObject *Sender);
    void __fastcall OpenButtonClick(TObject *Sender);
    void __fastcall SaveButtonClick(TObject *Sender);
    void __fastcall ShowGridCheckBoxClick(TObject *Sender);
    void __fastcall HighlightCheckBoxClick(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall AnimationCheckBoxClick(TObject *Sender);
    void __fastcall AnimatedTilesTimerTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

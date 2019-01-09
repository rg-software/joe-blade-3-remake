//---------------------------------------------------------------------------

#ifndef ConfigureUnitH
#define ConfigureUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TCheckBox *cb_f;
    TCheckBox *cb_ie;
    TCheckBox *cb_ia;
    TCheckBox *cb_t;
    TCheckBox *cb_fps;
    TCheckBox *cb_dj;
    TCheckBox *cb_l;
    TCheckBox *cb_kd;
    TCheckBox *cb_ns;
    TCheckBox *cb_se;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TButton *btnSave;
    void __fastcall btnSaveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    void Check(const std::string& cbname);
    void LoadConfiguration();
    void SaveConfiguration();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

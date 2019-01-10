//---------------------------------------------------------------------------

#include <vcl.h>
#include <fstream>
#include <string>
#pragma hdrstop

#include "ConfigureUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
void TForm1::Check(const std::string& cbname)
{
    for(int i = 0; i < ComponentCount; ++i)
    {
        TCheckBox *p = dynamic_cast<TCheckBox*>(Components[i]);
        if(p != NULL && p->Name == cbname.c_str())
            p->Checked = true;
    }
}
//---------------------------------------------------------------------------
void TForm1::LoadConfiguration()
{
    std::ifstream is("JoeBlade3.cfg");
    std::string s;
    while(is >> s)
        Check("cb_" + s.substr(1));
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    LoadConfiguration();
}
//---------------------------------------------------------------------------
void TForm1::SaveConfiguration()
{
    std::ofstream os("JoeBlade3.cfg");

    for(int i = 0; i < ComponentCount; ++i)
    {
        TCheckBox *p = dynamic_cast<TCheckBox*>(Components[i]);
        if(p != NULL && p->Checked)
		{
			AnsiString ps = p->Name;
			std::string key = "/" + std::string(ps.c_str()).substr(3);
            os << key << " ";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnSaveClick(TObject *Sender)
{
    SaveConfiguration();
    Close();
}
//---------------------------------------------------------------------------




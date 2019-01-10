//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <list>
#pragma hdrstop

#include "MainUnit.h"

using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void LevOptMain();

void __fastcall TForm1::GoButtonClick(TObject *Sender)
{
    LevOptMain();
	Application->MessageBox(L"Done!", L"", 0);
}
//---------------------------------------------------------------------------
 
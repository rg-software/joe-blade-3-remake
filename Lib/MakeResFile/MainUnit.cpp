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

list<AnsiString> files;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void CreateFilesList(const AnsiString& InitDir)
{
    TSearchRec dir, file;      // current dir and current file

    if(FindFirst(InitDir + "\\*.*", faArchive, file) == 0)
    {
        do
        {
            AnsiString fullname = InitDir + "\\" + file.Name;
            AnsiString relname = fullname.SubString(Form1->Path->Text.Length() + 2, fullname.Length() - (Form1->Path->Text.Length() + 2) + 1);

            files.push_back(relname);
        }
        while(!FindNext(file));
        FindClose(file);
    }


    if(FindFirst(InitDir + "\\*.*", faDirectory, dir) == 0) // find subdirs
    {
        do
            if(dir.Name != "." && dir.Name != "..")
                CreateFilesList(InitDir + "\\" + dir.Name);
        while(!FindNext(dir));
    }
    FindClose(dir);
}
//---------------------------------------------------------------------------
void CreateResource(const AnsiString& ResFileName)
{
    files.clear();
    CreateFilesList(Form1->Path->Text);

    FILE *hdrfile = fopen((ResFileName + ".hdr").c_str(), "wt");
    FILE *resfile = fopen((ResFileName + ".res").c_str(), "wb");

    for(list<AnsiString>::iterator p = files.begin(); p != files.end(); p++)
	{
		AnsiString path = Form1->Path->Text + "\\" + *p;
		FILE *temp = fopen(path.c_str(), "rb");
        fseek(temp, 0, SEEK_END);
        int size = ftell(temp);
        fseek(temp, 0, SEEK_SET);

        char *buffer = new char[size];
        fread(buffer, size, 1, temp);
        fclose(temp);

        fprintf(hdrfile, "%s\n%d %d\n", p->c_str(), ftell(resfile), size);
        fwrite(buffer, size, 1, resfile);

        delete[] buffer;
    }

    fclose(hdrfile);
    fclose(resfile);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::GoButtonClick(TObject *Sender)
{
    AnsiString ResFileName;

    int i = Path->Text.Length();
    while(i >= 1 && Path->Text[i] != '\\')
        ResFileName = AnsiString(Path->Text[i--]) + ResFileName;

    CreateResource(ResFileName);

    Application->MessageBox(L"Done!", L"", 0);
}
//---------------------------------------------------------------------------
 
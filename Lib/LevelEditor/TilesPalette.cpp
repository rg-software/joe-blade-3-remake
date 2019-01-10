//---------------------------------------------------------------------------
#pragma hdrstop

#include "TilesPalette.h"
#include "TilesPaletteElement.h"
#include "MainUnit.h"

#include <vector>
#include <map>
#include <list>
#include <set>

using namespace std;

vector<list<TilesPaletteElement *> > PaletteImages;      // palette tabs (vector of tabs)
map<AnsiString, TilesPaletteElement *> Elements;        // all palette elements <filename, element*>
int ActiveTab = 0;
TilesPaletteElement *ActiveElement = NULL;
set<TilesPaletteElement *> AnimatedTiles;
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
/*void TilesPalette::UnsetActiveTab(int index)
{
    for(list<TilesPaletteElement *>::iterator p = PaletteImages[index].begin(); p != PaletteImages[index].end(); p++)
        (*p)->Parent = NULL;
} */
//---------------------------------------------------------------------------
void TilesPalette::SetActiveTab(int index)
{
    for(list<TilesPaletteElement *>::iterator p = PaletteImages[ActiveTab].begin(); p != PaletteImages[ActiveTab].end(); p++)
        (*p)->Parent = NULL;

    ActiveTab = index;

    for(list<TilesPaletteElement *>::iterator p = PaletteImages[ActiveTab].begin(); p != PaletteImages[ActiveTab].end(); p++)
        (*p)->Parent = MainForm->TilesScrollBox;


    (*(PaletteImages[index].begin()))->Click();
    (*(PaletteImages[index].begin()))->Down = true;
}
//---------------------------------------------------------------------------
void TilesPalette::SelectArrow()
{
    int index = MainForm->TilesPaletteCtrl->TabIndex;
    (*(PaletteImages[index].begin()))->Click();
    (*(PaletteImages[index].begin()))->Down = true;
}
//---------------------------------------------------------------------------
TilesPaletteElement* TilesPalette::GetActiveElement()
{
    return ActiveElement;
/*    TilesPaletteElement *p;

    for(int i = 0; i < MainForm->TilesScrollBox->ComponentCount; i++)
        if((p = dynamic_cast<TilesPaletteElement *>(MainForm->TilesScrollBox->Components[i])) != NULL && p->GroupIndex == MainForm->TilesPaletteCtrl->TabIndex + 1)
            if(p->Down)
                return p; // active element
    return NULL;  // well, should be impossible :)*/
}
//---------------------------------------------------------------------------
void TilesPalette::SetActiveElement(TilesPaletteElement* element)
{
    ActiveElement = element;
}
//---------------------------------------------------------------------------
bool TilesPalette::IsArrow(TilesPaletteElement *element)
{
    // first element is always an arrow
    return element == *(PaletteImages[MainForm->TilesPaletteCtrl->TabIndex].begin());
}
//---------------------------------------------------------------------------
#include <dir.h>
void TilesPalette::LoadFromDir(AnsiString DirName)
{
    TSearchRec dir, file;      // current dir and current file
    int TabCount = 0;          // tab ID
    Elements.clear();

    if(FindFirst(DirName + "\\*.*", faDirectory, dir) == 0) // find subdirs
    {
        do
        {
            if(dir.Name == "." || dir.Name == "..")
                    continue;                          // these are not interesting :)

            MainForm->TilesPaletteCtrl->Tabs->Add(dir.Name);
            TabCount++;
            int CurrentOffset = 0;

			// insert arrow
			char dirr[512];
			char sign[512];
			getcwd(dirr, 512);
			FILE *ff = fopen("arrow.gif", "rb");
			fread(sign, 512, 1, ff);
            fclose(ff);

			TilesPaletteElement *image = new TilesPaletteElement(TabCount, "arrow.gif", MainForm->TilesScrollBox);
			image->Down = true;
			PaletteImages.push_back(list<TilesPaletteElement *>());
			PaletteImages[TabCount - 1].push_back(image);
			image->Left = CurrentOffset;
			CurrentOffset += image->Width;
			// end insert

			if(FindFirst(DirName + "\\" + dir.Name + "\\*.gif", faArchive, file) == 0)
			{
				do
				{
					AnsiString gifname = DirName + "\\" + dir.Name + "\\" + file.Name;

					TilesPaletteElement *image = new TilesPaletteElement(TabCount, gifname, MainForm->TilesScrollBox);
					Elements[gifname] = image;
					PaletteImages[TabCount - 1].push_back(image);

					image->Left = CurrentOffset;
                    CurrentOffset += image->Width;
                }
                while(!FindNext(file));
            }
            FindClose(file);
        }
        while(!FindNext(dir));
    }
    FindClose(dir);
}
//---------------------------------------------------------------------------
/*void TilesPaletteFunctions::TileClicked(TComponent *PaletteElement)
{
    TilesPaletteElement *CurrentPE = dynamic_cast<TilesPaletteElement *>(PaletteElement);
    TilesPaletteElement *p;

    for(int i = 0; i < MainForm->TilesScrollBox->ComponentCount; i++)
        if((p = dynamic_cast<TilesPaletteElement *>(MainForm->TilesScrollBox->Components[i])) != NULL)
            p->Color = clBtnFace;

    CurrentPE->Color = clRed;
}*/
//---------------------------------------------------------------------------
TilesPaletteElement* TilesPalette::GetElementByPath(const AnsiString& path)
{
    return Elements[path];
}
//---------------------------------------------------------------------------
  


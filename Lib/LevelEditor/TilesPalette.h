//---------------------------------------------------------------------------

#ifndef TilesPaletteH
#define TilesPaletteH

#include <vcl.h>
#include "TilesPaletteElement.h"
//---------------------------------------------------------------------------

namespace TilesPalette
{
    void LoadFromDir(AnsiString DirName);
//    void TileClicked(TComponent *PaletteElement);
  //  void UnsetActiveTab(int index);
    void SetActiveTab(int index);
    TilesPaletteElement* GetActiveElement();
    void SetActiveElement(TilesPaletteElement* element);
    bool IsArrow(TilesPaletteElement *element);
    TilesPaletteElement* GetElementByPath(const AnsiString& path);
    void SelectArrow();
};

//---------------------------------------------------------------------------
#endif


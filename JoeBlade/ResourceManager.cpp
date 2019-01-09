//---------------------------------------------------------------------------
#pragma hdrstop

#include "ResourceManager.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
TResourceManager& ResourceManager()
{
    static TResResourceManager manager;

    return manager;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


#pragma hdrstop

#include "ResourceFile.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
ResourceFile::ResourceFile(char *FileName)           // load resource file
{
    strcpy(hdrname, FileName);
    strcat(hdrname, ".hdr");
    strcpy(resname, FileName);
    strcat(resname, ".res");

    FILE *hdr = fopen(hdrname, "rt");
    char buffer[256], numbuffer[256];
    ResDescriptor d;
    for(;;)
    {
        fgets(buffer, 255, hdr);                     // resource name
        buffer[strlen(buffer) - 1] = '\0';

        fgets(numbuffer, 255, hdr);
        sscanf(numbuffer, "%d %d", &d.offset, &d.size);    // offset and size

        if(feof(hdr))
            break;

        toc[buffer] = d;
    }
    fclose(hdr);

    resource = new char[1];
    GoToFirstResource();

/*    FILE *f = fopen(resname, "rb");
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    allresources = new char[len];
    fseek(f, 0, SEEK_SET);
    fread(allresources, len, 1, f);
    fclose(f);*/
}
//---------------------------------------------------------------------------
int ResourceFile::NumberOfResources()                 // get amount of resources in the file
{
    return toc.size();
}
//---------------------------------------------------------------------------
void ResourceFile::SetCurrentResource(char *ResourceName)   // load desired resource
{
    currentres =  toc.find(ResourceName);
}
//---------------------------------------------------------------------------
int ResourceFile::ResourceSize()                           // size of current resource
{
    return currentres->second.size;
}
//---------------------------------------------------------------------------
void ResourceFile::GoToFirstResource()                     // go to first resource in the file
{
    currentres = toc.begin();
}
//---------------------------------------------------------------------------
bool ResourceFile::GoToNextResource()                      // go to next resource in the file (false = failed)
{
    if(currentres == toc.end())
        return false;
    currentres++;
    return true;
}
//---------------------------------------------------------------------------
const char* ResourceFile::RetreiveResource()               // obtain a pointer to the resource
{
    delete[] resource;
    resource = new char[currentres->second.size];
    FILE *f = fopen(resname, "rb");
    fseek(f, currentres->second.offset, SEEK_SET);
    fread(resource, currentres->second.size, 1, f);
    fclose(f);
    return resource;

//    return &allresources[currentres->second.offset];
}
//---------------------------------------------------------------------------
ResourceFile::~ResourceFile()                              // dispose object
{
    delete[] resource;
//    delete[] allresources;
}
//---------------------------------------------------------------------------


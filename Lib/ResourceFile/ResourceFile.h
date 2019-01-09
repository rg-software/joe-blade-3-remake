//---------------------------------------------------------------------------

#ifndef ResourceFileH
#define ResourceFileH

#include <stdio.h>
#include <map>
#include <string>

using namespace std;

//---------------------------------------------------------------------------
struct ResDescriptor
{
    int offset;
    int size;
};
//---------------------------------------------------------------------------
class ResourceFile
{
    private:
//        char *allresources;
        
        char *resource;
        char hdrname[256];
        char resname[256];
        map<string, ResDescriptor> toc;
        map<string, ResDescriptor>::iterator currentres;

    public:
        ResourceFile(char *FileName);                // load resource file
        ~ResourceFile();                             // dispose object
        int NumberOfResources();                     // get amount of resources in the file

        void SetCurrentResource(char *ResourceName); // load desired resource
        int ResourceSize();                          // size of current resource
        void GoToFirstResource();                    // go to first resource in the file
        bool GoToNextResource();                     // go to next resource in the file (false = failed)
        const char* RetreiveResource();              // obtain a pointer to the resource
};

//---------------------------------------------------------------------------
#endif

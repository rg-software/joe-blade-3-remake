//---------------------------------------------------------------------------

#ifndef ResourceManagerH
#define ResourceManagerH

#include <vcl.h>
#include <map>
#include "ResourceFile.h"

using namespace std;

//---------------------------------------------------------------------------
class TResourceManager
{
    public:
        virtual TStream *GetResourceStream(char *resourcename) = 0;
};
//---------------------------------------------------------------------------
class TFileResourceManager : public TResourceManager
{
    public:
        TStream *GetResourceStream(char *resourcename)
        {
            TMemoryStream *stream = new TMemoryStream();
            stream->LoadFromFile(resourcename);
            return stream;
        }
};
//---------------------------------------------------------------------------
class TResResourceManager : public TResourceManager
{
    private:
        map<AnsiString, ResourceFile *> ResFiles;
    public:
        TResResourceManager()
        {
            ResFiles["Common"] = new ResourceFile("Common");
            ResFiles["Music"] = new ResourceFile("Music");
            ResFiles["Rooms"] = new ResourceFile("Rooms");
            ResFiles["Sounds"] = new ResourceFile("Sounds");
            ResFiles["Sprites"] = new ResourceFile("Sprites");
            ResFiles["Tiles"] = new ResourceFile("Tiles");
        }

        ~TResResourceManager()
        {
            delete ResFiles["Common"];
            delete ResFiles["Music"];
            delete ResFiles["Rooms"];
            delete ResFiles["Sounds"];
            delete ResFiles["Sprites"];
            delete ResFiles["Tiles"];
        }

        TStream *GetResourceStream(char *resourcename)
        {
//            FILE *f = fopen("dump_rf.txt", "at"); fprintf(f, "%s\n", resourcename); fclose(f);

            char resfile[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            int i = 0;
            while(resourcename[i] != '\\')
                resfile[i] = resourcename[i++];
            i++;

            ResourceFile *p = ResFiles[resfile];
            p->SetCurrentResource(&resourcename[i]);

            TMemoryStream *stream = new TMemoryStream();
            stream->WriteBuffer(p->RetreiveResource(), p->ResourceSize());
            stream->Position = 0;

/*            FILE *fp = fopen("temp\\tempfile.gif", "wb");
            fwrite(p->RetreiveResource(), p->ResourceSize(), 1, fp);
            fclose(fp);*/

/*
            TMemoryStream *stream = new TMemoryStream();
            stream->LoadFromFile("temp\\tempfile.gif");
  */
            return stream;
        }
};
//---------------------------------------------------------------------------
TResourceManager& ResourceManager();
//---------------------------------------------------------------------------
#endif

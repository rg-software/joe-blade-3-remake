#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include <stdio.h>
#include <list>
#include <vcl.h>
#include <string>

using namespace std;
list<AnsiString> files;
AnsiString Path;

void CreateFilesList(const AnsiString& InitDir)
{
    TSearchRec dir, file;      // current dir and current file

    if(FindFirst(InitDir + "\\*.*", faArchive, file) == 0)
    {
        do
        {
            AnsiString fullname = InitDir + "\\" + file.Name;
			AnsiString relname = fullname.SubString(Path.Length() + 2, fullname.Length() - (Path.Length() + 2) + 1);

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
    CreateFilesList(Path);

    FILE *hdrfile = fopen((ResFileName + ".hdr").c_str(), "wt");
    FILE *resfile = fopen((ResFileName + ".res").c_str(), "wb");

    for(list<AnsiString>::iterator p = files.begin(); p != files.end(); p++)
	{
		AnsiString path = Path + "\\" + *p;
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

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc != 2)
	{
		std::cout << "Usage: MakeResFile <path>\n";
		return 1;
	}

	Path = argv[1];
	AnsiString ResFileName;

	int i = Path.Length();
	while(i >= 1 && Path[i] != '\\')
		ResFileName = AnsiString(Path[i--]) + ResFileName;

	CreateResource(ResFileName);

	return 0;
}

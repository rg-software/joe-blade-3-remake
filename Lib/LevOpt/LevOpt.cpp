#include <stdio.h>
#include <map>
#include <vector>
#include <algorithm>
#include <list>
#include <set>
#include <vcl.h>
#include <string>
#include <windows.h>

#pragma hdrstop
#pragma argsused

#include <tchar.h>

int LevOptMain();

int _tmain(int argc, _TCHAR* argv[])
{
    LevOptMain();
	return 0;
}


using namespace std;

const int N_of_layers = 6;
const int RoomBoxH = 6 + 1, RoomBoxW = 18;

struct Triple { int x, y; AnsiString id; Triple(int ax, int ay, const AnsiString& aid) : x(ax), y(ay), id(aid) {} };

struct Level
{
    vector<list<Triple> > level;
    AnsiString filename;

    Level() : level(N_of_layers) {}
};


set<AnsiString> Paths;
vector<Level> levels;
map<AnsiString, int> ResPaths;

//---------------------------------------------------------------------------
void ConvertLevel(const AnsiString& filename)
{
    FILE* infile = fopen(filename.c_str(), "rb");
    char str[256];
    map<int, AnsiString> TilePaths;
    int n_of_tiles;

    fread(&n_of_tiles, sizeof(int), 1, infile);     // number of tiles

    for(int i = 0; i < n_of_tiles; i++)        // convert IDs to pointers
    {
        int k = 0;
        WORD id;

        while((str[k++] = fgetc(infile)) != 0)
            ;
        str[k] = 0;

        fread(&id, sizeof(WORD), 1, infile);
        TilePaths[id] = str;
        Paths.insert(str);
    }

    //vector<list<Triple> > level(N_of_layers);
    Level lev;
    int MaxX = 0, MaxY = 0;

    for(int layer = 0; layer < N_of_layers; layer++)
    {
        int xsize, ysize;
        double coeff;

        fread(&coeff, sizeof(double), 1, infile);   // coeff
        fread(&xsize, sizeof(int), 1, infile);
        fread(&ysize, sizeof(int), 1, infile);

        for(int i = 0; i < xsize; i++)
            for(int j = 0; j < ysize; j++)
            {
                int c = fgetc(infile);

                if(c != 0)
                {
					MaxX = (std::max)(MaxX, i);
					MaxY = (std::max)(MaxY, j);
                    lev.level[layer].push_back(Triple(i, j, TilePaths[c]));
                }
            }
    }

    fclose(infile);
    infile = fopen((filename + ".start").c_str(), "rt");
    int Xlift, Ylift;
    fscanf(infile, "%d %d", &Xlift, &Ylift);
    fclose(infile);

/*    AnsiString jb3name = filename;
    jb3name[jb3name.Length() - 2] = 'j';
    jb3name[jb3name.Length() - 1] = 'b';
    jb3name[jb3name.Length()] = '3';

    lev.filename = jb3name;*/

    for(int x = 0; x <= MaxX; x += RoomBoxW)
        for(int y = 0; y <= MaxY; y += RoomBoxH)
        {
            Level curlev;
            for(int layer = 0; layer < N_of_layers; layer++)
                for(list<Triple>::iterator p = lev.level[layer].begin(); p != lev.level[layer].end(); p++)
                    if(p->x >= x && p->x < x + RoomBoxW && p->y >= y && p->y < y + RoomBoxH)
                        curlev.level[layer].push_back(Triple(p->x % RoomBoxW, p->y % RoomBoxH, p->id));

            int idx = 1;
            AnsiString jb3name = "";
            while(filename[idx] != '.')
                jb3name += filename[idx++];
            jb3name += AnsiString(".") + AnsiString(x / RoomBoxW - Xlift);
            jb3name += AnsiString(".") + AnsiString(y / RoomBoxH - Ylift);
            jb3name += ".jb3";
            curlev.filename = jb3name;

            int objs = 0;
            for(int layer = 0; layer < N_of_layers; layer++)
                objs += curlev.level[layer].size();

            if(objs > 0)
                levels.push_back(curlev);
        }


//    levels.push_back(lev);
}
//---------------------------------------------------------------------------
void WriteLevels()
{
    FILE *outfile;
    FILE *listfile = fopen("joeblade3.rooms", "wb");

    for(unsigned i = 0; i < levels.size(); i++)
    {
        AnsiString path = "Rooms\\" + levels[i].filename;
        AnsiString name = levels[i].filename.SubString(1, levels[i].filename.Length() - 4);

        fprintf(listfile, "%s\r\n%s\r\n", path.c_str(), name.c_str());
        outfile = fopen(path.c_str(), "wb");

        for(int layer = 0; layer < N_of_layers; layer++)
        {
            int size = levels[i].level[layer].size();
            fwrite(&size, sizeof(int), 1, outfile);
            for(list<Triple>::iterator p = levels[i].level[layer].begin(); p != levels[i].level[layer].end(); p++)
            {
                fwrite(&p->x, sizeof(int), 1, outfile);
                fwrite(&p->y, sizeof(int), 1, outfile);
                fwrite(&ResPaths[p->id], sizeof(int), 1, outfile);
            }
        }

        fclose(outfile);
    }
    fclose(listfile);
}
//---------------------------------------------------------------------------
void WriteTiles()
{
    FILE *file = fopen("joeblade3.tiles", "wb");

    /*for(map<int, AnsiString>::iterator p = TilePaths.begin(); p != TilePaths.end(); p++)
        fprintf(file, "%d\r\n%s\r\n", p->first, p->second.c_str());*/

    for(map<AnsiString, int>::iterator p = ResPaths.begin(); p != ResPaths.end(); p++)
        fprintf(file, "%d\r\n%s\r\n", p->second, p->first.c_str());
    fclose(file);
}
//---------------------------------------------------------------------------
int LevOptMain()
{
    printf("Converting *.lev to Rooms\\*.jb3 + joeblade3.tiles + joeblade3.rooms\n");

    TSearchRec file;
    if(FindFirst("*.lev", faArchive, file))
        return 0;

    system("md Rooms");
	do
        ConvertLevel(file.Name);
    while(!FindNext(file));
    FindClose(file);

    int id = 1;
    for(set<AnsiString>::iterator p = Paths.begin(); p != Paths.end(); p++)
        ResPaths[*p] = id++;

    WriteLevels();
    WriteTiles();

    return 0;
}
//---------------------------------------------------------------------------

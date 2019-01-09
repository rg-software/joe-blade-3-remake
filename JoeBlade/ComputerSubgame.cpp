//---------------------------------------------------------------------------
#pragma hdrstop

#include "ComputerSubgame.h"
#include "SubgameIndicator.h"
#include <algorithm>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <math.hpp>
#include "Globals.h"
#include "Room.h"
#include "ScreenPrinter.h"
#include "Utils.h"
#include "JoeBlade.h"
#include "SoundManager.h"
#include "TilesManager.h"
#include "Config.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
void ComputerSubgame()
{
    static TComputerSubgame subgame;

    subgame.Run();
}
//---------------------------------------------------------------------------
void TComputerSubgame::LeftOperation()
{
    vector<int> hh;
    for(int i = 0; i <= 7; i++)
        if(highlights[i])
            hh.push_back(i);

    int temp = numbers[hh[0]];
    numbers[hh[0]] = numbers[hh[1]];
    numbers[hh[1]] = numbers[hh[2]];
    numbers[hh[2]] = numbers[hh[3]];
    numbers[hh[3]] = temp;
}
//---------------------------------------------------------------------------
void TComputerSubgame::RightOperation()
{
    vector<int> hh;
    for(int i = 0; i <= 7; i++)
        if(highlights[i])
            hh.push_back(i);

    int temp = numbers[hh[0]];
    numbers[hh[0]] = numbers[hh[3]];
    numbers[hh[3]] = numbers[hh[2]];
    numbers[hh[2]] = numbers[hh[1]];
    numbers[hh[1]] = temp;
}
//---------------------------------------------------------------------------
void TComputerSubgame::FireOperation()
{
}
//---------------------------------------------------------------------------
void TComputerSubgame::SubgameTimerFun()
{
    SoundManager().PlaySound(Sounds::TIMERTICK);

    int oldidx;
    for(oldidx = 0; !highlights[oldidx]; oldidx++)
        ;

    int newidx;
    while((newidx = RandomRange(0, 3)) == oldidx)
        ;

    fill(highlights.begin(), highlights.end(), false);
    switch(newidx)
    {
        case 0: highlights[0] = highlights[1] = highlights[6] = highlights[7] = true; break;
        case 1: highlights[1] = highlights[2] = highlights[5] = highlights[6] = true; break;
        case 2: highlights[2] = highlights[3] = highlights[4] = highlights[5] = true; break;
    } 
}
//---------------------------------------------------------------------------
void TComputerSubgame::ShowObjectsMenu()
{
    map<int, set<int> > Data;  // <object, set_of_floors>

    for(map<AnsiString, Room *>::iterator room = Rooms.begin(); room != Rooms.end(); room++)     // пробегаем по всем комнатам игры
    {
        AnsiString FloorNoStr = "";
        int idx = 1;
        while(room->first[idx] != '.')
            FloorNoStr += room->first[idx++];

        int FloorNo = atoi(FloorNoStr.c_str());                     // определяем этаж текущей комнаты

        // пробегаем по всем объектам комнаты
        for(list<Triple>::iterator p = room->second->Data[Config::ObjectsLayer].begin(); p != room->second->Data[Config::ObjectsLayer].end(); p++)
            Data[p->id].insert(FloorNo);
    }

    const int CRAX = -1;

    Data[CRAX].insert(CraxFloor);

    int objids[] = { TTiles::DISK, TTiles::HOSTAGE, TTiles::DETONATOR, TTiles::DYNAMITE, TTiles::AMMUNITION, TTiles::LIFTCARD, TTiles::FOOD, CRAX };
    map<int, AnsiString> msgs;
    msgs[TTiles::DISK] = "a computer disk";
    msgs[TTiles::HOSTAGE] = "a hostage";
    msgs[TTiles::DETONATOR] = "a detonator";
    msgs[TTiles::DYNAMITE] = "an explosive";
    msgs[TTiles::AMMUNITION] = "some ammunition";
    msgs[TTiles::LIFTCARD] = "a lift pass";
    msgs[TTiles::FOOD] = "some food";
    msgs[CRAX] = "crax bloodfinger";

    vector<pair<int, set<int> > > FinalList; // vector<(object_id, set_of_floors)>
    for(int i = 0; i < sizeof(objids) / sizeof(objids[0]); i++)
    {
        int id = objids[i];
        if(Data[id].size() > 0)
            FinalList.push_back(pair<int, set<int> >(id, Data[id]));
    }

    random_shuffle(FinalList.begin(), FinalList.end());

    vector<AnsiString> menu;
    menu.push_back("");
    menu.push_back("well played, mr. blade!");
    menu.push_back("");
    menu.push_back("select an item");
    menu.push_back("");
    for(unsigned i = 0; i < min(4, FinalList.size()); i++)
        menu.push_back(msgs[FinalList[i].first]);

    int result;
    while((result = ShowMenu(menu, 5, menu.size() - 1)) == -1)
        ;
    result -= 5;
    int objid = FinalList[result].first;
    int idx = RandomRange(0, FinalList[result].second.size());
    int curidx = -1;
    int floor;
    set<int>::iterator p = FinalList[result].second.begin();

    while(idx != curidx)
    {
        curidx++;
        floor = *p;
        p++;
    }

    const char *floorName = (floor == Joe->CurrentRoom.Z) ? "this floor" : GetFloorName(floor);
    AnsiString msg;
    MyMessageBox((msg.sprintf("-1 2>you can find|-1 3>%s|-1 4>on|-1 5>%s|-1 7>press space to continue|\0", msgs[objid], floorName)).c_str());
}
//---------------------------------------------------------------------------
void TComputerSubgame::Run()
{
    highlights[0] = highlights[1] = highlights[6] = highlights[7] = true;
    PrepareNumbers();

    highlights[0] = highlights[1] = highlights[6] = highlights[7] = true;

    SoundManager().PlaySound(Sounds::COMPUTER_SUBGAME);
    MyMessageBox("-1 3>sixty seconds to hack into|-1 4>computer system|-1 6>press space to continue|\0");
    GameStatus = GAME;

    if(RunKernel())
    {
        SoundManager().PlaySound(Sounds::COMPUTER_SUBGAME_VICTORY);
        ShowObjectsMenu();
    }
    else
    {
        SoundManager().PlaySound(Sounds::COMPUTER_SUBGAME_DEFEAT);
        MyMessageBox("-1 4>too late!|-1 6>press space to continue|\0");
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma hdrstop

#include "Utils.h"
#include "Config.h"
#include "Globals.h"
#include "JoeBlade.h"
#include "Bullet.h"
#include "Smoke.h"
#include "Room.h"
#include <stdio.h>
#include <map>
#include <Math.hpp>
#include <algorithm>
#include <memory>
#include "ScreenPrinter.h"
#include "TilesManager.h"
#include "ResourceManager.h"
#include "MessagePanel.h"
#include "SoundManager.h"
#include "EnemiesManager.h"
#include "JukeBox.h"
#include "Background.h"
#include "StatusPanel.h"

#include <windows.h>

#pragma package(smart_init)

using namespace std;
//---------------------------------------------------------------------------
void RenewObjectives()
{
    curobjs[0] = objectives[0];
    curobjs[1] = objectives[1];
    curobjs[2] = objectives[2];
    curobjs[3] = objectives[3];
}

void ShowNextObjective(Objective obj)
{
    curobjs[obj] = NULL;
    int i = 0;
    while(curobjs[i] == NULL)
        i++;
    MessagePanel().EnqueueMessage(curobjs[i]);
}

bool ObjectivesPending()
{
    return curobjs[0] != NULL || curobjs[1] != NULL || curobjs[2] != NULL;
}
//---------------------------------------------------------------------------
void ClearRoomBox()
{
    SDL_Rect r;
    r.x = Config::Xoffset + Config::GridSize; r.y = Config::Yoffset;
    r.w = Config::RoomWidth - Config::GridSize; r.h = Config::RoomHeight;

    SDL_FillRect(SDL_GetVideoSurface(), &r, 0);
}
//---------------------------------------------------------------------------
bool DefaultExitCondition()
{
    static bool SpaceWasPressed = true;

    SDL_Event event;
    SDL_PollEvent(&event);
    Uint8 *keys = SDL_GetKeyState(NULL);
    if(!SpaceWasPressed && keys[SDLK_SPACE] == SDL_PRESSED)
    {
        SpaceWasPressed = true;
        return true;
    }
    SpaceWasPressed = (keys[SDLK_SPACE] == SDL_PRESSED);
    return false;
}

void MyMessageBox(char *messages, void (*fun)(), bool (*exitcond)())
{
    for(;;)
    {
        ClearRoomBox();
        char coords[10], buffer[50];
        char *p = messages;
        int x, y;
        int len;

        if(strlen(messages) != 0)
            do                                  // "1 5>hello|2 4>bye|"
            {
                memset(coords, 0, 10);
                memset(buffer, 0, 50);

                len = 0;
                while(p[len] != '>')
                    len++;
                strncpy(coords, p, len);
                sscanf(coords, "%d %d", &x, &y);
                if(x != -1)
                    x = Config::Xoffset + ScreenPrinter().CharW() * x;
                y = Config::Yoffset + ScreenPrinter().CharH() * y;

                p += len + 1;
                len = 0;
                while(p[len] != '|')
                    len++;
                strncpy(buffer, p, len);
                p += len + 1;

                if(x == -1)
                    ScreenPrinter().PrintCAligned(y, buffer);
                else
                    ScreenPrinter().Print(x, y, buffer);
            }
            while(*p != 0);

        if(fun != NULL)
            (*fun)();

        if((*exitcond)() == true)
            break;

        MessagePanel().Show();
        JukeBox().Show();
        SDL_Flip(SDL_GetVideoSurface());
    }

    SoundManager().PlaySound(Sounds::MENU_CLICK);
}
//---------------------------------------------------------------------------
const char* GetFloorName(int floorNo)
{
    return Config::FloorNames[floorNo - Config::LowerFloor + 2];
}
//---------------------------------------------------------------------------
const char* GetObjectMessage(int object)
{
    static map<int, AnsiString> objmessages;
    static bool firstrun = true;

    if(firstrun)
    {
        firstrun = false;

        //int DYNAMITE, MINE, 

        objmessages[TTiles::KEY] = "yes! a key!";
        objmessages[TTiles::SECURITYCARD] = "a security pass!";
        objmessages[TTiles::MASK] = "joe in disguise!";
        objmessages[TTiles::AMMUNITION] = "some ammunition!";
        objmessages[TTiles::DISK] = "a computer disk!";
        objmessages[TTiles::GRENADE] = "a grenade!";
        objmessages[TTiles::DETONATOR] = "a detonator";
        objmessages[TTiles::FOOD] = "fooood!!";
        objmessages[TTiles::HOSTAGE] = "you have found a hostage";
        objmessages[TTiles::ARMOUR] = "a bullet-proof vest";
        objmessages[TTiles::LIFTCARD] = "a lift pass";
    }

    return objmessages[object].c_str();
}
//---------------------------------------------------------------------------
SDL_Surface *LoadSurfaceFromGIFResource(char *gifname)
{
    auto_ptr<TStream> stream(ResourceManager().GetResourceStream(gifname));
    MyGIFStructure gif = MyGIFStructure(stream.get());

    SDL_Surface *result = SDL_ConvertSurface(gif.Frames[0], gif.Frames[0]->format, gif.Frames[0]->flags);
    SDL_SetColorKey(result, SDL_SRCCOLORKEY, SDL_MapRGB(result->format, Config::ColorKey[0], Config::ColorKey[1], Config::ColorKey[2]));

    return result;

/*    MyGIFStructure gif = MyGIFStructure(gifname);

    SDL_Surface *result = SDL_ConvertSurface(gif.Frames[0], gif.Frames[0]->format, gif.Frames[0]->flags);
    SDL_SetColorKey(result, SDL_SRCCOLORKEY, SDL_MapRGB(result->format, Config::ColorKey[0], Config::ColorKey[1], Config::ColorKey[2]));
    */
//    SDL_Surface *result = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);

//    SDL_ConvertSurface(gif.Frames[0], gif.Frames[0]->format, gif.Frames[0]->flags);

//    return result;
}
//---------------------------------------------------------------------------
/*list<void (*)()> destructors;

void AddSDLDestructor(void (*fun)())
{
    destructors.push_back(fun);
}
//---------------------------------------------------------------------------
void CallSDLDestructors()
{
    for(list<void (*)()>::iterator p = destructors.begin(); p != destructors.end(); p++)
        (*p)();
} */
//---------------------------------------------------------------------------
void ClearRooms()
{
    for(map<AnsiString, Room *>::iterator p = Rooms.begin(); p != Rooms.end(); p++)
        delete p->second;
    Rooms.clear();
}
//---------------------------------------------------------------------------
void LoadRooms()                                    // загрузить все комнаты
{
    ClearRooms();
    FILE *f = fopen("joeblade3.rooms", "rt");       // список имён ресурсов комнат и самих комнат

    char path[255], name[255];

    for(;;)
    {
        fgets(path, 255, f);                        // имя ресурса
        fgets(name, 255, f);                        // имя комнаты (вида Z.X.Y)
        if(feof(f))
            break;

        path[strlen(path) - 1] = 0;                 // обрезаем \n
        name[strlen(name) - 1] = 0;                 // обрезаем \n

        Room *r = new Room(path);                   // загружаем комнату
        Rooms[name] = r;                            // и кладём её в массив комнат
    }

    fclose(f);
}
//---------------------------------------------------------------------------
void ShuffleObjects()
{
    map<int, int> CObjects;                 // количество объектов разных типов <id, количество> в игре
    map<int, int> ObjectsOnFloor;           // количество объектов (любых) на этаже (<этаж, количество>)
    map<int, list<AnsiString> > RoomNames;  // список названий комнат данного этажа (<этаж, список имён>)
    int liftcards = 0;                      // количество лифт-карточек в игре

    Config::DynamitesTotal = 0;
    for(map<AnsiString, Room *>::iterator room = Rooms.begin(); room != Rooms.end(); room++)     // пробегаем по всем комнатам игры
    {
        AnsiString FloorNoStr = "";
        int idx = 1;
        while(room->first[idx] != '.')
            FloorNoStr += room->first[idx++];

        int FloorNo = atoi(FloorNoStr.c_str());                     // определяем этаж текущей комнаты

//        char *s = room->first.c_str();
        RoomNames[FloorNo].push_back(room->first);                  // и вносим имя комнаты в список комнат данного этажа

        // пробегаем по всем объектам комнаты
        for(list<Triple>::iterator p = room->second->Data[Config::ObjectsLayer].begin(); p != room->second->Data[Config::ObjectsLayer].end(); p++)
        {
            if(p->id == TTiles::LIFTCARD)                            // если объект - лифт-карточка
                liftcards++;                                         // просто запоминаем количество найденных карт
            else
                CObjects[p->id]++;                                   // иначе вносим найденный объект в список объектов

            ObjectsOnFloor[FloorNo]++;                               // обновляем значение количества объектов данного этажа
        }
    }

    Config::DynamitesTotal = CObjects[TTiles::DYNAMITE];
    Config::HostagesTotal = CObjects[TTiles::HOSTAGE];

/*    FILE *f = fopen("dump_objs.txt", "wt");
    fprintf(f, "keys = %d\n", CObjects[TTiles::KEY]);
    fprintf(f, "hostages = %d\n", CObjects[TTiles::HOSTAGE]);
    fprintf(f, "detonators = %d\n", CObjects[TTiles::DETONATOR]);
    fprintf(f, "dynamites = %d\n", CObjects[TTiles::DYNAMITE]);
    fprintf(f, "security cards = %d\n", CObjects[TTiles::SECURITYCARD]);
    fprintf(f, "lift cards = %d\n", CObjects[TTiles::LIFTCARD]);
    fclose(f);*/

    vector<int> ShuffledIDs;
    for(map<int, int>::iterator p = CObjects.begin(); p != CObjects.end(); p++)   // составляем список идентификаторов всех найденных объектов
        for(int i = 0; i < p->second; i++)                                        // (например: 1, 2, 1, 1, 5, 12, 5, ...)
            ShuffledIDs.push_back(p->first);

/*    FILE *f = fopen("sdump.txt", "wt");
    fprintf(f, "key=%d mine=%d dynamite=%d mask=%d ammo=%d food=%d\n", TTiles::KEY, TTiles::MINE, TTiles::DYNAMITE, TTiles::MASK, TTiles::AMMUNITION, TTiles::FOOD);
    for(unsigned i = 0; i < ShuffledIDs.size(); i++)
        fprintf(f, "%d ", ShuffledIDs[i]);
    fprintf(f, "\n");*/

    random_shuffle(ShuffledIDs.begin(), ShuffledIDs.end());                       // и перемешиваем его в случайном порядке

/*    for(unsigned i = 0; i < ShuffledIDs.size(); i++)
        fprintf(f, "%d ", ShuffledIDs[i]);
    fclose(f);*/


    int index = 0;                                                     // индекс в массиве перемешанных идентификаторов
    int iteration = 0;                                                 // текущая итерация цикла установки объектов
    vector<list<Triple>::iterator> reserved(liftcards);                // массив liftcards итераторов на зарезервированные места

    // цикл по этажам (т.к. map - сортированная коллекция, цикл идёт от нижних этажей к верхним - это важно)
    for(map<int, list<AnsiString> >::iterator roomslist = RoomNames.begin(); roomslist != RoomNames.end(); roomslist++)
    {
        // работаем с этажом roomslist->first
        // цикл по комнатам этажа
        for(list<AnsiString>::iterator roomname = roomslist->second.begin(); roomname != roomslist->second.end(); roomname++)
        {
            Room *room = Rooms[*roomname];              // текущая комната
            // цикл по объектам комнаты
            for(list<Triple>::iterator p = room->Data[Config::ObjectsLayer].begin(); p != room->Data[Config::ObjectsLayer].end(); p++)
            {
                if(iteration < liftcards)           // первые liftcards мест ничем не заполняются
                    reserved[iteration] = p;        // они лишь вносятся в список зарезервированных
                else
                    p->id = ShuffledIDs[index++];   // в противном случае кладём на это место текущий объект из массива ShuffledIDs
                iteration++;
            }
        }
    }

    // первые liftcards мест на самом нижнем этаже теперь зарезервированы.
    // осталось выбрать liftcards случайных позиций на других этажах и обменять лежащие там предметы
    // на лифтовые карточки; предметы при этом переносятся на нижний этаж в зарезервированные места
    for(int card = 0; card < liftcards; card++)
    {
        // выбираем случайный этаж из заведомо достижимых на данный момент
        int floor = 1 + Config::LowerFloor + RandomRange(0, Config::LiftCardFloors[card] - Config::LowerFloor + 1);
        int objNo = RandomRange(0, ObjectsOnFloor[floor]);           // выбираем случайный порядковый номер объекта на данном этаже
        int curObjNo = 0;
        // проходим по всем объектам данного этажа:
        // сначала по комнатам этажа
        for(list<AnsiString>::iterator roomname = RoomNames[floor].begin(); roomname != RoomNames[floor].end(); roomname++)
        {
            Room *room = Rooms[*roomname];
            // а теперь - по объектам комнаты
            for(list<Triple>::iterator p = room->Data[Config::ObjectsLayer].begin(); p != room->Data[Config::ObjectsLayer].end(); p++)
            {
                if(curObjNo == objNo)                 // если номер текущего объекта совпадает с objNo
                {
                    reserved[card]->id = p->id;       // кладём текущий объект на зарезервированное место
                    p->id = TTiles::LIFTCARD;         // а на его старое место кладём лифт-карточку
                    goto skip;                        // переходим к следующей карточке
                }
                curObjNo++;
            }
        }
        skip: ;
    }

    for(map<AnsiString, Room *>::iterator room = Rooms.begin(); room != Rooms.end(); room++)
    {
        bool found = true;

        while(found)
        {
            found = false;
            for(list<Triple>::iterator p = room->second->Data[Config::ObjectsLayer].begin(); p != room->second->Data[Config::ObjectsLayer].end(); p++)
                if(p->id == TTiles::NOTHING)
                {
                    found = true;
                    room->second->Data[Config::ObjectsLayer].erase(p);
                    break;
                }
        }
    }
}
//---------------------------------------------------------------------------
void InitializeNewGame()
{
    LoadRooms();

    GameTimerTicks = 0;
    RenewObjectives();
    DynamiteTimerEnabled = false;
    Score = 0;
    CraxCaptured = false;
    CraxFloor = RandomRange(Config::CraxFloorL, Config::CraxFloorH + 1);
    DynamitesRigged = 0;
    HostagesFound = 0;
    Joe->Init();

    ShuffleObjects();
    Inventory.clear();
    if(Config::InfKeysDisks)
    {
        Inventory[TTiles::KEY] = 50;
        Inventory[TTiles::DISK] = 50;
    }

    EnemiesManager().ReInit();
/*    for(vector<SimpleEnemy *>::iterator p = enemies.begin(); p != enemies.end(); p++)
        *p = new SimpleEnemy("Sprites\\officer-green.gif", GameCharacter::LEFT, GameCharacter::PASSIVE);*/
}
//---------------------------------------------------------------------------
int ShowMenu(const vector<AnsiString>& items, int min_idx, int max_idx)
{
    int cur_idx = min_idx;
    bool SpaceWasPressed = true;

    SDL_Delay(Config::MenuDelay);
    for(;;)
    {
        SDL_Event event;
        SDL_PollEvent(&event);
        Uint8 *keys = SDL_GetKeyState(NULL);
/*        if(event.type == SDL_KEYDOWN)
            event = event;*/
        if(keys[SDLK_DOWN] == SDL_PRESSED && cur_idx < max_idx)
        {
            cur_idx++;
            SoundManager().PlaySound(Sounds::MENU_CLICK);
            SDL_Delay(Config::MenuDelay);
        }
        else if(keys[SDLK_UP] == SDL_PRESSED && cur_idx > min_idx)
        {
            cur_idx--;
            SoundManager().PlaySound(Sounds::MENU_CLICK);
            SDL_Delay(Config::MenuDelay);
        }
        else if(keys[SDLK_SPACE] == SDL_PRESSED && !SpaceWasPressed)
        {
            SoundManager().PlaySound(Sounds::MENU_CLICK);
            SDL_Delay(Config::MenuDelay);
            return cur_idx;
        }
        else if(keys[SDLK_ESCAPE] == SDL_PRESSED)
        {
            SoundManager().PlaySound(Sounds::MENU_CLICK);
            SDL_Delay(Config::MenuDelay);
            return -1;
        }

        SpaceWasPressed = (keys[SDLK_SPACE] == SDL_PRESSED);
        ClearRoomBox();

        for(unsigned i = 0; i < items.size(); i++)
        {
            int MsgY = Config::Yoffset + i * ScreenPrinter().CharH();

            ScreenPrinter().PrintCAligned(MsgY, items[i].c_str());
            if(i == (unsigned)cur_idx)
            {
                ScreenPrinter().Print(Config::Xoffset + Config::GridSize, MsgY, ">");
                ScreenPrinter().Print(Config::Xoffset + Config::RoomWidth - ScreenPrinter().CharW(), MsgY, "<");
            }
        }

        MessagePanel().Show();
        JukeBox().Show();
        SDL_Flip(SDL_GetVideoSurface());
    }
}
//---------------------------------------------------------------------------
int SelectSlot()
{
    vector<AnsiString> slots;

    slots.push_back("select slot");
    slots.push_back("");

    for(int i = 1; i <= Config::N_of_slots; i++)
    {
        char filename[] = { 's', 'l', 'o', 't', i + '0', '.', 's', 'a', 'v', 0 };

        FILE *fp = fopen(filename, "rb");
        unsigned short d, m, y;
        int f;
        fread(&d, sizeof(unsigned short), 1, fp);
        fread(&m, sizeof(unsigned short), 1, fp);
        fread(&y, sizeof(unsigned short), 1, fp);
        fread(&f, sizeof(int), 1, fp);

        if(feof(fp))
            slots.push_back("empty");
        else
        {
            char buffer[50];
            sprintf(buffer, "%d.%d.%d, %s", d, m, y, GetFloorName(f));
            slots.push_back(buffer);
        }

        fclose(fp);
    }

    int result = ShowMenu(slots, 2, slots.size() - 1);
    if(result != -1)
        result -= 1;

    return result;
}
//---------------------------------------------------------------------------
void SaveGame()
{
    int slot = SelectSlot();
    if(slot == -1)
        return;

    char filename[] = { 's', 'l', 'o', 't', slot + '0', '.', 's', 'a', 'v', 0 };

    FILE *fp = fopen(filename, "wb");
    unsigned short d, m, y;

    TDateTime::CurrentDate().DecodeDate(&y, &m, &d);
    fwrite(&d, sizeof(unsigned short), 1, fp);
    fwrite(&m, sizeof(unsigned short), 1, fp);
    fwrite(&y, sizeof(unsigned short), 1, fp);

    fwrite(&Joe->CurrentRoom.Z, sizeof(int), 1, fp);

    fwrite(&Score, sizeof(int), 1, fp);
    fwrite(&DynamiteTimer, sizeof(int), 1, fp);
    fwrite(&DynamiteTimerEnabled, sizeof(bool), 1, fp);
    fwrite(&GameTimerTicks, sizeof(int), 1, fp);

    fwrite(&CraxCaptured, sizeof(bool), 1, fp);
    fwrite(&CraxFloor, sizeof(int), 1, fp);
    fwrite(&DynamitesRigged, sizeof(int), 1, fp);
    fwrite(&HostagesFound, sizeof(int), 1, fp);

    int isize = (int)Inventory.size();
    fwrite(&isize, sizeof(int), 1, fp);
    for(map<int, int>::iterator p = Inventory.begin(); p != Inventory.end(); p++)
    {
        fwrite(&p->first, sizeof(int), 1, fp);
        fwrite(&p->second, sizeof(int), 1, fp);
    }

    int rsize = (int)Rooms.size();
    fwrite(&rsize, sizeof(int), 1, fp);
    for(map<AnsiString, Room *>::iterator room = Rooms.begin(); room != Rooms.end(); room++)
    {
        int len = room->first.Length();
        fwrite(&len, sizeof(int), 1, fp);
        fwrite(room->first.c_str(), len, 1, fp);

        room->second->SaveLayer(Config::ObjectsLayer, fp);
        room->second->SaveLayer(Config::SystemLayer, fp);
    }

    for(int i = 0; i <= 3; i++)
    {
        bool completed = (curobjs[i] == NULL);
        fwrite(&completed, sizeof(bool), 1, fp);
    }

    bullet->SaveState(fp);
    enemybullet->SaveState(fp);
    Joe->SaveState(fp);
    
    for(vector<Smoke *>::iterator p = SmokeObjects.begin(); p != SmokeObjects.end(); p++)
        (*p)->SaveState(fp);

    int mlen = strlen(MessagePanel().GetTopMessage());
    fwrite(&mlen, sizeof(int), 1, fp);
    fwrite(MessagePanel().GetTopMessage(), mlen, 1, fp);

    EnemiesManager().SaveState(fp);

    fclose(fp);
}
//---------------------------------------------------------------------------
void LoadGame()
{
    int slot = SelectSlot();
    if(slot == -1)
        return;

    char filename[] = { 's', 'l', 'o', 't', slot + '0', '.', 's', 'a', 'v', 0 };

    FILE *fp = fopen(filename, "rb");
    unsigned short d, m, y;

    fread(&d, sizeof(unsigned short), 1, fp);
    fread(&m, sizeof(unsigned short), 1, fp);
    fread(&y, sizeof(unsigned short), 1, fp);

    if(feof(fp))
        return;

    InitializeNewGame();
    GameStatus = GAME;

    fread(&Joe->CurrentRoom.Z, sizeof(int), 1, fp);

    fread(&Score, sizeof(int), 1, fp);
    fread(&DynamiteTimer, sizeof(int), 1, fp);
    fread(&DynamiteTimerEnabled, sizeof(bool), 1, fp);
    fread(&GameTimerTicks, sizeof(int), 1, fp);

    fread(&CraxCaptured, sizeof(bool), 1, fp);
    fread(&CraxFloor, sizeof(int), 1, fp);
    fread(&DynamitesRigged, sizeof(int), 1, fp);
    fread(&HostagesFound, sizeof(int), 1, fp);

    int isize;
    fread(&isize, sizeof(int), 1, fp);
    Inventory.clear();
    for(int i = 0; i < isize; i++)
    {
        int key, value;
        fread(&key, sizeof(int), 1, fp);
        fread(&value, sizeof(int), 1, fp);

        Inventory[key] = value;
    }

    int rsize;
    fread(&rsize, sizeof(int), 1, fp);
    for(int i = 0; i < rsize; i++)
    {
        char roomname[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
        int len;

        fread(&len, sizeof(int), 1, fp);
        fread(roomname, len, 1, fp);

        Rooms[roomname]->LoadLayer(Config::ObjectsLayer, fp);
        Rooms[roomname]->LoadLayer(Config::SystemLayer, fp);
    }

    for(int i = 0; i <= 3; i++)
    {
        bool completed;
        fread(&completed, sizeof(bool), 1, fp);

        curobjs[i] = (completed ? NULL : objectives[i]);
    }

    bullet->LoadState(fp);
    enemybullet->LoadState(fp);
    Joe->LoadState(fp);

    for(vector<Smoke *>::iterator p = SmokeObjects.begin(); p != SmokeObjects.end(); p++)
        (*p)->LoadState(fp);

    int mlen;
    char message[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    fread(&mlen, sizeof(int), 1, fp);
    fread(message, mlen, 1, fp);
    MessagePanel().EnqueueMessage(message);

    EnemiesManager().LoadState(fp);

    fclose(fp);
}
//---------------------------------------------------------------------------
void ShowSplash()
{
    SDL_Surface *splash = LoadSurfaceFromGIFResource("Common\\splash640x480.gif");
    SDL_SetColorKey(splash, 0, 0);

    int j = Config::ScreenParams[1] / 2;
    for(int i = Config::ScreenParams[0] / 2; i >= 0; i -= 4)
    {
        SDL_Rect r;
        r.x = i;
        r.y = j;
        r.w = 2 * (Config::ScreenParams[0] / 2 - i);
        r.h = 2 * (Config::ScreenParams[1] / 2 - j);
        j -= 3;

        My_SDL_BlitSurface(splash, &r, SDL_GetVideoSurface(), &r);
        SDL_Flip(SDL_GetVideoSurface());
        SDL_Delay(10);

        SDL_Event event;
        SDL_PollEvent(&event);
    }
    SDL_FreeSurface(splash);
}
//---------------------------------------------------------------------------
void RemoveSplash()
{
    SDL_Surface *splash = LoadSurfaceFromGIFResource("Common\\splash640x480.gif");
    SDL_SetColorKey(splash, 0, 0);
    SDL_Rect r1;

    r1.x = Config::Xoffset + Config::GridSize; r1.y = Config::Yoffset;
    r1.w = Config::RoomWidth - Config::GridSize; r1.h = Config::RoomHeight;

    int j = 0;
    for(int i = 0; i <= Config::ScreenParams[0] / 2; i += 4)
    {
        SDL_Rect r;
        r.x = i;
        r.y = j;
        r.w = 2 * (Config::ScreenParams[0] / 2 - i);
        r.h = 2 * (Config::ScreenParams[1] / 2 - j);
        j += 3;

        Background().Show();
        StatusPanel().Show();
        SDL_FillRect(SDL_GetVideoSurface(), &r1, 0);
        My_SDL_BlitSurface(splash, &r, SDL_GetVideoSurface(), &r);
        SDL_Flip(SDL_GetVideoSurface());
        SDL_Delay(10);

        SDL_Event event;
        SDL_PollEvent(&event);
    }

    SDL_FreeSurface(splash);
}
//---------------------------------------------------------------------------
void ShowOriginalSplash()
{
    SDL_Surface *splash = LoadSurfaceFromGIFResource("Common\\original-splash.gif");
    SDL_Rect r;
    r.x = Config::Xoffset + Config::GridSize;
    r.y = Config::Yoffset;
    My_SDL_BlitSurface(splash, NULL, SDL_GetVideoSurface(), &r);
    SDL_FreeSurface(splash);
}

void ShowOriginalScreen()
{
    SDL_Surface *screen = LoadSurfaceFromGIFResource("Common\\original-screen.gif");
    SDL_Rect r;
    r.x = Config::Xoffset + (Config::GridSize + Config::RoomWidth) / 2;
    r.y = Config::Yoffset;
    My_SDL_BlitSurface(screen, NULL, SDL_GetVideoSurface(), &r);
    SDL_FreeSurface(screen);
}

void ShowObjectives()
{
    SDL_Surface *crax =  LoadSurfaceFromGIFResource("Sprites\\crax.gif");
    SDL_Rect r;
    r.x = Config::Xoffset + Config::GridSize;
    r.y = Config::Yoffset + ScreenPrinter().CharW();
    My_SDL_BlitSurface(TilesManager().GetTileByID(TTiles::HOSTAGE), NULL, SDL_GetVideoSurface(), &r);
    r.x = Config::Xoffset + 27 * ScreenPrinter().CharH();
    r.y = Config::Yoffset + 3 * ScreenPrinter().CharW();
    My_SDL_BlitSurface(crax, NULL, SDL_GetVideoSurface(), &r);
    SDL_FreeSurface(crax);
    r.x = Config::Xoffset + Config::GridSize;
    r.y = Config::Yoffset + 5 * ScreenPrinter().CharW();
    My_SDL_BlitSurface(TilesManager().GetTileByID(TTiles::DYNAMITE), NULL, SDL_GetVideoSurface(), &r);
}

void ShowJB3Splash()
{
    SDL_Surface *splash = LoadSurfaceFromGIFResource("Common\\joeblade3-splash.gif");
    SDL_Rect r;
    r.x = Config::Xoffset + Config::GridSize;
    r.y = Config::Yoffset;
    My_SDL_BlitSurface(splash, NULL, SDL_GetVideoSurface(), &r);
    SDL_FreeSurface(splash);
}

void ShowRRSplash()
{
    SDL_Surface *splash = LoadSurfaceFromGIFResource("Common\\remakes.gif");
    SDL_Rect r;
    r.x = Config::Xoffset + Config::GridSize;
    r.y = Config::Yoffset;
    My_SDL_BlitSurface(splash, NULL, SDL_GetVideoSurface(), &r);
    SDL_FreeSurface(splash);
}

void ShowPhoto()
{
    SDL_Surface *splash = LoadSurfaceFromGIFResource("Common\\a&m.gif");
    SDL_SetColorKey(splash, 0, 0);
    SDL_Rect r;
    r.x = Config::Xoffset + Config::GridSize;
    r.y = Config::Yoffset;
    My_SDL_BlitSurface(splash, NULL, SDL_GetVideoSurface(), &r);
    SDL_FreeSurface(splash);
}

void ShowInfo()
{
    MyMessageBox("21 0>joe blade D|23 1>remake|25 3>by|19 4>maxim mozgovoy|24 5>and|19 6>anton mozgovoy|22 9>press space|", ShowOriginalSplash);
    MyMessageBox("4 0>original game|9 1>by|6 3>tom prosser|3 4>sonic graffiti|4 5>andrew severn|6 9>press space|", ShowOriginalScreen);
    MyMessageBox("-1 0>your mission|2 2>|6 3>rescue all hostages|6 4>capture crax bloodfinger|6 5>rig all explosives|6 6>escape via the roof|22 9>press space|", ShowObjectives);
    MyMessageBox("16 0>game controls|11 2>movement: cursor keys|11 3>fire: space|11 4>jukebox on, off: ctrl|11 5>change track: shift|22 9>press space|", ShowJB3Splash);
    MyMessageBox("-1 5>joe blade D remake|-1 6>is created specially for|-1 7>retro remakes competition CAAE|-1 9>press space|", ShowRRSplash);
    MyMessageBox("21 2>happy gaming!|25 4>maxim|26 5>and|25 6>anton|22 9>press space|", ShowPhoto);
}
//---------------------------------------------------------------------------
void LoadHiTable()
{
    FILE *f = fopen("joeblade3.hitbl", "rb");

    HiTable.clear();
    for(int k = 0; k < Config::HiTableSize; k++)
    {
        AnsiString name;
        int len = fgetc(f);
        for(int i = 0; i < len; i++)
            name += char(~fgetc(f));
        int scorelen = fgetc(f);
        AnsiString score;
        for(int i = 0; i < scorelen; i++)
            score += char(~fgetc(f));

        HiTable.push_back(HiEntry(name.c_str(), atoi(score.c_str())));
    }
    fclose(f);
}
//---------------------------------------------------------------------------
void SaveHiTable()
{
    FILE *f = fopen("joeblade3.hitbl", "wb");

    for(int k = 0; k < Config::HiTableSize; k++)
    {
        fputc(HiTable[k].name.Length(), f);
        for(int i = 1; i <= HiTable[k].name.Length(); i++)
            fputc(~HiTable[k].name[i], f);
        AnsiString s;
        s.sprintf("%d", HiTable[k].score);
        fputc(s.Length(), f);
        for(int i = 1; i <= s.Length(); i++)
            fputc(~s[i], f);
    }
    fclose(f);
}
//---------------------------------------------------------------------------
void ShowHiTable()
{
    AnsiString message = "-1 0>hiscores|";
    int ybegin = 2;

    for(int i = 0; i < Config::HiTableSize; i++)
    {
        message += "3 " + AnsiString(ybegin + i) + ">";
        message += HiTable[i].name + "|";
        AnsiString score;
        score.sprintf("%d", HiTable[i].score);

        message += AnsiString(33 - score.Length()) + " " + AnsiString(ybegin + i) + ">";
        message += score + "|";
    }

    MyMessageBox(message.c_str());
}
//---------------------------------------------------------------------------
int My_SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
    static CRITICAL_SECTION CriticalSection;
    static bool first = true;
    if(first)
    {
        InitializeCriticalSection(&CriticalSection);
        first = false;
    }

    EnterCriticalSection(&CriticalSection);
    int result = SDL_BlitSurface(src, srcrect, dst, dstrect);
    LeaveCriticalSection(&CriticalSection);
    return result;
}
//---------------------------------------------------------------------------
Sprite::RoomPtr GetNeighboringRoom(Sprite::RoomPtr currentRoom, int dx, int stopper)
{
    Sprite::RoomPtr result = currentRoom;
    result.pointer = NULL;

    if(!currentRoom.pointer->HasStopper(stopper))
    {
        char roomname[100];
        sprintf(roomname, "%d.%d.%d", currentRoom.Z, currentRoom.X + dx, currentRoom.Y);
        result.pointer = Rooms[roomname];
        result.X = currentRoom.X + dx;
    }

    return result;
}
//---------------------------------------------------------------------------
int GetScrollDx()
{
    if(!Config::ScrollEnabled)
        return 0;
    
    Sprite::RoomPtr JoeRoom = ((Sprite *)Joe)->CurrentRoom;
    
    Sprite::RoomPtr left = GetNeighboringRoom(JoeRoom, -1, TTiles::LEFT_STOP);
    Sprite::RoomPtr right = GetNeighboringRoom(JoeRoom, 1, TTiles::RIGHT_STOP);

/*    int zoneX = Config::RoomWidth - Config::RoomWidth * Config::ScrollZone;
    int joeX = Joe->GetX() + Joe->GetW();

    if(joeX > zoneX && right.pointer != NULL) // right part
        return joeX - zoneX;

    if(Joe->GetX() < Config::RoomWidth * Config::ScrollZone && left.pointer != NULL)
        return Joe->GetX() - 228;//Config::RoomWidth * Config::ScrollZone - Joe->GetW();
  */

    if(Joe->GetX() > 265 && right.pointer != NULL)
        return Joe->GetX() - 265;

    if(Joe->GetX() < 265 && left.pointer != NULL)
        return Joe->GetX() - 265;

    return 0;
}
//---------------------------------------------------------------------------
int GetGlobalX(Sprite *p)
{
    return p->CurrentRoom.X * (Config::RoomWidth - Config::GridSize) + p->GetX();
}
//---------------------------------------------------------------------------
int GetGlobalY(Sprite *p)
{
    return p->CurrentRoom.Y * (Config::RoomWidth - Config::GridSize) + p->GetY();    
}
//---------------------------------------------------------------------------
int OneRoomLeft()
{
    int GX = GetGlobalX(Joe);
    GX -= (Config::RoomWidth - Config::GridSize);
    int rX = Joe->CurrentRoom.X - 1;

    return GX - rX * (Config::RoomWidth - Config::GridSize);
}
//---------------------------------------------------------------------------
int OneRoomRight()
{
    int GX = GetGlobalX(Joe);
    GX += Joe->GetW();
    GX += (Config::RoomWidth - Config::GridSize);
    int rX = Joe->CurrentRoom.X + 1;
    return GX - rX * (Config::RoomWidth - Config::GridSize);
}
//---------------------------------------------------------------------------
bool IsFarFromJoe(Sprite *p)
{
    int JoeX = GetGlobalX(Joe);
    int pX = GetGlobalX(p);
    
    if(JoeX > pX)
        return JoeX - (pX + p->GetW()) > Config::RoomWidth - Config::GridSize;
    return pX - (JoeX + Joe->GetW()) > Config::RoomWidth - Config::GridSize;
}
//---------------------------------------------------------------------------
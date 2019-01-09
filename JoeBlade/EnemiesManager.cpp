//---------------------------------------------------------------------------
#pragma hdrstop

#include <math.hpp>
#include "EnemiesManager.h"
#include "SimpleEnemy.h"
#include "ArmedEnemy.h"
#include "CleverEnemy.h"
#include "RobotEnemy.h"
#include "CraxBloodfinger.h"
#include "TilesManager.h"
#include "GameCharacter.h"
#include "Room.h"
#include "Utils.h"
#include "JoeBlade.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
TEnemiesManager::TEnemiesManager() : Enemies(Config::MaxEnemies)//, ResurrectTimes(Config::MaxEnemies), WasValid(Config::MaxEnemies)
{
    SimpleEnemyGfx.push_back(new Sprite("Sprites\\fatso-grey.gif"));
    SimpleEnemyGfx.push_back(new Sprite("Sprites\\fatso-mustard.gif"));
    SimpleEnemyGfx.push_back(new Sprite("Sprites\\fatso-yellow.gif"));
    SimpleEnemyGfx.push_back(new Sprite("Sprites\\officer-green.gif"));
    SimpleEnemyGfx.push_back(new Sprite("Sprites\\officer-grey.gif"));
    SimpleEnemyGfx.push_back(new Sprite("Sprites\\officer-yellow.gif"));

    CleverEnemyGfx.push_back(new Sprite("Sprites\\officer-green.gif"));
    CleverEnemyGfx.push_back(new Sprite("Sprites\\officer-grey.gif"));
    CleverEnemyGfx.push_back(new Sprite("Sprites\\officer-yellow.gif"));

    RobotEnemyGfx.push_back(new Sprite("Sprites\\robot-green.gif"));
    RobotEnemyGfx.push_back(new Sprite("Sprites\\robot-red.gif"));
    RobotEnemyGfx.push_back(new Sprite("Sprites\\robot-yellow.gif"));

    ArmedEnemyGfx.push_back(new Sprite("Sprites\\officer-green.gif"));
    ArmedEnemyGfx.push_back(new Sprite("Sprites\\officer-grey.gif"));
    ArmedEnemyGfx.push_back(new Sprite("Sprites\\officer-yellow.gif"));

    CraxGfx = new Sprite("Sprites\\crax.gif");

    // init maxenemies enemies with IsValid = false
    for(int i = 0; i < Config::MaxEnemies; i++)
        Enemies[i] = new SimpleEnemy(SimpleEnemyGfx[0], GameCharacter::LEFT, false, 0);

    InitializeCriticalSection(&EnemyCS);

}
//---------------------------------------------------------------------------
TEnemiesManager::~TEnemiesManager()
{
    for(unsigned i = 0; i < SimpleEnemyGfx.size(); i++)
        delete SimpleEnemyGfx[i];

    for(unsigned i = 0; i < CleverEnemyGfx.size(); i++)
        delete CleverEnemyGfx[i];

    for(unsigned i = 0; i < RobotEnemyGfx.size(); i++)
        delete RobotEnemyGfx[i];

    for(unsigned i = 0; i < ArmedEnemyGfx.size(); i++)
        delete ArmedEnemyGfx[i];

    delete CraxGfx;

    for(int i = 0; i < Config::MaxEnemies; i++)
        delete Enemies[i];
}
//---------------------------------------------------------------------------
void TEnemiesManager::ReInit()
{
    EnterCriticalSection(&EnemyCS);

    for(int i = 0; i < Config::MaxEnemies; i++)
    {
        delete Enemies[i];
        Enemies[i] = new SimpleEnemy(SimpleEnemyGfx[0], GameCharacter::LEFT, false, 0);
    }

    LeaveCriticalSection(&EnemyCS);
}
//---------------------------------------------------------------------------
Enemy *TEnemiesManager::CreateEmptyEnemyObject()
{
    int ArmedIdx = RandomRange(0, ArmedEnemyGfx.size());
    int CleverIdx = RandomRange(0, CleverEnemyGfx.size());
    int RobotIdx = RandomRange(0, RobotEnemyGfx.size());
    int SimpleIdx = RandomRange(0, SimpleEnemyGfx.size());

    if(!CraxCaptured && Joe->CurrentRoom.Z == CraxFloor && Enemies[0] == NULL)
        return new CraxBloodfinger(CraxGfx, GameCharacter::LEFT, true);

    if(Joe->CurrentRoom.Z >= Config::ArmedEnemyLowerFloor && RandomRange(1, 11) <= 3)
        return new ArmedEnemy(ArmedEnemyGfx[ArmedIdx], GameCharacter::LEFT, true, ArmedIdx);

    if(Joe->CurrentRoom.Z >= Config::CleverEnemyLowerFloor && RandomRange(1, 11) <= 5)
        if(RandomRange(0, 2) == 0)
            return new CleverEnemy(CleverEnemyGfx[CleverIdx], GameCharacter::LEFT, true, CleverIdx);
        else
            return new RobotEnemy(RobotEnemyGfx[RobotIdx], GameCharacter::LEFT, true, RobotIdx);

    return new SimpleEnemy(SimpleEnemyGfx[SimpleIdx], GameCharacter::LEFT, true, SimpleIdx);
}
//---------------------------------------------------------------------------
Enemy *TEnemiesManager::CreateNewEnemy()
{
    EnterCriticalSection(&EnemyCS);

    //return NULL;

    int objects[] = { TTiles::UP_ARROW, TTiles::DOWN_ARROW, TTiles::LEFT_ARROW, TTiles::RIGHT_ARROW, TTiles::UP_ARROW_CLST, TTiles::UP_ARROW_SCRT };
    Triple objinfo = ((Sprite *)Joe)->CurrentRoom.pointer->GetRandomObject(objects, sizeof(objects) / sizeof(int), Config::SystemLayer);
    GameCharacter::DirType Direction;
    int X;

    Enemy *p = CreateEmptyEnemyObject();
    p->CurrentRoom = Joe->CurrentRoom;

    if(objinfo.id == TTiles::LEFT_ARROW)
    {
        Direction = GameCharacter::RIGHT;
        X = OneRoomLeft();
        p->ChangeRoom(0, -1, 0);
        //XrX.first = Config::GridSize - Config::GridSize + 1;
    }
    else if(objinfo.id == TTiles::RIGHT_ARROW)
    {
        Direction = GameCharacter::LEFT;
        X = OneRoomRight();
        p->ChangeRoom(0, 1, 0);
        //XrX.first = Config::RoomWidth - p->GetW() + Config::GridSize - 1;
    }
    else
    {
        Direction = (RandomRange(0, 2) == 0 ? GameCharacter::LEFT : GameCharacter::RIGHT);
        X = objinfo.x * Config::GridSize + Config::Xoffset;
    }

    if(dynamic_cast<CraxBloodfinger *>(p) != 0)
        p->ChangeRoom(0, 0, 1);
    p->SetCoords(X, Config::FloorCoord - p->GetH());
    p->SetDirection(Direction);

    LeaveCriticalSection(&EnemyCS);

    return p;
}
//---------------------------------------------------------------------------
void TEnemiesManager::UpdateEnemiesList()
{
    EnterCriticalSection(&EnemyCS);

    //if i-th not valid, possibly resurrect else update
    // resurrect: make new...
    for(int i = 0; i < Config::MaxEnemies; i++)
    {
        if(Enemies[i]->Is_Valid())
            Enemies[i]->Update();

        if(!Enemies[i]->Is_Valid() && GameTimerTicks >= Enemies[i]->GetResurrectTime())
        {
            delete Enemies[i];
            Enemies[i] = NULL;
            Enemies[i] = CreateNewEnemy();
        }
    }

    LeaveCriticalSection(&EnemyCS);
}
//---------------------------------------------------------------------------
void TEnemiesManager::ProcessGrenadeExplosion()
{
    EnterCriticalSection(&EnemyCS);

    for(int i = 0; i < Config::MaxEnemies; i++)
        if(Enemies[i]->Is_Valid() && !IsFarFromJoe(Enemies[i]))
            Enemies[i]->ProcessBulletHit();

    LeaveCriticalSection(&EnemyCS);
}
//---------------------------------------------------------------------------
Enemy* TEnemiesManager::EnemyHitsJoe()
{
    const double coeff = 0.15;

    TRect joerect = Rect(Joe->GetX(), Joe->GetY(), Joe->GetX() + Joe->GetW(), Joe->GetY() + Joe->GetH());
    int w_margin = Joe->GetW() * coeff;
    int h_margin = Joe->GetH() * coeff;
    TRect r;

    for(vector<Enemy *>::iterator p = Enemies.begin(); p != Enemies.end(); p++)
        if((*p)->Is_Valid() && (*p)->CurrentRoom == Joe->CurrentRoom)
        {
            TRect enemyrect = Rect((*p)->GetX(), (*p)->GetY(), (*p)->GetX() + (*p)->GetW(), (*p)->GetY() + (*p)->GetH());

            if(IntersectRect(r, joerect, enemyrect) && r.Width() >= w_margin && r.Height() >= h_margin)
                return *p;
        }

    return NULL;
}
//---------------------------------------------------------------------------
void TEnemiesManager::ShowEnemies()
{
    EnterCriticalSection(&EnemyCS);

    for(vector<Enemy *>::iterator p = Enemies.begin(); p != Enemies.end(); p++)
        (*p)->Show();

    LeaveCriticalSection(&EnemyCS);
}
//---------------------------------------------------------------------------
void TEnemiesManager::RemoveCrax()
{
    EnterCriticalSection(&EnemyCS);

    delete Enemies[0];
    Enemies[0] = NULL;
    Enemies[0] = CreateNewEnemy();

    LeaveCriticalSection(&EnemyCS);
}
//---------------------------------------------------------------------------
void TEnemiesManager::SaveState(FILE *fp)
{
    for(int i = 0; i < Config::MaxEnemies; i++)
    {
        int len = strlen(Enemies[i]->GetSignature());
        int idx = Enemies[i]->GetFramesetIdx();

        fwrite(&len, sizeof(int), 1, fp);
        fwrite(Enemies[i]->GetSignature(), len, 1, fp);
        fwrite(&idx, sizeof(int), 1, fp);

        Enemies[i]->SaveState(fp);
    }
}
//---------------------------------------------------------------------------
void TEnemiesManager::LoadState(FILE *fp)
{
    for(int i = 0; i < Config::MaxEnemies; i++)
    {
        delete Enemies[i];

        char signature[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        int len, idx;

        fread(&len, sizeof(int), 1, fp);
        fread(signature, len, 1, fp);
        fread(&idx, sizeof(int), 1, fp);

        if(!strcmp(signature, "CraxBloodfinger"))
            Enemies[i] = new CraxBloodfinger(CraxGfx, GameCharacter::LEFT, true);
        else if(!strcmp(signature, "RobotEnemy"))
            Enemies[i] = new RobotEnemy(RobotEnemyGfx[idx], GameCharacter::LEFT, true, idx);
        else if(!strcmp(signature, "SimpleEnemy"))
            Enemies[i] = new SimpleEnemy(SimpleEnemyGfx[idx], GameCharacter::LEFT, true, idx);
        else if(!strcmp(signature, "ArmedEnemy"))
            Enemies[i] = new ArmedEnemy(ArmedEnemyGfx[idx], GameCharacter::LEFT, true, idx);
        else if(!strcmp(signature, "CleverEnemy"))
            Enemies[i] = new CleverEnemy(CleverEnemyGfx[idx], GameCharacter::LEFT, true, idx);
        else
            exit(1);

            Enemies[i]->LoadState(fp);
    }
}
//---------------------------------------------------------------------------
TEnemiesManager& EnemiesManager()
{
    static TEnemiesManager manager;

    return manager;
}
//---------------------------------------------------------------------------


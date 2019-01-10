//---------------------------------------------------------------------------
#pragma hdrstop
#include <SDL.h>
#include <algorithm>

#include "JoeBlade.h"
#include "Config.h"
#include "Utils.h"
#include "Room.h"
#include "Bullet.h"
#include "SoundManager.h"
#include "RobotEnemy.h"
#include "CraxBloodfinger.h"
#include "SimpleEnemy.h"
#include "TilesManager.h"
#include "MessagePanel.h"
#include "EnemiesManager.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
void JoeBlade::Init()
{
    SpaceWasPressed = false;
    DownWasPressed = false;
    ArmourOn = false;
    Direction = RIGHT;
    State = NO_JUMP;
    JumpDyIdx = 0;
    MaskTime = GameTimerTicks - Config::ShieldTime - 1;
    ArmourTime = GameTimerTicks - Config::ShieldTime - 1;
    Ammo = Config::InitialAmmo;
    Energy = Config::InitialEnergy;
    SetCoords(Config::StartX, Config::FloorCoord - GetH());
    SetRoom(Config::StartRoomZ, Config::StartRoomX, Config::StartRoomY);
    GameCharacter::Init();
}
//---------------------------------------------------------------------------
void JoeBlade::PerformJumpAction()
{
                                // ������ ������
    const int JumpDy[] = { -4, -6, -4, -4, -4, -4, -4, -4, -4, -4, -2, -2, -2, -2, -2, -2, -2, 0, 0,
                            0, 0, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 6, 4 };

    Y += JumpDy[JumpDyIdx++];    // ��������� Y-���������� ���

    if(JumpDyIdx == sizeof(JumpDy) / sizeof(int))   // ���� ����� ������� ���������
    {
        State = NO_JUMP;            // ��������� � "�����������" (��-������) ���������
        JumpDyIdx = 0;              // �������� - �� ������ ������� JumpDy
    }

    if(State == JUMP_LEFT || State == JUMP_RIGHT)   // ���� ������� � �������, ������� ������� ���
        PerformGoAction(Direction);
}
//---------------------------------------------------------------------------
void JoeBlade::PerformGoAction(DirType dir)         // ������� ��� � ������ �����������
{
    // ���� ��� ����������� �� ������ (����� ��� ������), ��������� �����
    if(!CurrentRoom.pointer->Intersects(this, dir == LEFT ? TTiles::LEFT_STOP : TTiles::RIGHT_STOP, Config::SystemLayer, 0))
        if(Direction != dir)          // ���� ����������� �� ������������� ��������
        {
            Direction = dir;          // �������� �����������
            UpdateFrame();            // ��������� ����� ������
        }
        else
        {
            AdvanceFrame();           // ����� ������ ��������� � ���������� �����
            MakeStep();               // � ������ ���
        }
}
//---------------------------------------------------------------------------
void JoeBlade::Jump_UpdateState()             // ��������, ��� ������ ������� ������� � ���� �� ��������� ������
{                                             // ������ ������� ��������, � ����� ������
    Uint8 *keys = SDL_GetKeyState(NULL);

    if(keys[SDLK_LEFT] == SDL_PRESSED)        // ���� ������� ������
    {
        State = JUMP_LEFT;
        Direction = LEFT;
    }
    else if(keys[SDLK_RIGHT] == SDL_PRESSED)  // ���� ������� �������
    {
        State = JUMP_RIGHT;
        Direction = RIGHT;
    }
    else                                      // ���� ������� "�����"
        State = JUMP;

    UpdateFrame();                            // ��������� ����� ������ (����� �������������)
}
//---------------------------------------------------------------------------
void JoeBlade::Update()
{
    Uint8 *keys = SDL_GetKeyState(NULL);

    if(State == JUMP_LEFT || State == JUMP_RIGHT || State == JUMP)    // ���� ��������� � ��������� ������
        PerformJumpAction();                                          // ��������� ��������� ���� (������� ������� ������������)
    else
    {
        if(keys[SDLK_UP] == SDL_PRESSED)                // ����� (�� � ������) ���� ������ "�����"
            Jump_UpdateState();                              // ��������� � ���� ������
        if(keys[SDLK_LEFT] == SDL_PRESSED)                           // ����� (�� � ������ � �� ������ "�����"):
            PerformGoAction(LEFT);                           // "������" - ��� ������
        if(keys[SDLK_RIGHT] == SDL_PRESSED)             // "�������" - ��� �������
            PerformGoAction(RIGHT);
    }

    if(keys[SDLK_SPACE] == SDL_PRESSED && !SpaceWasPressed)     // ����� ������ (� �� ����� �� ��� ����� - ����� �� ���� "���������" ����)
    {
        if((State == JUMP_LEFT || State == JUMP_RIGHT || State == JUMP) && Inventory[TTiles::GRENADE] > 0)
        {
            Inventory[TTiles::GRENADE]--;
            SoundManager().PlaySound(Sounds::GRENADEEXPLOSION);
            EnemiesManager().ProcessGrenadeExplosion();
        }
        else
            if(Ammo > 0)
            {
                bullet->Init(Direction == LEFT ? X : X + frames[0]->w, Y + 30, Direction == LEFT ? -32 : 32);    // ������������� ����...
                if(!Config::InfAmmo)
                    Ammo--;
                SoundManager().PlaySound(Sounds::GUNSHOT);
            }
            else
                MessagePanel().EnqueueMessage("out of ammo");
    }

    if(!(MaskTime + Config::ShieldTime > GameTimerTicks) && enemybullet->Hits(this) &&
       !(ArmourTime + Config::ShieldTime > GameTimerTicks) && !Config::InfEnergy)
    {
        Energy = std::max(0, Energy - Config::EnergyFactor);
        enemybullet->Remove();
    }
    
    if(!(ArmourTime + Config::ShieldTime > GameTimerTicks) && ArmourOn)
    {
        ArmourOn = false;
        MessagePanel().EnqueueMessage("the vest wears off");
    }

    Enemy *enemy;
    if((enemy = EnemiesManager().EnemyHitsJoe()) != NULL)
    {
        if(dynamic_cast<CraxBloodfinger *>(enemy) != 0)
            GameStatus = CRAX_SUBGAME;

        if(!(MaskTime + Config::ShieldTime > GameTimerTicks))
        {
            if(dynamic_cast<RobotEnemy *>(enemy) != 0 && Energy > 0 && !Config::InfEnergy)
                Energy--;

            if(Energy > 0 && !Config::InfEnergy && dynamic_cast<CraxBloodfinger *>(enemy) == 0)
                Energy--;

            if(Energy == Config::EnergyFactor)
                MessagePanel().EnqueueMessage("nearly dead!");

            if(Energy == 0)
            {
                SoundManager().PlaySound(Sounds::ZEROENERGY);
                MessagePanel().EnqueueMessage("completely dead!!");
                GameStatus = GAMEOVER;
            }
        }
    }

    if(!DownWasPressed && keys[SDLK_DOWN] == SDL_PRESSED && Inventory[TTiles::DISK] > 0 && CurrentRoom.pointer->Intersects(this, TTiles::COMPUTER, Config::SystemLayer, 0.5))
    {
        Inventory[TTiles::DISK]--;
        GameStatus = COMPUTER_SUBGAME;
    }

        if(keys[SDLK_d] == SDL_PRESSED)
            GameStatus = COMPUTER_SUBGAME; /*      SoundManager().PlaySound(Sounds::SECURITY_SUBGAME);*/

    // ���� ��� ���������� ����������� �����, � ������ ��������������� �������, ������� � �������
    if(!DownWasPressed && keys[SDLK_DOWN] == SDL_PRESSED && CurrentRoom.pointer->Intersects(this, TTiles::UP_ARROW, Config::SystemLayer, 0.5))
    {
        ChangeRoom(0, 0, -1);
        SoundManager().PlaySound(Sounds::ENTERDOOR);
    }
    else if(!DownWasPressed && keys[SDLK_DOWN] == SDL_PRESSED && CurrentRoom.pointer->Intersects(this, TTiles::DOWN_ARROW, Config::SystemLayer, 0.5))
    {
        ChangeRoom(0, 0, 1);
        SoundManager().PlaySound(Sounds::ENTERDOOR);
    }
    else if(!DownWasPressed && keys[SDLK_DOWN] == SDL_PRESSED && CurrentRoom.pointer->Intersects(this, TTiles::UP_FLOOR, Config::SystemLayer, 0.5))
    {
        ChangeRoom(1, 0, 0);
        MessagePanel().EnqueueMessage(GetFloorName(Joe->CurrentRoom.Z));
        SoundManager().PlaySound(Sounds::ENTERDOOR);
    }
    else if(!DownWasPressed && keys[SDLK_DOWN] == SDL_PRESSED && CurrentRoom.pointer->Intersects(this, TTiles::DOWN_FLOOR, Config::SystemLayer, 0.5))
    {
        ChangeRoom(-1, 0, 0);
        MessagePanel().EnqueueMessage(GetFloorName(Joe->CurrentRoom.Z));
        SoundManager().PlaySound(Sounds::ENTERDOOR);
    }
    else if(CurrentRoom.pointer->Intersects(this, TTiles::FINISH, Config::SystemLayer, 0.5))
        GameStatus = GAMEVICTORY;
    else if(!DownWasPressed && keys[SDLK_DOWN] == SDL_PRESSED && CurrentRoom.pointer->Intersects(this, TTiles::LIFT_ENTRANCE, Config::SystemLayer, 0.5))
        GameStatus = LIFTMENU;

    // ��� ������� ����� � �������� �����
    Triple object;
    if(!DownWasPressed && keys[SDLK_DOWN] == SDL_PRESSED && CurrentRoom.pointer->Intersects(this, TTiles::UP_ARROW_CLST, Config::SystemLayer, 0.5, &object))
    {
        if(Inventory[TTiles::KEY] > 0)      // ������ ���� ���� ����
        {
            CurrentRoom.pointer->ChangeTile(object, TTiles::UP_ARROW, Config::SystemLayer);   // ������ ������ ����� �� "�������"
            Inventory[TTiles::KEY]--;                                                                  // ������� ���� ����
            ChangeRoom(0, 0, -1);                                                                    // ������� � �������
            SoundManager().PlaySound(Sounds::ENTERDOOR);
        }
    }
     else if(!DownWasPressed && keys[SDLK_DOWN] == SDL_PRESSED && CurrentRoom.pointer->Intersects(this, TTiles::UP_ARROW_SCRT, Config::SystemLayer, 0.5, &object))
    {
        // security
        CurrentRoom.pointer->ChangeTile(object, TTiles::UP_ARROW, Config::SystemLayer);   // ������ ������ ����� �� "�������"
        ChangeRoom(0, 0, -1);                                                                    // ������� � �������
        SoundManager().PlaySound(Sounds::ENTERDOOR);
        if(Inventory[TTiles::SECURITYCARD] > 0)
            Inventory[TTiles::SECURITYCARD]--;
        else
            GameStatus = SECURITY_SUBGAME;
    }

    SpaceWasPressed = (keys[SDLK_SPACE] == SDL_PRESSED);
    DownWasPressed = (keys[SDLK_DOWN] == SDL_PRESSED);

    UpdateRoomLR();

    if(!Config::TimeStop && GameTimerTicks >= DynamiteTimer + Config::DynamiteTimerTime)
    {
        SoundManager().PlaySound(Sounds::OUTOFTIME);
        MessagePanel().EnqueueMessage("you have run out of time! too late!");
        GameStatus = GAMEOVER;
    }

    if(CurrentRoom.pointer->Intersects(this, TTiles::ANYOBJECT, Config::ObjectsLayer, 0.3, &object))   // ���� �������� (�����=0.2) ���� �� ������� ��������
    {
        if(object.id == TTiles::MINE)          // todo: GAMEOVER
        {
            SoundManager().PlaySound(Sounds::MINEEXPLOSION);
            MessagePanel().EnqueueMessage("oh dear! you have stepped on a mine!");
            GameStatus = GAMEOVER;
        }
        else if(object.id == TTiles::DYNAMITE && Inventory[TTiles::DETONATOR] > 0)
        {
            MessagePanel().EnqueueMessage("explosive rigged");
            Inventory[TTiles::DYNAMITE]++;
            if(--Config::DynamitesTotal == 0)
                ShowNextObjective(DYNAMITE_RIGGED);
            SoundManager().PlaySound(Sounds::GETOBJECT);
            Inventory[TTiles::DETONATOR]--;
            CurrentRoom.pointer->ChangeTile(object, TTiles::DYNAMITE_ARMED, Config::ObjectsLayer);
            Score += Config::ObjectiveItemScore;
            
            if(!Config::TimeStop)
                DynamiteTimerEnabled = true;
            DynamiteTimer = GameTimerTicks;
        }
        else if(object.id == TTiles::DYNAMITE && Inventory[TTiles::DETONATOR] == 0)
        {
            if(strcmp(MessagePanel().GetBottomMessage(), "no detonators"))
                MessagePanel().EnqueueMessage("no detonators");
        }
        else if(object.id == TTiles::DYNAMITE_ARMED)  // do nothing
            ;
        else
        {
            SoundManager().PlaySound(Sounds::GETOBJECT);
            MessagePanel().EnqueueMessage(GetObjectMessage(object.id));
            CurrentRoom.pointer->RemoveTile(object, Config::ObjectsLayer);             // ������� ������ � ������

            Score += (object.id == TTiles::HOSTAGE) ? Config::ObjectiveItemScore : Config::ItemScore;

            if(object.id == TTiles::HOSTAGE && --Config::HostagesTotal == 0)
                ShowNextObjective(HOSTAGES_FOUND);
            if(object.id == TTiles::AMMUNITION)
                Ammo = std::min(Config::InitialAmmo, Ammo + Config::AmmoPackSize);
            else if(object.id == TTiles::FOOD)
                Energy = std::min(Config::InitialEnergy, Energy + Config::EnergyPackSize);
            else if(object.id == TTiles::MASK)
                MaskTime = GameTimerTicks;
            else if(object.id == TTiles::ARMOUR)
            {
                ArmourTime = GameTimerTicks;
                ArmourOn = true;
            }
            else
                Inventory[object.id]++;                                                 // � ����� ���� � ������ ;)
        }
    }
}
//---------------------------------------------------------------------------
void JoeBlade::Show() 
{
    if(MaskTime + Config::ShieldTime > GameTimerTicks)
    {
        frames.swap(Masked.frames);
        GameCharacter::Show();
        frames.swap(Masked.frames);
    }
    else
        GameCharacter::Show();
}
//---------------------------------------------------------------------------


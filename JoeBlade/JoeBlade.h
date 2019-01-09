//---------------------------------------------------------------------------
#ifndef JoeBladeH
#define JoeBladeH

#include "GameCharacter.h"

//---------------------------------------------------------------------------
class JoeBlade : public GameCharacter
{
    enum { JUMP_LEFT, JUMP_RIGHT, JUMP, NO_JUMP } State;   // ��������� ���

    bool SpaceWasPressed, DownWasPressed, ArmourOn;
    int JumpDyIdx;   // �������� � ������� JumpDy
    int ArmourTime, MaskTime;
    Sprite Masked;

    void PerformJumpAction();            // ��������� ��������� ���� ������
    void PerformGoAction(DirType dir);   // ��������� ��������� ���� ��������
    void Jump_UpdateState();             // �������� ��������� ��� ��� �������, ��� ������ ��� ������� ������

    public:
        int Ammo, Energy;

        JoeBlade(const AnsiString& name, int RoomZ = 0, int RoomX = 0, int RoomY = 0) : GameCharacter(name, RIGHT, true, RoomZ, RoomX, RoomY),
                                                                                        Masked("Sprites\\officer-green.gif") {}

        virtual void SaveState(FILE *fp)
        {
            fwrite(&Ammo, sizeof(int), 1, fp);
            fwrite(&Energy, sizeof(int), 1, fp);
            fwrite(&State, sizeof(State), 1, fp);
            fwrite(&ArmourOn, sizeof(bool), 1, fp);
            fwrite(&JumpDyIdx, sizeof(int), 1, fp);
            fwrite(&ArmourTime, sizeof(int), 1, fp);
            fwrite(&MaskTime, sizeof(int), 1, fp);
            GameCharacter::SaveState(fp);
        }

        virtual void LoadState(FILE * fp)
        {
            fread(&Ammo, sizeof(int), 1, fp);
            fread(&Energy, sizeof(int), 1, fp);
            fread(&State, sizeof(State), 1, fp);
            fread(&ArmourOn, sizeof(bool), 1, fp);
            fread(&JumpDyIdx, sizeof(int), 1, fp);
            fread(&ArmourTime, sizeof(int), 1, fp);
            fread(&MaskTime, sizeof(int), 1, fp);
            GameCharacter::LoadState(fp);
        }

        void Init();
        virtual void Update();
        virtual void Show();                                // ���������� �� ������

};
//---------------------------------------------------------------------------
#endif

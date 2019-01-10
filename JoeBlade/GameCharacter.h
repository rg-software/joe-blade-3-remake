//---------------------------------------------------------------------------
#ifndef GameCharacterH
#define GameCharacterH

#include "Sprite.h"
#include <SDL.h>
#include "Config.h"

//---------------------------------------------------------------------------
class GameCharacter : public Sprite
{
    public:
        enum DirType { LEFT, RIGHT };                // направление спрайта
        DirType Direction;
//        enum StateType { ACTIVE, PASSIVE };

    protected:
//        StateType CharacterState;
        bool IsValid;
        int newticks;

    public:
        virtual void SaveState(FILE *f)
        {
            fwrite(&Direction, sizeof(DirType), 1, f);
            fwrite(&IsValid, sizeof(bool), 1, f);
            fwrite(&newticks, sizeof(int), 1, f);
            Sprite::SaveState(f);
        }

        virtual void LoadState(FILE *f)
        {
            fread(&Direction, sizeof(DirType), 1, f);
            fread(&IsValid, sizeof(bool), 1, f);
            fread(&newticks, sizeof(int), 1, f);
            Sprite::LoadState(f);
        }

        /*        GameCharacter(const AnsiString& name, DirType dir, int RoomZ = 0, int RoomX = 0, int RoomY = 0) : Sprite(name, RoomZ, RoomX, RoomY),
                                                                                                           Direction(dir), CharacterState(ACTIVE)
        {
            Y = Config::FloorCoord - GetH();
            UpdateFrame();
        } */

        GameCharacter(const AnsiString& name, DirType dir, bool isvalid = true, int RoomZ = 0, int RoomX = 0, int RoomY = 0) : Sprite(name, RoomZ, RoomX, RoomY),
                                                                                                           Direction(dir), IsValid(isvalid)
        {
            Y = Config::FloorCoord - GetH();
            UpdateFrame();
        }

        GameCharacter(Sprite *framesource, DirType dir, bool isvalid) : Sprite(framesource), Direction(dir), IsValid(isvalid) {}

        GameCharacter(GameCharacter *source) : Sprite(source), Direction(source->Direction), IsValid(source->IsValid) {}

        void Init()
        {
            Y = Config::FloorCoord - GetH();
            UpdateFrame();
        }
        int MinFrameIndex() const { return Direction == RIGHT ? Config::SpriteRightMinFrame : Config::SpriteLeftMinFrame; } // номер первого кадра
        int MaxFrameIndex() const { return Direction == RIGHT ? Config::SpriteRightMaxFrame : Config::SpriteLeftMaxFrame; } // номер последнего кадра
        void AdvanceFrame() { if(CurrentFrame++ == MaxFrameIndex()) CurrentFrame = MinFrameIndex(); }                 // переход к след. кадру
        void UpdateFrame();                  // обновить (если требуется) текущий набор кадров (левые/правые)
        void MakeStep() { X += (Direction == LEFT) ? -4 : 4; }           // сделать один шаг (налево/направо) - сдвиг на 4 пикселя
        virtual void Update() = 0;
        virtual void Show();
        void SetTimeToWait(int msecs);
        void UpdateRoomLR();
        int DistanceToJoe();
  //      bool IsActive() { return CharacterState == ACTIVE; }
        bool Is_Valid() { return IsValid; }
        void SetDirection(DirType dir) { Direction = dir; UpdateFrame(); }
};
//---------------------------------------------------------------------------
#endif

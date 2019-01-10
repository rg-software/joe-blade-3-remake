//---------------------------------------------------------------------------

#ifndef SpriteH
#define SpriteH

#include <vector>
#include "Globals.h"
#include <SDL.h>
#include <MyGIFStructure.h>

using namespace std;

//---------------------------------------------------------------------------
class Sprite
{
    protected:
        vector<SDL_Surface*> frames;               // ����� �������
        int CurrentFrame;                          // ����� �������� �����
        int X, Y;                                  // ���������� �� ������ (������������ ���� �������)

        void Load(const AnsiString& name);               // ��������� ������ � ������ ������
        friend class JoeBlade;

    public:
        struct RoomPtr
        {
            Room *pointer; int X, Y, Z;

            bool operator==(const RoomPtr& rhs) const { return pointer == rhs.pointer; }
            bool operator!=(const RoomPtr& rhs) const { return pointer != rhs.pointer; }
        }
        CurrentRoom;

        Sprite(const AnsiString& name, int RoomZ = 0, int RoomX = 0, int RoomY = 0)
        {
            Load(name);
            SetRoom(RoomZ, RoomX, RoomY);
        }

        Sprite(Sprite *sprite);

        virtual void SaveState(FILE *fp)
        {
            fwrite(&CurrentFrame, sizeof(int), 1, fp);
            fwrite(&X, sizeof(int), 1, fp);
            fwrite(&Y, sizeof(int), 1, fp);
            fwrite(&CurrentRoom.X, sizeof(int), 1, fp);
            fwrite(&CurrentRoom.Y, sizeof(int), 1, fp);
            fwrite(&CurrentRoom.Z, sizeof(int), 1, fp);
        }

        virtual void LoadState(FILE *fp)
        {
            fread(&CurrentFrame, sizeof(int), 1, fp);
            fread(&X, sizeof(int), 1, fp);
            fread(&Y, sizeof(int), 1, fp);
            fread(&CurrentRoom.X, sizeof(int), 1, fp);
            fread(&CurrentRoom.Y, sizeof(int), 1, fp);
            fread(&CurrentRoom.Z, sizeof(int), 1, fp);
            SetRoom(CurrentRoom.Z, CurrentRoom.X, CurrentRoom.Y);  // ����� ���������� CurrentRoom.pointer
        }

        void SetRoom(int z, int x, int y);                 // ��������� ������� �������
        void ChangeRoom(int dz, int dx, int dy);           // ������� �� ������� ������� �� ������ ���������

        int GetX() const { return X; }
        int GetY() const { return Y; }
        int GetH() const { return frames[0]->h; }
        int GetW() const { return frames[0]->w; }
        void SetCoords(int x, int y) { X = x; Y = y; }
        virtual void Show();                         // ���������� ������ �� ������
        virtual ~Sprite()
        {
            for(unsigned i = 0; i < frames.size(); i++)
                SDL_FreeSurface(frames[i]);
        }
};
//---------------------------------------------------------------------------

#endif

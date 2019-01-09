//---------------------------------------------------------------------------
#ifndef SmokeH
#define SmokeH

#include "Sprite.h"
//---------------------------------------------------------------------------
class Smoke : public Sprite
{
    private:
        bool is_valid;

    public:
        Smoke(const AnsiString& name, int RoomZ = 0, int RoomX = 0, int RoomY = 0) : is_valid(false), Sprite(name, RoomZ, RoomX, RoomY) {}
        void Invalidate() {is_valid = false; }
        bool IsValid() const { return is_valid; }
        void Init(int x, int y) { X = x; Y = y; is_valid = true; CurrentFrame = 0; }
        void Show()
        {
            if(is_valid)
                Sprite::Show();
        }

        void AdvanceFrame()
        {
            if(CurrentFrame == (int)frames.size() - 1)
                is_valid = false;
            else
                CurrentFrame++;
        }

        virtual void SaveState(FILE *fp)
        {
            fwrite(&is_valid, sizeof(bool), 1, fp);
            Sprite::SaveState(fp);
        }

        virtual void LoadState(FILE *fp)
        {
            fread(&is_valid, sizeof(bool), 1, fp);
            Sprite::LoadState(fp);
        }
};
//---------------------------------------------------------------------------
#endif

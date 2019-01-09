//---------------------------------------------------------------------------
#ifndef BulletH
#define BulletH

#include "Config.h"
#include "Utils.h"
#include "Sprite.h"
//---------------------------------------------------------------------------
class Bullet : public Sprite
{
    int step;

    public:
        Bullet(const AnsiString& name) : Sprite(name) { X = 0; Y = -10; }

        void Init(int x, int y, int dx) { SetCoords(x, y); step = dx; CurrentRoom = ((Sprite *)Joe)->CurrentRoom; }
        void Remove() { Init(0, -10, 0); }
        bool IsValid() { return !(X == 0 && Y == -10); }

        void Update()
        {
            if(!IsFarFromJoe(this))
                X += step;
            else
                Remove();
        }

        bool Hits(Sprite *p)
        {
            int bulletX = GetGlobalX(this);
            int bulletY = GetGlobalY(this);
            int enemyX = GetGlobalX(p);
            int enemyY = GetGlobalY(p);
            
            TRect r;
            TRect bullet = Rect(bulletX, bulletY, bulletX + GetW(), bulletY + GetH());
            TRect sprite = Rect(enemyX, enemyY, enemyX + p->GetW(), enemyY + p->GetH());

            return IntersectRect(r, bullet, sprite);// && CurrentRoom.pointer == p->CurrentRoom.pointer;
        }

        virtual void SaveState(FILE *fp)
        {
            fwrite(&step, sizeof(int), 1, fp);
            Sprite::SaveState(fp);
        }

        virtual void LoadState(FILE *fp)
        {
            fread(&step, sizeof(int), 1, fp);
            Sprite::LoadState(fp);
        }
};
//---------------------------------------------------------------------------
#endif

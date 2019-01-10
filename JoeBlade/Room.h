//---------------------------------------------------------------------------

#ifndef RoomH
#define RoomH

#include <vector>
#include <vcl.h>
#include <SDL.h>
#include <list>
#include "ComputerSubgame.h"

using namespace std;
class Sprite;

//---------------------------------------------------------------------------
struct Triple                // тройка чисел (x, y, id) тайла
{
    int x, y, id;

    Triple() {}
    Triple(int ax, int ay, int aid) : x(ax), y(ay), id(aid) {}
    bool operator==(const Triple& rhs) const { return x == rhs.x && y == rhs.y && id == rhs.id; }
};
//---------------------------------------------------------------------------
class Room                       // одиночная комната
{
    vector<list<Triple> > Data;      // комната - массив слоёв, а каждый слой - список троек
    SDL_Surface *cache;              // хранит сгенерированную комнату
    bool is_cachevalid;              // соответствует ли кэш комнате в данный момент времени?

    void MakeRoomSurface();          // сгенерировать комнату и записать в кэш

    // определяет, пересекается ли спрайт с данным системным объектом на данном слое
    //для проверки использует переданную булеву функцию
    //bool GenericIntersects(const Sprite* sprite, int sysobj, int layer, bool (*func)(const TRect *, const Sprite*, int));

    public:
        Room(const AnsiString& name);                             // загрузаем комнату с именем name
        ~Room() { if(cache != NULL) SDL_FreeSurface(cache); }
        void Show(SDL_Rect* src, int xOffset, int yOffset);
        void Show();                                              // отобразить комнату на экране
        void DestroyCache() { if(cache != NULL) SDL_FreeSurface(cache); is_cachevalid = false; cache = NULL; }
        void SaveLayer(int layer, FILE *fp);
        void LoadLayer(int layer, FILE *fp);

        // пересекается ли спрайт с данным объектом
        // (находясь полностью на его фоне или покрывая не менее coeff от его ширины и coeff от его высоты)
        bool Intersects(const Sprite* sprite, int sysobj, int layer, double coeff, Triple *object);
        bool Room::Intersects(const Sprite* sprite, int sysobj, int layer, double coeff)
        {
            Triple object;
            return Intersects(sprite, sysobj, layer, coeff, &object);
        }
        bool HasStopper(int stopper);
        void RemoveTile(const Triple& object, int layer);                 // удалить объект со слоя
        void ChangeTile(const Triple& srctile, int desttile, int layer);  // поместить на место объекта c id=srctile объект с id=desttile на данном слое
        Triple GetRandomObject(int *object_ids, int N, int layer);

        friend void ShuffleObjects();
        friend void TComputerSubgame::ShowObjectsMenu(); 
};
//---------------------------------------------------------------------------
#endif

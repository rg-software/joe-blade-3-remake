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
struct Triple                // ������ ����� (x, y, id) �����
{
    int x, y, id;

    Triple() {}
    Triple(int ax, int ay, int aid) : x(ax), y(ay), id(aid) {}
    bool operator==(const Triple& rhs) const { return x == rhs.x && y == rhs.y && id == rhs.id; }
};
//---------------------------------------------------------------------------
class Room                       // ��������� �������
{
    vector<list<Triple> > Data;      // ������� - ������ ����, � ������ ���� - ������ �����
    SDL_Surface *cache;              // ������ ��������������� �������
    bool is_cachevalid;              // ������������� �� ��� ������� � ������ ������ �������?

    void MakeRoomSurface();          // ������������� ������� � �������� � ���

    // ����������, ������������ �� ������ � ������ ��������� �������� �� ������ ����
    //��� �������� ���������� ���������� ������ �������
    //bool GenericIntersects(const Sprite* sprite, int sysobj, int layer, bool (*func)(const TRect *, const Sprite*, int));

    public:
        Room(const AnsiString& name);                             // ��������� ������� � ������ name
        ~Room() { if(cache != NULL) SDL_FreeSurface(cache); }
        void Show(SDL_Rect* src, int xOffset, int yOffset);
        void Show();                                              // ���������� ������� �� ������
        void DestroyCache() { if(cache != NULL) SDL_FreeSurface(cache); is_cachevalid = false; cache = NULL; }
        void SaveLayer(int layer, FILE *fp);
        void LoadLayer(int layer, FILE *fp);

        // ������������ �� ������ � ������ ��������
        // (�������� ��������� �� ��� ���� ��� �������� �� ����� coeff �� ��� ������ � coeff �� ��� ������)
        bool Intersects(const Sprite* sprite, int sysobj, int layer, double coeff, Triple *object);
        bool Room::Intersects(const Sprite* sprite, int sysobj, int layer, double coeff)
        {
            Triple object;
            return Intersects(sprite, sysobj, layer, coeff, &object);
        }
        bool HasStopper(int stopper);
        void RemoveTile(const Triple& object, int layer);                 // ������� ������ �� ����
        void ChangeTile(const Triple& srctile, int desttile, int layer);  // ��������� �� ����� ������� c id=srctile ������ � id=desttile �� ������ ����
        Triple GetRandomObject(int *object_ids, int N, int layer);

        friend void ShuffleObjects();
        friend void TComputerSubgame::ShowObjectsMenu(); 
};
//---------------------------------------------------------------------------
#endif

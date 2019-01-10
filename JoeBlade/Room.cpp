//---------------------------------------------------------------------------
#pragma hdrstop

#include "Utils.h"
#include "Room.h"
#include "Config.h"
#include "Sprite.h"
#include "Globals.h"
#include "stdlib.h"
#include "ResourceManager.h"
#include "TilesManager.h"
#include <memory>
#include <algorithm>
#include <Math.hpp>

#pragma package(smart_init)

//---------------------------------------------------------------------------
Room::Room(const AnsiString& name) : cache(NULL)    // ��������� ������� name
{
    std::auto_ptr<TStream> stream(ResourceManager().GetResourceStream(name.c_str()));
    Data.resize(Config::N_of_layers);

    is_cachevalid = false;
    for(int layer = 0; layer < Config::N_of_layers; layer++)
    {
        int size;
        stream->ReadBuffer(&size, sizeof(int));       // ��� ������� ���� ������ ���������� �����

        for(int i = 0; i < size; i++)
        {
            int x, y, id;

            stream->ReadBuffer(&x, sizeof(int));      // ��������� ��������� ������
            stream->ReadBuffer(&y, sizeof(int));
            stream->ReadBuffer(&id, sizeof(int));

            Data[layer].push_back(Triple(x, y, id));  // � ��������� � � �������� �������
        }
    }
}
//---------------------------------------------------------------------------
void Room::MakeRoomSurface()                    // ������������� ������� � �������� � ���
{
    if(cache == NULL)       // ������ ���-����������� �������� � �������
        cache = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_ANYFORMAT, Config::RoomWidth, Config::RoomHeight,
                    Config::BackBuffer->format->BitsPerPixel, Config::BackBuffer->format->Rmask,
                    Config::BackBuffer->format->Gmask, Config::BackBuffer->format->Bmask, Config::BackBuffer->format->Amask);

    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.w = cache->w; rect.h = cache->h;
    SDL_FillRect(cache, &rect, 0);       

    for(int layer = 0; layer < Config::N_of_layers; layer++)     // ���� �� ���� �����
        if(layer != Config::SystemLayer)                         // ���� ������ ���� - �� ���������
            for(list<Triple>::const_iterator p = Data[layer].begin(); p != Data[layer].end(); p++)
            {
                SDL_Rect r;                           // �������������� ����
                SDL_Surface *tile = TilesManager().GetTileByID(p->id);

                if(layer == 0)
                     SDL_SetColorKey(tile, 0, 0);

                r.x = p->x * Config::GridSize;
                r.y = p->y * Config::GridSize;
                r.w = tile->w;
                r.h = tile->h;

                My_SDL_BlitSurface(tile, NULL, cache, &r);     // � ������ ��� � ����
            }
}
//---------------------------------------------------------------------------
void Room::Show()                      // ���������� ������� �� ������
{
    Show(NULL, 0, 0);
}
//---------------------------------------------------------------------------
void Room::Show(SDL_Rect* src, int xOffset, int yOffset)
{
    if(!is_cachevalid)
    {
        MakeRoomSurface();                 // �������������� ������������ �����������
        is_cachevalid = true;
    }

    SDL_Rect dest;                               // ������� ���������� ���� �� �����
    dest.x = Config::Xoffset + xOffset;
    dest.y = Config::Yoffset + yOffset;

    My_SDL_BlitSurface(cache, src, Config::BackBuffer, &dest);
}
//---------------------------------------------------------------------------
void Room::SaveLayer(int layer, FILE *fp)
{
    int size = (int)Data[layer].size();

    fwrite(&size, sizeof(int), 1, fp);
    for(list<Triple>::iterator p = Data[layer].begin(); p != Data[layer].end(); p++)
    {
        fwrite(&p->x, sizeof(int), 1, fp);
        fwrite(&p->y, sizeof(int), 1, fp);
        fwrite(&p->id, sizeof(int), 1, fp);
    }
}
//---------------------------------------------------------------------------
void Room::LoadLayer(int layer, FILE *fp)
{
    int size;

    fread(&size, sizeof(int), 1, fp);
    Data[layer].clear();

    for(int i = 0; i < size; i++)
    {
        int x, y, id;

        fread(&x, sizeof(int), 1, fp);
        fread(&y, sizeof(int), 1, fp);
        fread(&id, sizeof(int), 1, fp);

        Data[layer].push_back(Triple(x, y, id));
    }
}
//---------------------------------------------------------------------------
bool Room::Intersects(const Sprite* sprite, int sysobj, int layer, double coeff, Triple *object)
{
    TRect spriterect = Rect(sprite->GetX(), sprite->GetY(), sprite->GetX() + sprite->GetW(), sprite->GetY() + sprite->GetH());

    for(list<Triple>::const_iterator p = Data[layer].begin(); p != Data[layer].end(); p++)    // ������������ ��� ������� ����
        if(p->id == sysobj || sysobj == TTiles::ANYOBJECT)                                    // ���� ������ ������ ������� ����
        {
            TRect r;
            SDL_Surface *tile = TilesManager().GetTileByID(p->id);
            int w_margin = TilesManager().GetTileByID(p->id)->w * coeff;    // �������� �����, ����������� ���
            int h_margin = TilesManager().GetTileByID(p->id)->h * coeff;    // ������������ �����������

            // ���� ������������� �����������, ������� ��� ���������
            if(IntersectRect(r, spriterect, Rect(p->x * Config::GridSize, p->y * Config::GridSize, p->x * Config::GridSize + tile->w,
                                                 p->y * Config::GridSize + tile->h)))
            {
                // r - ������������� �����������; ���� ����������� ������������� ���������, return true
                if((int)r.Width() >= std::min(sprite->GetW(), w_margin) && (int)r.Height() >= min(sprite->GetH(), h_margin))
                {
                    *object = *p;
                    return true;
                }
            }
        }

    return false;
}
//---------------------------------------------------------------------------
bool Room::HasStopper(int stopper)
{
    for(list<Triple>::const_iterator p = Data[Config::SystemLayer].begin(); p != Data[Config::SystemLayer].end(); p++)    // ������������ ��� ������� ����
        if(p->id == stopper)
            return true;
    return false;
}
//---------------------------------------------------------------------------
void Room::ChangeTile(const Triple& srctile, int desttile, int layer)
{
    for(list<Triple>::iterator p = Data[layer].begin(); p != Data[layer].end(); p++)
        if(*p == srctile)
        {
            p->id = desttile;
            is_cachevalid = false;
            break;
        }
}
//---------------------------------------------------------------------------
void Room::RemoveTile(const Triple& object, int layer)
{
    for(list<Triple>::iterator p = Data[layer].begin(); p != Data[layer].end(); p++)
        if(*p == object)
        {
            Data[layer].erase(p);;
            is_cachevalid = false;
            break;
        }
}
//---------------------------------------------------------------------------
Triple Room::GetRandomObject(int *object_ids, int N, int layer)
{
    int objects = 0;
    for(list<Triple>::const_iterator p = Data[layer].begin(); p != Data[layer].end(); p++)    // ������������ ��� ������� ����
        for(int i = 0; i < N; i++)
            if(p->id == object_ids[i])                                                       // ���� ������ ������ ������� ����
                objects++;

    int object = RandomRange(0, objects);
    int curobj = 0;
    for(list<Triple>::const_iterator p = Data[layer].begin(); p != Data[layer].end(); p++)    // ������������ ��� ������� ����
        for(int i = 0; i < N; i++)
            if(p->id == object_ids[i])                                                       // ���� ������ ������ ������� ����
                if(curobj++ == object)
                    return *p;

    return Triple(0, 0, 0);
}
//---------------------------------------------------------------------------


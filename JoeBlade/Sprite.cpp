//---------------------------------------------------------------------------
#pragma hdrstop

#include "Utils.h"
#include "Sprite.h"
#include "Config.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <memory>
#include "Globals.h"
#include "Room.h"
#include <sdl.h>
#include <vector>

#pragma package(smart_init)

using namespace std;

//---------------------------------------------------------------------------
Sprite::Sprite(Sprite *sprite)
{
    CurrentFrame = sprite->CurrentFrame;
    X = sprite->X;
    Y = sprite->Y;
    CurrentRoom = sprite->CurrentRoom;
    for(vector<SDL_Surface *>::iterator p = sprite->frames.begin(); p != sprite->frames.end(); p++)
    {
        SDL_Surface *surf = SDL_ConvertSurface(*p, (*p)->format, (*p)->flags);
        SDL_SetColorKey(surf, SDL_SRCCOLORKEY, SDL_MapRGB(surf->format, Config::ColorKey[0], Config::ColorKey[1], Config::ColorKey[2]));
        frames.push_back(surf);
    }
}
//---------------------------------------------------------------------------
void Sprite::Load(const AnsiString& name)
{
    for(unsigned i = 0; i < frames.size(); i++)
        SDL_FreeSurface(frames[i]);

    frames.clear();
    auto_ptr<TStream> stream(ResourceManager().GetResourceStream(name.c_str()));

    MyGIFStructure gif = MyGIFStructure(stream.get());                        // считываем спрайт из потока
    CurrentFrame = 0;

    for(int i = 0; i < gif.NumberOfFrames; i++)               // по всем кадрам
    {
        SDL_Surface* tile;      // создать тайл
        SDL_Surface* gifsur = gif.Frames[i];

        tile = SDL_CreateRGBSurface(gifsur->flags, gifsur->w, gifsur->h, Config::ScreenParams[2], gifsur->format->Rmask,
                                        gifsur->format->Gmask, gifsur->format->Bmask, gifsur->format->Amask);

  //        tile = SDL_ConvertSurface(gif.Frames[i], gif.Frames[i]->format, gif.Frames[i]->flags);
        My_SDL_BlitSurface(gifsur, NULL, tile, NULL);

        SDL_SetColorKey(tile, SDL_SRCCOLORKEY, SDL_MapRGB(tile->format, Config::ColorKey[0], Config::ColorKey[1], Config::ColorKey[2]));
        frames.push_back(tile);
    }
}
//---------------------------------------------------------------------------
void Sprite::Show()                                // отобразить на экране
{
    RoomPtr JoeRoom = ((Sprite *)Joe)->CurrentRoom;

    SDL_Rect rect;
    rect.x = X + Config::Xoffset; rect.y = Y + Config::Yoffset;
    rect.w = frames[0]->w;
    rect.h = frames[0]->h;

    rect.x -= GetScrollDx();

    bool doBlit = false;

    if(CurrentRoom == JoeRoom) doBlit = true;
    
    std::pair<int, int> curYZ = std::make_pair(CurrentRoom.Y, CurrentRoom.Z), joeYZ = std::make_pair(JoeRoom.Y, JoeRoom.Z);
    
    if(CurrentRoom.X + 1 == JoeRoom.X && curYZ == joeYZ && Config::ScrollEnabled) // комната слева
    {
        rect.x -= (Config::RoomWidth - Config::GridSize);     
        //rect.x -= (Config::RoomWidth - rect.w + 16/*Config::GridSize +3*/); //$$
        doBlit = true;
    }

    if(CurrentRoom.X - 1 == JoeRoom.X && curYZ == joeYZ && Config::ScrollEnabled) // комната справа
    {
        rect.x += (Config::RoomWidth - Config::GridSize);     
        //rect.x += (Config::RoomWidth - rect.w + 16/*Config::GridSize +3*/);     //$$
        doBlit = true;
    }

    if(doBlit)
        My_SDL_BlitSurface(frames[CurrentFrame], NULL, SDL_GetVideoSurface(), &rect);
}
//---------------------------------------------------------------------------
void Sprite::SetRoom(int z, int x, int y)
{
    CurrentRoom.X = x;
    CurrentRoom.Y = y;
    CurrentRoom.Z = z;

    char roomname[100];

    // достаём комнату из массива (имя комнаты имеет вид Z.X.Y)
    sprintf(roomname, "%d.%d.%d", z, x, y);
    CurrentRoom.pointer = Rooms[roomname];
}
//---------------------------------------------------------------------------
void Sprite::ChangeRoom(int dz, int dx, int dy)
{
    SetRoom(CurrentRoom.Z + dz, CurrentRoom.X + dx, CurrentRoom.Y + dy);
}
//---------------------------------------------------------------------------


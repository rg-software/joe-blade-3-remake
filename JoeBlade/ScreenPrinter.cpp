//---------------------------------------------------------------------------
#pragma hdrstop

#include "ScreenPrinter.h"
#include <string.h>
#include "Utils.h"

#pragma package(smart_init)

using namespace std;

//---------------------------------------------------------------------------
TScreenPrinter::TScreenPrinter()
{
    letters = LoadSurfaceFromGIFResource("Common\\letters.gif");
    numbers = LoadSurfaceFromGIFResource("Common\\numbers-yg.gif");
    whitenumbers = LoadSurfaceFromGIFResource("Common\\numbers.gif");
}
//---------------------------------------------------------------------------
TScreenPrinter::~TScreenPrinter()
{
    SDL_FreeSurface(letters);
    SDL_FreeSurface(numbers);
    SDL_FreeSurface(whitenumbers);
}
//---------------------------------------------------------------------------
void TScreenPrinter::Print(int x, int y, char *message)
{
    Print(x, y, message, SDL_GetVideoSurface());
}
//---------------------------------------------------------------------------
void TScreenPrinter::Print(int x, int y, char *message, SDL_Surface *surface)
{
    char msg[200];

    strcpy(msg, message);

    int len = strlen(msg);
    for(int i = 0; i < len; i++)   // ��������� ����� �������� ����� �����
        if(msg[i] == '-')          // ����� z, ��� ��� ������ ���� �� �������
            msg[i] = 'z' + 1;      // ������ ���� z
        else if(msg[i] == ' ')     // ������ �������� ����� ������
            msg[i] = 'z' + 2;
        else if(msg[i] == '>')     // ����� ������� ������
            msg[i] = 'z' + 3;
        else if(msg[i] == '<')     // � ������� �����
            msg[i] = 'z' + 4;
        else if(msg[i] == ',')     // �������
            msg[i] = 'z' + 5;
        else if(msg[i] == '.')     // �����
            msg[i] = 'z' + 6;
        else if(msg[i] == '!')     // �����. ����
            msg[i] = 'z' + 7;
        else if(msg[i] == ':')     // ���������
            msg[i] = 'z' + 8;


    int char_w = CharW();
    int char_h = CharH();

    SDL_Rect src, dest;
    src.y = 0;
    src.w = char_w;
    src.h = char_h;
    dest.y = y;

    SDL_Surface *p;
    for(int i = 0; i < len; i++)
    {
        char c = msg[i];
        if(c >= '0' && c <= '9')       // ���� ������ - �����
        {
            c -= '0';                  // ���������� ���������� ����� �������
            p = numbers;               // ������������� ��������������� �����������
        }
        else if(c >= 'A' && c <= 'J')  // ����� �����
        {
            c -= 'A';
            p = whitenumbers;
        }
        else
        {
            c -= 'a';                  // ����������...
            p = letters;
        }

        src.x = c * char_w;
        dest.x = char_w * i + x;

        My_SDL_BlitSurface(p, &src, surface, &dest);
    }
}
//---------------------------------------------------------------------------
void TScreenPrinter::PrintCAligned(int y, char *message)
{
    PrintCAligned(y, message, SDL_GetVideoSurface());
}
//---------------------------------------------------------------------------
void TScreenPrinter::PrintCAligned(int y, char *message, SDL_Surface *surface)
{
    int x = (surface->w - strlen(message) * CharW()) / 2;
    Print(x, y, message, surface);
}
//---------------------------------------------------------------------------


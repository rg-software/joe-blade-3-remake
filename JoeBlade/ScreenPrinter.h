//---------------------------------------------------------------------------

#ifndef ScreenPrinterH
#define ScreenPrinterH

#include <SDL.h>

//---------------------------------------------------------------------------
// ������ ��������� �� �������� ������. ����� (�������������� ������ ��������)
// ������� �� ������� letters.gif, ����� - �� numbers-yg.gif
//---------------------------------------------------------------------------
class TScreenPrinter
{
    private:
        SDL_Surface *letters;
        SDL_Surface *numbers;
        SDL_Surface *whitenumbers;

    public:
        TScreenPrinter();
        ~TScreenPrinter();
        void Print(int x, int y, char *message, SDL_Surface *surface);
        void Print(int x, int y, char *message);     // ���������� ���������
        void PrintCAligned(int y, char *message);
        void PrintCAligned(int y, char *message, SDL_Surface *surface);
        int CharW() { return numbers->w / 10; }      // �������� ������ �������
        int CharH() { return numbers->h; }           // �������� ������ �������
};
//---------------------------------------------------------------------------
TScreenPrinter& ScreenPrinter()                      
{
    static TScreenPrinter printer;

    return printer;
}
//---------------------------------------------------------------------------
#endif

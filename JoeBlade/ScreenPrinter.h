//---------------------------------------------------------------------------

#ifndef ScreenPrinterH
#define ScreenPrinterH

#include <SDL.h>

//---------------------------------------------------------------------------
// Печать сообщений на основном экране. Буквы (поддерживаются ТОЛЬКО строчные)
// берутся из ресурса letters.gif, цифры - из numbers-yg.gif
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
        void Print(int x, int y, char *message);     // напечатать сообщение
        void PrintCAligned(int y, char *message);
        void PrintCAligned(int y, char *message, SDL_Surface *surface);
        int CharW() { return numbers->w / 10; }      // получить ширину символа
        int CharH() { return numbers->h; }           // получить высоту символа
};
//---------------------------------------------------------------------------
TScreenPrinter& ScreenPrinter()                      
{
    static TScreenPrinter printer;

    return printer;
}
//---------------------------------------------------------------------------
#endif

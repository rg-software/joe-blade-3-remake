//---------------------------------------------------------------------------

#ifndef MessagePanelH
#define MessagePanelH

#include <sdl.h>
#include <list>
#include <vcl.h>

using namespace std;
//---------------------------------------------------------------------------
// панель сообщений (отображает происходящие события)
// расположена на высоте Config::MessagePanelY
// ширина не ограничена
//---------------------------------------------------------------------------
class TMessagePanel
{
    list<AnsiString> messages;
    SDL_Surface *panel;
    int lastchanged;
    int MsgY;
    SDL_TimerID timer;
    CRITICAL_SECTION MessageCS;

    
    public:
        TMessagePanel();
        ~TMessagePanel() { SDL_FreeSurface(panel); SDL_RemoveTimer(timer); }

        int LastChanged() { return lastchanged; }
        void EnqueueMessage(const char *message);
        const char* GetTopMessage() const { return messages.begin()->c_str(); }
        const char* GetBottomMessage() const { return messages.back().c_str(); }
        void Update();
        void Show();
};
//---------------------------------------------------------------------------
TMessagePanel& MessagePanel();
//---------------------------------------------------------------------------
#endif

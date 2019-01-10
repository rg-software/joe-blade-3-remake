//---------------------------------------------------------------------------
#ifndef StatusPanelH
#define StatusPanelH

#include <SDL.h>
#include <map>
#include <utility>

using namespace std;

//---------------------------------------------------------------------------
class TMyStatusPanel
{
	SDL_Surface *numbers;
	SDL_Surface *bullets;
	SDL_Surface *hearts;
	map<int, pair<int, int> > coords;

	void PrintNumber(int number, int x, int y, int width);

	public:
		TMyStatusPanel();
		void Show();
		~TMyStatusPanel()
		{
			SDL_FreeSurface(numbers);
			SDL_FreeSurface(bullets);
            SDL_FreeSurface(hearts);
        }
};
//---------------------------------------------------------------------------
TMyStatusPanel& StatusPanel();
//---------------------------------------------------------------------------
#endif

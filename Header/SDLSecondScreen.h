#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


namespace Screen2
{

	class ScreenSecond
	{
	private:
		SDL_Window *w;
		SDL_Event E1;
		SDL_Renderer *TempRender;
		SDL_Texture * newTexture;
		SDL_Surface * newSurface;
		TTF_Font * Font;
	public:
		int Gamestate;
		ScreenSecond();
		void init();
		void Gameloop();
		void quit();
	};


}

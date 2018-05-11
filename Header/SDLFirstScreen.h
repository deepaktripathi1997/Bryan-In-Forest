#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace Screen1 
{

	class Screen
	{
	private :
		SDL_Window * window;
		SDL_Event e;
		SDL_Renderer * R1;
		SDL_Texture * T1, *T2;
		SDL_Surface * S1, *S2;
		TTF_Font * Font;
		int mousex, mousey;
		int Texturex, Texturey;
	public :
		Screen();
		void init();
		void Gameloop();
		void loadSurface();
		void loadTexture();
		~Screen();
	};

}
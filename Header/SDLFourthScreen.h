
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

namespace Screen4
{

	class Level2
	{
	private:
		SDL_Window *window;
		SDL_Event e;
		SDL_Renderer * R1;
		SDL_Texture * T[20];
		SDL_Surface * S[20];
		SDL_Color fgcol[10];
		TTF_Font * Font;
		SDL_Rect SrcBackSurface[20];
		SDL_Rect DestFontSurface[20];
		Mix_Music * music[20];
		Mix_Chunk * chunk[20];

		//In memory text stream
		std::stringstream timeText;

		bool running, spacebarpressed;
		int mousex, mousey;
		int windowx, windowy;
		int window1id, window2id;
		int Texturex[20], Texturey[20];
		int SpriteForwardCount;
		int framewidth, frameheight;
		int jumpheight;
		int Zoombie1width;
		int Zoombieheight;
		int ZoombieCount;
		SDL_Color colText;
		SDL_Color timecolor;
		bool Scrolling;
		char ch;
		int count;
		int SpriteBackwardCount;
		std::stringstream gameresult;
		Uint32 storeresult;
	public:
		int LevelResult;
		Level2();
		void init();
		Uint32 getgameresult();
		//	void initMusic();
		void Gameloop();
		void loadSurface();
		void loadTexture();
		bool loadmusic();
		void loadRect();
		//bool checkcollision(SDL_Rect ,SDL_Rect);
		//	void loadtext(SDL_Color );
		int GetGameState();
		void gameover();
		bool quit();
	};

	//The application time based timer
	class LTimer
	{
	public:
		//Initializes variables
		LTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

	private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
	};


}
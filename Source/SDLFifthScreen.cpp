#include "SDLFifthScreen.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <sstream>

using namespace Screen5;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_TICKS_PER_FRAME = 1000 / 60;
const int velsprite = 10;
const int velzoombie = 2;
const int velscroll = 5;

LTimer::LTimer()
{
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void LTimer::start()
{
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
	//If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	//If the timer is running and paused
	if (mStarted && mPaused)
	{
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (mStarted)
	{
		//If the timer is paused
		if (mPaused)
		{
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
	return mPaused && mStarted;
}

LTimer timer;


Level3::Level3()
{
	window = NULL;
	R1 = NULL;
	for (int i = 0; i < 20; i++)
	{
		S[i] = NULL;
		T[i] = NULL;
		Texturex[i] = 0;
		Texturey[i] = 0;
		music[i] = NULL;
		chunk[i] = NULL;
		SrcBackSurface[i] = { 0,0,0,0 };
		DestFontSurface[i] = { 0,0,0,0 };
	}


	running = false;
	mousex = mousey = 0;
	LevelResult = 0;
}


void Level3::init() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << SDL_GetError();
	}
	else
	{
		window = SDL_CreateWindow(" Level3 ", 0, 0, 1366, 768, SDL_WINDOW_MAXIMIZED);
		window1id = SDL_GetWindowID(window);
	}

	if (IMG_Init(IMG_INIT_JPG || IMG_INIT_PNG) < 0)
	{
		std::cout << IMG_GetError();
	}

	if (TTF_Init() < 0)
	{
		TTF_GetError();
	}
	else
	{
		Font = TTF_OpenFont("Kreepshow Frigid.ttf", 50);
	}

	R1 = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (R1 == NULL)
	{
		std::cout << SDL_GetError();
	}

	if (Mix_Init(MIX_INIT_MP3 || MIX_INIT_OGG) < 0)
	{
		std::cout << Mix_GetError();
	}
	else
	{
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 8192) < 0)
		{
			std::cout << Mix_GetError();
		}
	}

	loadSurface();
}

void Level3::loadSurface()
{
	S[0] = IMG_Load("Level3.jpg");

	if (S[0] == NULL)
	{
		std::cout << SDL_GetError();
	}

	S[1] = IMG_Load("Spriteforward.png");
	S[2] = IMG_Load("Spritebackward.png");
	S[3] = IMG_Load("Zoombie1.png");

	for (int i = 0; i < 4; i++)
	{
		if (S[i] == NULL)
		{
			std::cout << SDL_GetError();
		}
	}

	loadTexture();
}


void Level3::loadTexture()
{
	T[0] = SDL_CreateTextureFromSurface(R1, S[0]);
	T[1] = SDL_CreateTextureFromSurface(R1, S[1]);
	T[2] = SDL_CreateTextureFromSurface(R1, S[2]);
	T[3] = SDL_CreateTextureFromSurface(R1, S[3]);
	T[4] = SDL_CreateTextureFromSurface(R1, IMG_Load("Lion.png"));

	for (int i = 0; i < 5; i++)
	{
		if (T[i] == NULL)
		{
			std::cout << SDL_GetError();
		}
	}

	SDL_QueryTexture(T[0], NULL, NULL, &Texturex[0], &Texturey[0]);
	SDL_QueryTexture(T[1], NULL, NULL, &Texturex[1], &Texturey[1]);
	SDL_QueryTexture(T[2], NULL, NULL, &Texturex[2], &Texturey[2]);
	SDL_QueryTexture(T[3], NULL, NULL, &Texturex[3], &Texturey[3]);

	SDL_GetWindowSize(window, &windowx, &windowy);

	Gameloop();
}

void Level3::Gameloop()
{
	//In memory text stream
	std::stringstream timeText;

	SDL_Color colText = { 28,75,75 };

	int count = 0;

	char ch = 'r';
	bool Scrolling = false;
	int weahercount = 1;

	if (loadmusic() == false)
	{
		std::cout << Mix_GetError();
	}

	//Background on Window
	DestFontSurface[0].x = 0;
	DestFontSurface[0].y = 100;
	DestFontSurface[0].w = windowx;
	DestFontSurface[0].h = 544;

	//Background's Clippimg
	SrcBackSurface[0].x = 0;
	SrcBackSurface[0].y = 0;
	SrcBackSurface[0].w = 400;
	SrcBackSurface[0].h = 800;

	//Sprite's Forward

	int SpriteForwardCount = 1;

	Uint32  PlayerInSPace = 1000;

	int framewidth = Texturex[1] / 8;
	int frameheight = Texturey[1];

	bool spacebarpressed = false;
	bool running1 = false;

	SrcBackSurface[1].x = 0;
	SrcBackSurface[1].y = 0;
	SrcBackSurface[1].w = framewidth;
	SrcBackSurface[1].h = frameheight;

	DestFontSurface[1].x = 20;
	DestFontSurface[1].y = 450;
	DestFontSurface[1].w = framewidth / 3;
	DestFontSurface[1].h = frameheight / 2;

	int jumpheight = 100;

	//Sprite's Backward
	int SpriteBackwardCount = 1;

	SrcBackSurface[2].x = Texturex[2] - framewidth;
	SrcBackSurface[2].y = 0;
	SrcBackSurface[2].w = framewidth;
	SrcBackSurface[2].h = frameheight;

	DestFontSurface[2].x = 20;
	DestFontSurface[2].y = 440;
	DestFontSurface[2].w = framewidth / 2;
	DestFontSurface[2].h = frameheight / 2;

	//Time Rectangle
	DestFontSurface[3].x = 700;
	DestFontSurface[3].y = 100;
	DestFontSurface[3].w = 300;
	DestFontSurface[3].h = 100;


	int Zoombie1width = Texturex[3] / 6;
	int Zoombieheight = Texturey[3];
	int ZoombieCount = 1;

	//Zoombie Clipping
	SrcBackSurface[3].x = 0;
	SrcBackSurface[3].y = 0;
	SrcBackSurface[3].w = Zoombie1width;
	SrcBackSurface[3].h = Zoombieheight;


	//Zoombie's Position on window
	DestFontSurface[4].x = windowx + 10;
	DestFontSurface[4].y = 450;
	DestFontSurface[4].w = framewidth / 2;
	DestFontSurface[4].h = frameheight / 2;


	//Player's Power
	//DestFontSurface[6].x = ;
	//DestFontSurface[6].y = 450;
	//DestFontSurface[6].w = framewidth / 2;
	//DestFontSurface[6].h = frameheight / 2;

	//Lion's Zoombie


	Mix_PlayMusic(music[0], 1);

	while (!running)
	{

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				LevelResult = 1;
				if (Mix_PlayingMusic() == 1)
					Mix_PauseMusic();
				running = true;
			}

			//Scrolling effect
			if (SrcBackSurface[0].x + SrcBackSurface[0].w < Texturex[0] && e.key.keysym.sym == SDLK_RIGHT)
			{


				if (DestFontSurface[1].x > SrcBackSurface[0].w / 2)
				{
					SrcBackSurface[0].x += velscroll;
					Scrolling = true;
				}

			}


			//Checks the collision with the end walls
			if (DestFontSurface[1].x > windowx - 100)
			{
				DestFontSurface[1].x = windowx - 100;
				LevelResult = 1;

				running = true;
			}


			if (e.type == SDL_KEYDOWN)
			{
				if (spacebarpressed == false && timer.getTicks() == 0) {

					//Move the Sprite forward
					switch (e.key.keysym.sym)
					{
					case SDLK_RIGHT:
						ch = 'r';
						if (spacebarpressed == false)
						{

							if (SpriteForwardCount < 9)
							{
								SrcBackSurface[1].x += framewidth;

								if (Scrolling == false)
									DestFontSurface[1].x += velsprite;
								if (Scrolling == true)
									DestFontSurface[1].x += velsprite - velscroll;

								SpriteForwardCount++;
							}

							if (SpriteForwardCount == 9)
							{
								SpriteForwardCount = 1;
								SrcBackSurface[1].x = 0;
							}

							SpriteBackwardCount = 9 - SpriteForwardCount;
						}

						Mix_PlayChannel(2, chunk[0], 1);

						break;

						//Move the Sprite backward
					case SDLK_LEFT:

						ch = 'l';
						if (SrcBackSurface[2].x == 0)
						{
							SrcBackSurface[2].x = Texturex[2] - framewidth;
						}

						if (SpriteBackwardCount < 9 && SrcBackSurface[2].x != 0)
						{
							SrcBackSurface[2].x -= framewidth;

							if (Scrolling == false)
								DestFontSurface[1].x -= velsprite;
							if (Scrolling == true)
								DestFontSurface[1].x -= velsprite - velscroll;

							SpriteBackwardCount++;
						}
						else if (SpriteBackwardCount == 9)
						{
							SrcBackSurface[2].x = (Texturex[2] - framewidth);
							SpriteBackwardCount = 0;
						}

						SpriteForwardCount = 9 - SpriteBackwardCount;
						break;
						Mix_PlayChannel(2, chunk[0], 1);


					}//end of switch
				}

				if (e.key.keysym.sym == SDLK_SPACE && spacebarpressed == false)

				{
					spacebarpressed = true;
					timer.start();
				}






			}//end of if keyboard events

			 //Checks the collision of sprite with the left wall
			if (DestFontSurface[1].x < 10)
				DestFontSurface[1].x = 10;



		}//while 2 ends



		SDL_RenderClear(R1);

		SDL_RenderCopy(R1, T[0], &SrcBackSurface[0], &DestFontSurface[0]);

		if (count == 1)
			SDL_RenderFillRect(R1, &DestFontSurface[3]);

		if (ch == 'r')
		{
			if (spacebarpressed == true && timer.getTicks() < 500)
			{

				if (timer.getTicks() < 250 && DestFontSurface[1].y > jumpheight)
				{
					DestFontSurface[1].x += 70;
					DestFontSurface[1].y -= 200;
					SDL_Delay(30);
				}

				if (timer.getTicks() > 250 && DestFontSurface[1].y < jumpheight + 350)
				{
					DestFontSurface[1].x += 70;
					DestFontSurface[1].y += 200;
					SDL_Delay(20);
				}
			}

			else
			{
				spacebarpressed = false;
				DestFontSurface[1].y = jumpheight + 350;
				timer.stop();
			}

			SDL_RenderCopy(R1, T[1], &SrcBackSurface[1], &DestFontSurface[1]);
		}



		if (ch == 'l')
		{
			if (spacebarpressed == true && timer.getTicks() < 500)
			{

				if (timer.getTicks() < 250 && DestFontSurface[1].y > jumpheight)
				{
					DestFontSurface[1].x -= 70;
					DestFontSurface[1].y -= 200;
					SDL_Delay(30);
				}

				if (timer.getTicks() > 250 && DestFontSurface[1].y < jumpheight + 350)
				{
					DestFontSurface[1].x -= 70;
					DestFontSurface[1].y += 200;
					SDL_Delay(10);
				}

			}
			else
			{
				spacebarpressed = false;
				DestFontSurface[1].y = jumpheight + 350;
				timer.stop();
			}
			SDL_RenderCopy(R1, T[2], &SrcBackSurface[2], &DestFontSurface[1]);
		}



		if (ZoombieCount > 6)
		{
			ZoombieCount = 1;
			SrcBackSurface[3].x = 0;
		}
		if (DestFontSurface[4].x < 20)
		{
			DestFontSurface[4].x = windowx + 10;
		}

		if (ZoombieCount < 7)
		{

			if (Scrolling == false)
				DestFontSurface[4].x -= (velzoombie+25);
			if (Scrolling == true)
				DestFontSurface[4].x -= (velzoombie + 28);


			SrcBackSurface[3].x += Zoombie1width;

			SDL_RenderCopy(R1, T[3], &SrcBackSurface[3], &DestFontSurface[4]);
			SDL_Delay(100);
			ZoombieCount++;
		}






		if (SDL_IntersectRect(&DestFontSurface[1], &DestFontSurface[4], &DestFontSurface[5]) == SDL_TRUE)
		{



			count++;

			if (count < 4)
			{
				DestFontSurface[4].x = windowx + 20;
			}
			if (count == 4)
			{
				LevelResult = 0;

				if (Mix_PlayingMusic() == 1)
				{
					Mix_PauseMusic();
				}

				running = true;
			}
		}


		SDL_RenderPresent(R1);

	}//end of game loop


}

Uint32 Level3::getgameresult()
{
	if (storeresult != 0)
		return storeresult;
	return 0;
}


int Level3::GetGameState() {
	return LevelResult;
}



bool Level3::quit()
{
	for (int i = 0; i < 20; i++)
	{
		T[i] = NULL;
		S[i] = NULL;


		SDL_FreeSurface(S[i]);
		SDL_DestroyTexture(T[i]);
	}

	SDL_DestroyRenderer(R1);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return true;
}


Level3 Screen5::Level3::Constructor(Level3  &S2)
{
	S2.window = window;
	S2.R1 = R1;
	for (int i = 0; i < 20; i++)
	{
		S2.S[i] = S[i];
		S2.T[i] = T[i];
		S2.Texturex[i] = Texturex[i];
		S2.Texturey[i] = Texturey[i];
		S2.music[i] = music[i];
		S2.chunk[i] = chunk[i];
		S2.e = e;
		S2.SrcBackSurface[i] = SrcBackSurface[i];
		S2.DestFontSurface[i] = DestFontSurface[i];
	}

	S2.running = false;
	S2.mousex = 0;
	S2.mousey = 0;
	S2.LevelResult = LevelResult;
	return S2;
}



bool Level3::gameover(int check)
{
	bool runninggameover = false;
	bool result = false;

	if (check == 1)
	{
		int windowwidth = 1050;
		int windowheight = 590;

		SDL_SetWindowSize(window, windowwidth, windowheight);
		SDL_Texture * temp = SDL_CreateTextureFromSurface(R1, IMG_Load("Gameover.jpg"));
		SDL_Texture * temp1 = SDL_CreateTextureFromSurface(R1, IMG_Load("Resume.jpg"));
		SDL_Texture * temp2 = SDL_CreateTextureFromSurface(R1, IMG_Load("Quit.jpg"));
		int textureresumex, textureresumey, texturequitx, texturequity;
		SDL_QueryTexture(temp1, NULL, NULL, &textureresumex, &textureresumey);
		SDL_QueryTexture(temp2, NULL, NULL, &texturequitx, &texturequity);
		SDL_Rect Resume, Quit;
		Resume.x = windowwidth / 3;
		Resume.y = windowheight - 200;
		Resume.w = 170;
		Resume.h = 54;

		Quit.x = Resume.x + Resume.w + 50;
		Quit.y = Resume.y;
		Quit.w = 155;
		Quit.h = 46;



		SDL_Event e;

		while (!runninggameover)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				int mousex, mousey;

				if (e.type == SDL_QUIT)
				{
					runninggameover = true;
				}

				if (e.type == SDL_MOUSEMOTION)
				{
					mousex = e.button.x;
					mousey = e.button.y;

					if (mousex > Resume.x && mousey > Resume.y && mousex < Resume.x + 220 && mousey < Resume.y + 104)
					{
						Resume.w = 220;
						Resume.h = 104;
					}
					else
					{
						Resume.w = 155;
						Resume.h = 54;

					}
					if (mousex > Quit.x && mousey > Quit.y && mousex < Quit.x + 205 && mousey < Quit.y + 96)
					{
						Quit.w = 205;
						Quit.h = 96;
					}
					else
					{
						Quit.w = 155;
						Quit.h = 46;
					}

				}


				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					SDL_GetMouseState(&mousex, &mousey);

					if (e.button.button == SDL_BUTTON_LEFT && (e.button.clicks == 1 || e.button.clicks == 2))
					{
						if (mousex > Resume.x && mousey > Resume.y && mousex < Resume.x + 220 && mousey < Resume.y + 104)
						{
							Resume.w = 220;
							Resume.h = 104;
							runninggameover = true;
							result = true;
						}

						if (mousex > Quit.x && mousey > Quit.y && mousex < (Quit.x + 205) && mousey < Quit.y + 96)
						{
							Quit.w = 205;
							Quit.h = 96;
							runninggameover = true;
							result = false;
						}

					}

				}



			}

			SDL_RenderClear(R1);

			SDL_RenderCopy(R1, temp, NULL, NULL);

			SDL_RenderCopy(R1, temp1, NULL, &Resume);

			SDL_RenderCopy(R1, temp2, NULL, &Quit);

			SDL_RenderPresent(R1);

		}


		return result;
	}

	return result;
}




bool Level3::loadmusic()
{
	chunk[0] = Mix_LoadWAV("footsteps1.wav");
	music[0] = Mix_LoadMUS("musLevel3.mp3");
	return true;
}
















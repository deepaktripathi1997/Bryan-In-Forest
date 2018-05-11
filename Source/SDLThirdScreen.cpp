#include "SDLThirdScreen.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <random>
#include <conio.h>

using namespace Screen3;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_TICKS_PER_FRAME = 1000 / 60;
const int velsprite = 4;
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


Level1::Level1()
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


void Level1::init() {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << SDL_GetError();
	}
	else
	{
		window = SDL_CreateWindow(" Level1 ", 0, 0, 1366, 768,SDL_WINDOW_MAXIMIZED);
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

	Gameloop();
}

void Level1::loadSurface()
{
	S[0] = IMG_Load("Level1.jpg");

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

}


void Level1::loadTexture()
{
	T[0] = SDL_CreateTextureFromSurface(R1, S[0]);
	T[1] = SDL_CreateTextureFromSurface(R1, S[1]);
	T[2] = SDL_CreateTextureFromSurface(R1, S[2]);
	T[3] = SDL_CreateTextureFromSurface(R1, S[3]);

	for (int i = 0; i < 4; i++)
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

}

void Level1::loadRect()
{
	colText = { 28,75,75 };

	std::random_device rd;   // non-deterministic generator  
	std::mt19937 gen(rd());  // to seed mersenne twister.  
	std::uniform_int_distribution<> dist(1, 255); // distribute results between 1 and 255 inclusive.  


	count = 0;

	ch = 'r';
	Scrolling = false;

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
	SrcBackSurface[0].w = 350;
	SrcBackSurface[0].h = 544;

	//Sprite's Forward

	SpriteForwardCount = 1;

	framewidth = Texturex[1] / 8;
	frameheight = Texturey[1];

	spacebarpressed = false;
	//	bool running1 = false;

	SrcBackSurface[1].x = 0;
	SrcBackSurface[1].y = 0;
	SrcBackSurface[1].w = framewidth;
	SrcBackSurface[1].h = frameheight;

	DestFontSurface[1].x = 20;
	DestFontSurface[1].y = 450;
	DestFontSurface[1].w = framewidth / 3;
	DestFontSurface[1].h = frameheight / 2;

	jumpheight = 100;

	//Sprite's Backward
	SpriteBackwardCount = 1;

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
	DestFontSurface[3].y = 0;
	DestFontSurface[3].w = 500;
	DestFontSurface[3].h = 100;


	Zoombie1width = Texturex[3] / 6;
	Zoombieheight = Texturey[3];
	ZoombieCount = 1;

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

	DestFontSurface[7].x = windowx;
	DestFontSurface[7].y = 450;
	DestFontSurface[7].w = framewidth / 2;
	DestFontSurface[7].h = frameheight / 2;


	//Player's Power
	DestFontSurface[6].x = 100;
	DestFontSurface[6].y = 50;
	DestFontSurface[6].w = 200;
	DestFontSurface[6].h = 50;

	DestFontSurface[8].x = 100;
	DestFontSurface[8].y = 30;
	DestFontSurface[8].w = 405;
	DestFontSurface[8].h = 55;

	DestFontSurface[9].x = 100;
	DestFontSurface[9].y = 30;
	DestFontSurface[9].w = 300;
	DestFontSurface[9].h = 55;

	DestFontSurface[10].x = 100;
	DestFontSurface[10].y = 30;
	DestFontSurface[10].w = 200;
	DestFontSurface[10].h = 55;

	DestFontSurface[11].x = 100;
	DestFontSurface[11].y = 30;
	DestFontSurface[11].w = 100;
	DestFontSurface[11].h = 55;

}



void Level1::Gameloop()
{
	std::random_device rd;   // non-deterministic generator  
	std::mt19937 gen(rd());  // to seed mersenne twister.  
	std::uniform_int_distribution<> dist(1, 255); // distribute results between 1 and 255 inclusive.  

	loadSurface();
	loadTexture();
	loadRect();

	Mix_PlayMusic(music[0],3);

	LTimer time1;
	time1.start();

	while (!running)
	{

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				LevelResult = 0;
				running = true;
			}
	
			//Scrolling effect
			if (SrcBackSurface[0].x + SrcBackSurface[0].w < Texturex[0] && e.key.keysym.sym == SDLK_RIGHT)
			{
			

				if (DestFontSurface[1].x > SrcBackSurface[0].w / 4)
				{
					SrcBackSurface[0].x += velsprite;
					Scrolling = true;
				}
			
			}
			else
			{
				Scrolling = false;
			}
			
			
		
			//Checks the collision with the end walls
			if (Scrolling == false)
			{
				if (DestFontSurface[1].x > windowx - 50)
				{
					DestFontSurface[1].x = windowx - 50;

					LevelResult = 1;

					running = true;
				}
			}
			



			if (e.type == SDL_KEYDOWN)
			{
				Mix_PlayChannel(2,chunk[0], 1);
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
									DestFontSurface[1].x += velsprite;

								SpriteForwardCount++;
							}

							if (SpriteForwardCount == 9)
							{
								SpriteForwardCount = 1;
								SrcBackSurface[1].x = 0;
							}

							SpriteBackwardCount = 9 - SpriteForwardCount;
						}

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
								DestFontSurface[1].x -= velsprite;

							SpriteBackwardCount++;
						}
						else if (SpriteBackwardCount == 9)
						{
							SrcBackSurface[2].x = (Texturex[2] - framewidth);
							SpriteBackwardCount = 0;
						}

						SpriteForwardCount = 9 - SpriteBackwardCount;
						
						
						
						break;

					default:
							 break;
					}//end of switch
					Mix_Pause(2);
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

		switch (count)
		{
		
		

		default: break;
		}
		if (ch == 'r')
		{
			if (spacebarpressed == true && timer.getTicks() < 500)
			{

				if (timer.getTicks() < 250 && DestFontSurface[1].y > jumpheight)
				{
					DestFontSurface[1].x += 40;
					DestFontSurface[1].y -= 200;
					SDL_Delay(30);
				}

				if (timer.getTicks() > 250 && DestFontSurface[1].y < jumpheight + 350)
				{
					DestFontSurface[1].x += 40;
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
					DestFontSurface[1].x -= 40;
					DestFontSurface[1].y -= 200;
					SDL_Delay(30);
				}

				if (timer.getTicks() > 250 && DestFontSurface[1].y < jumpheight + 350)
				{
					DestFontSurface[1].x -= 40;
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
				std::uniform_int_distribution<> distZommbie(windowx - 50,windowx + 50);
				
				DestFontSurface[4].x = distZommbie(gen);

			}

			if (ZoombieCount < 7) 
			{
				std::uniform_int_distribution<> velZoombie(5,20);
				if (Scrolling == false)
					DestFontSurface[4].x -= velZoombie(gen);
				if (Scrolling == true)
					DestFontSurface[4].x -= velZoombie(gen);


				SrcBackSurface[3].x += Zoombie1width;

				SDL_RenderCopy(R1, T[3], &SrcBackSurface[3], &DestFontSurface[4]);

				SDL_Delay(100);
				ZoombieCount++;
			}

		  
			timeText.str("");
			timeText << "Score : "<<(time1.getTicks()/1000.f)<<"s";

			timecolor.r = dist(gen);
			timecolor.b = dist(gen);
			timecolor.g = dist(gen);


			SDL_RenderCopy(R1, SDL_CreateTextureFromSurface(R1, TTF_RenderText_Solid(Font, timeText.str().c_str(),timecolor)), NULL,&DestFontSurface[3]);

			switch (count)
			{
			case 0: SDL_RenderCopy(R1,SDL_CreateTextureFromSurface(R1, IMG_Load("fullHealth.png")), NULL, &DestFontSurface[8]);
				break;
			case 1: SDL_RenderCopy(R1, SDL_CreateTextureFromSurface(R1, IMG_Load("health75.png")), NULL, &DestFontSurface[9]);
				break;
			case 2:	SDL_RenderCopy(R1, SDL_CreateTextureFromSurface(R1, IMG_Load("health50.png")), NULL, &DestFontSurface[10]);
				break;
			case 3:	SDL_RenderCopy(R1, SDL_CreateTextureFromSurface(R1, IMG_Load("health25.png")), NULL, &DestFontSurface[11]);
				break;
			default: break;
			}


		if( SDL_IntersectRect(&DestFontSurface[1], &DestFontSurface[4],&DestFontSurface[5]) == SDL_TRUE)
		{
				count++;

				if (count < 4)
				{
					std::uniform_int_distribution<> distZommbie(windowx/2, windowx);
					DestFontSurface[4].x = distZommbie(gen);

				}
				if (count == 4)
				{
					LevelResult = 2;
					
					if (Mix_PlayingMusic() == 1)
					{
						Mix_PauseMusic();
					}

					running = true;
				}
			}


		SDL_RenderPresent(R1);

	}//end of game loop

	storeresult = time1.getTicks();
	time1.stop();
	if (LevelResult == 2)
		gameover();
	else
		quit();

}


int Level1::GetGameState() {
	return LevelResult;
}



bool Level1::quit()
{
	for (int i = 0; i < 20; i++)
	{
		T[i] = NULL;
		S[i] = NULL;


		SDL_FreeSurface(S[i]);
		SDL_DestroyTexture(T[i]);
	}
	Mix_HaltMusic();
	SDL_DestroyRenderer(R1);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return true;
}

Uint32 Level1::getgameresult()
{
	if(storeresult!=0)
	return storeresult;
	return 0;
}

void Level1::gameover()
{
	bool runninggameover = false;


		int windowwidth  = 1050;
		int windowheight = 590;

		SDL_SetWindowSize(window,windowwidth,windowheight);
		SDL_Texture * temp = SDL_CreateTextureFromSurface(R1, IMG_Load("Gameover.jpg"));
		SDL_Texture * temp1 = SDL_CreateTextureFromSurface(R1, IMG_Load("PlayAgain.jpg"));
		SDL_Texture * temp2 = SDL_CreateTextureFromSurface(R1, IMG_Load("Quit.jpg"));
		int textureresumex, textureresumey, texturequitx, texturequity;
		SDL_QueryTexture(temp1, NULL, NULL, &textureresumex, &textureresumey);
		SDL_QueryTexture(temp2, NULL, NULL, &texturequitx, &texturequity);
		SDL_Rect Resume, Quit;
		Resume.x = windowwidth/3;
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
					LevelResult = 0;
					runninggameover = true;

				}

				if (e.type == SDL_MOUSEMOTION)
				{
					mousex = e.button.x;
					mousey = e.button.y;

					if (mousex > Resume.x && mousey > Resume.y && mousex < Resume.x+220 && mousey < Resume.y + 104)
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
						if (mousex > Resume.x && mousey > Resume.y && mousex < Resume.x + 220 && mousey < Resume.y + 104 )
						{
							Resume.w = 220;
							Resume.h = 104;
							LevelResult = 0;
							runninggameover = true;
						}

						if (mousex > Quit.x && mousey > Quit.y && mousex < (Quit.x + 205) && mousey < Quit.y + 96)
						{
							Quit.w = 205;
							Quit.h = 96;
							LevelResult = 0;
							runninggameover = true;
						}

					}

				}



			}

			SDL_RenderClear(R1);
			
			SDL_RenderCopy(R1, temp, NULL, NULL);
			
			SDL_RenderCopy(R1, temp1, NULL,&Resume);
			
			SDL_RenderCopy(R1, temp2, NULL,&Quit);
			
			SDL_RenderPresent(R1);

		}

	quit();

}

bool Level1::loadmusic()
{
	chunk[0] = Mix_LoadWAV("Windows Navigation Start.wav");
	music[0] = Mix_LoadMUS("muslevel1.mp3");
	return true;
}


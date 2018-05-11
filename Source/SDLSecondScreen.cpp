
#include "SDLSecondScreen.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <sstream>
#include <random>
using namespace Screen2;

struct Biography
{
	std::string FirstName;
	std::string LastName;
	Uint32 Score;
	Biography()
	{
		FirstName = "";
		LastName = "";
		Score = 0;
	}

};



ScreenSecond::ScreenSecond()
{
	w = NULL;
	TempRender = NULL;
	newTexture = NULL;
	newSurface = NULL;
	Font = NULL;
	Gamestate = 0;
}

void ScreenSecond::init() {

	w = SDL_CreateWindow("Information", 0, 0, 900, 700, SDL_WINDOW_MAXIMIZED);

	if (w == NULL)
	{
		std::cout<<SDL_GetError();
	}

	TempRender = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);

	if (TempRender == NULL)
	{
		std::cout << SDL_GetError();
	}

	newSurface = IMG_Load("HomeScreen.jpg");

	if (newSurface == NULL)
	{
		std::cout << SDL_GetError();
	}

	newTexture = SDL_CreateTextureFromSurface(TempRender, newSurface);
	
	if (newTexture == NULL)
	{
		std::cout << SDL_GetError();
	}

	if (TTF_Init() == 1)
	{
		std::cout << TTF_GetError();
	}
	
	Font = TTF_OpenFont("Kreepshow Frigid.ttf", 50);

	if (Font == NULL)
	{
		std::cout << TTF_GetError();
	}


	Gameloop();
}


void ScreenSecond::Gameloop() {
	
	std::stringstream text ;
	text.str("");
	
	int count = 1;
	Biography B1;

	int x, y;
	SDL_GetWindowSize(w, &x, &y);

	SDL_Rect Rect[7];
	Rect[0].x = 150;
	Rect[0].y = 50;
	Rect[0].w = 600;
	Rect[0].h = 300;

	//First Name
	Rect[1].x = 50;
	Rect[1].y = 350;
	Rect[1].w = 300;
	Rect[1].h = 50;

	//Last Name
	Rect[2].x = 50;
	Rect[2].y = 420;
	Rect[2].w = 300;
	Rect[2].h = 50;


	//Enter the first Name
	Rect[3].x = 400;
	Rect[3].y = 350;
	Rect[3].w = 300;
	Rect[3].h = 50;

	//Enter the Last name
	Rect[4].x = 400;
	Rect[4].y = 420;
	Rect[4].w = 300;
	Rect[4].h = 50;

	//Help Screen
	Rect[5].x = 900;
	Rect[5].y = 50;
	Rect[5].w = 300;
	Rect[5].h = 200;

	//tab button
	Rect[6].x = 1050;
	Rect[6].y = 50;
	Rect[6].w = 40;
	Rect[6].h = 50;

	std::random_device rd;   // non-deterministic generator  
	std::mt19937 gen(rd());  // to seed mersenne twister.  
	std::uniform_int_distribution<> dist(1, 255); // distribute results between 1 and 6 inclusive.  


	SDL_Color col;
	col.r = dist(gen);
	col.b = dist(gen);
	col.g = dist(gen);


	SDL_Texture * T1 = SDL_CreateTextureFromSurface(TempRender, TTF_RenderText_Solid(Font, " BIOGRAPHY ", col));
	SDL_Texture * T2 = SDL_CreateTextureFromSurface(TempRender, TTF_RenderText_Solid(Font, " First Name : ", col));
	SDL_Texture * T3 = SDL_CreateTextureFromSurface(TempRender, TTF_RenderText_Solid(Font, " Last Name : ", col));
	SDL_Texture * help = SDL_CreateTextureFromSurface(TempRender, IMG_Load("help.jpg"));

	SDL_SetTextureAlphaMod(help, 225);

	bool check = false;
	bool enterpressed = false;

	SDL_StartTextInput();
	while (!check)
	{
		while (SDL_PollEvent(&E1))
		{
			if (E1.type == SDL_QUIT)
			{
				check = true;
				Gamestate = 0;
			}
			if (E1.type == SDL_TEXTINPUT || E1.type == SDL_KEYDOWN)
			{
				if (count == 1)
				{
					if (E1.type == SDL_KEYDOWN && E1.key.keysym.sym == SDLK_BACKSPACE && B1.FirstName.length() > 0)
						B1.FirstName = B1.FirstName.substr(0, B1.FirstName.length() - 1);
					if (E1.type == SDL_KEYDOWN && E1.key.keysym.sym == SDLK_TAB)
						count = 2;
					else if (E1.type == SDL_TEXTINPUT)
						B1.FirstName += E1.text.text;
				}

				if (count == 2)
				{
					if (E1.type == SDL_KEYDOWN && E1.key.keysym.sym == SDLK_BACKSPACE && B1.FirstName.length() > 0)
						B1.LastName = B1.LastName.substr(0, B1.LastName.length() - 1);
					if (E1.type == SDL_KEYDOWN && E1.key.keysym.sym == SDLK_q)
						count = 3;
					else if (E1.type == SDL_TEXTINPUT)
						B1.LastName += E1.text.text;
				}

				if (count == 3)
				{
					std::fstream f1("Biography.txt", std::ios::app|std::ios::trunc);
					f1.write((char *)&B1, sizeof(B1));
					f1.close();
					check = true;
					Gamestate = 1;
				}
			
			}
			if (E1.type == SDL_KEYDOWN)
			{
				if (E1.key.keysym.sym == SDLK_TAB)
				{
					SDL_SetRenderDrawColor(TempRender, 134, 56, 187,200);
					SDL_RenderFillRect(TempRender, &Rect[6]);
				}


			}

		}
		SDL_RenderClear(TempRender);
		SDL_RenderCopy(TempRender, newTexture, NULL, NULL);
		SDL_RenderCopy(TempRender,T1, NULL, &Rect[0]);
		SDL_RenderCopy(TempRender, T2, NULL, &Rect[1]);
		SDL_RenderCopy(TempRender, T3, NULL, &Rect[2]);
		SDL_RenderCopy(TempRender, help, NULL, &Rect[5]);
		SDL_RenderCopy(TempRender,SDL_CreateTextureFromSurface(TempRender,TTF_RenderText_Solid(Font,B1.FirstName.c_str(),col)) , NULL, &Rect[3]);
		SDL_RenderCopy(TempRender, SDL_CreateTextureFromSurface(TempRender, TTF_RenderText_Solid(Font, B1.LastName.c_str(), col)), NULL, &Rect[4]);
		SDL_RenderPresent(TempRender);
	}
	SDL_StopTextInput();
}


void ScreenSecond::quit() {
	SDL_DestroyTexture(newTexture);
	SDL_FreeSurface(newSurface);
	SDL_DestroyRenderer(TempRender);
	SDL_DestroyWindow(w);


	newTexture = NULL;
	newSurface = NULL;
	TempRender = NULL;
	w = NULL;
	//Close
}

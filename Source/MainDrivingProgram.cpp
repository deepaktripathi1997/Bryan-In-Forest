
// if gamestate is 0 then it means exit the game 
// if gamestate is 1 then it means play the game
// if gamestate is 2 then it means load the game 
// if gamestate is 3 then it means open the game settings
// if levelstate is 0 then you haven't succeeded in comleting the level 
// if levelstate is 1 then you have comleted the particular level




#include "SDLFirstScreen.h"
#include "SDLSecondScreen.h"
#include "SDLThirdScreen.h"
#include "SDLFourthScreen.h"
#include "SDLFifthScreen.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>

Screen1::Screen * S1;
Screen2::ScreenSecond *S2;
Screen3::Level1 *S3;
Screen4::Level2 *S4;
Screen5::Level3 *S5;



int main(int argc, char ** argv)
{
	int checkstage = 1;
	while (checkstage > 0) {
		
		switch (checkstage)
		{
		case 1: S1 = new Screen1::Screen;
			    S1->init();
			switch (S1->gamestate)
			{
			case 0: checkstage = 0;
				break;
			case 1: checkstage = 2;
				break;
			default: checkstage = 0;
				break;
			}
			delete S1;
				break;
		case 2: S2 = new Screen2::ScreenSecond;
				S2->init();
			switch (S2->Gamestate)
			{
			case 0: checkstage = 1;
				break;
			case 1: checkstage = 3;
				break;
			default: checkstage = 0;
				break;
			}
			delete S2;
			break;
		case 3:S3 = new Screen3::Level1;
				S3->init();
			switch (S3->LevelResult)
			{
			case 0: checkstage = 1;
				break;
			case 1: checkstage = 4;
				break;
			case 2: checkstage = 3;
				break;
			default: checkstage = 0;
				break;
			}
			delete S3;
			break;
		case 4: S4 = new Screen4::Level2;
			S4->init();
			switch (S4->LevelResult)
			{
			case 0: checkstage = 1;
				break;
			case 1: checkstage = 5;
				break;
			case 2: checkstage = 4;
				break;
			default: checkstage = 0;
				break;
			}
			delete S4;
			break;
		case 5:S5 = new Screen5::Level3;
				S5->init();
			switch (S5->LevelResult)
			{
			case 0: checkstage = 1;
				break;
			case 1: checkstage = 6;
				break;
			case 2: checkstage = 5;
				break;
			default: checkstage = 0;
				break;
			}
			delete S5;
			break;



		default: checkstage = 0;
				 break;
		}

		

	}
	return 0;
}
#ifndef THESCREEN_H
#define THESCREEN_H
#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
class TheScreen{
	TheScreen();

	TheScreen(const TheScreen&);
	TheScreen* operator=(const TheScreen&);

	SDL_Window*window;
	SDL_Renderer*renderer;

public:	
	~TheScreen();
	//this is singleton
	static TheScreen&Instance();

	void CreateWindow();

	SDL_Renderer*GetRenderer();
	SDL_Window*GetWindow();
	SDL_Rect GetSize();
};
//to make life easier..
#define Screen TheScreen::Instance()
#endif//THESCREEN_H
#include"TheScreen.h"
TheScreen::TheScreen(){
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
}
TheScreen::~TheScreen(){
	SDL_Log("Destroyed SDLs");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	//SDL_Quit();
}
void TheScreen::CreateWindow(){
	window = SDL_CreateWindow("Mini Word Guessing Game",
		 SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
		 500,500,SDL_WINDOW_SHOWN|
		 //SDL_WINDOW_BORDERLESS|
		 //SDL_WINDOW_FULLSCREEN_DESKTOP );
			SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_PRESENTVSYNC);
	//SDL_SetWindowOpacity(window,0.75f);
}
SDL_Renderer*TheScreen::GetRenderer(){
	return renderer;
}
SDL_Window*TheScreen::GetWindow(){
	return window;
}
TheScreen&TheScreen::Instance(){
	static TheScreen instance;
	return instance;
}
SDL_Rect TheScreen::GetSize(){
	SDL_Rect rect;
	SDL_GetWindowSize(window, &rect.w,&rect.h);
	return rect;
}

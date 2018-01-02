#ifndef TEXTURE_H
#define TEXTURE_H
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"vec2.h"

SDL_Color Color(int r = 255, int g = 255, int b = 255, int a = 255);
SDL_Rect getRect(int x, int y, int w, int h);

#define WINDOW_W 1366
#define WINDOW_H 768
class Texture{
	SDL_Texture*texture;
	SDL_Rect sprite;
	int row,col;
	SDL_Renderer*renderer;
public:
	vec2 drawRatio;
	Texture(std::string path,SDL_Renderer*renderer,int row = 1,int col = 1,
		Size windowSize = Size(WINDOW_W,WINDOW_H));
	Texture(SDL_Surface*surface,SDL_Renderer*renderer,int row = 1,int col = 1,
		Size windowSize = Size(WINDOW_W,WINDOW_H));

	~Texture();
	void render(int x, int y, int w = -1, int h = -1,double angle = 0,int index = 0);
	//void render(vec2&pos,Size&size,double angle = 0,int index = 0);
	void render(vec2 pos,Size size,double angle = 0,int index = 0);

	void setColor(SDL_Color&color);
	void setColor(SDL_Color color);
	int getSpriteNum()const;
	SDL_Rect&getSprite();
	Size getSize();
	void setWindowSize(Size size);
};
#endif //TEXTURE_H

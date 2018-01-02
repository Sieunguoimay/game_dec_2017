#include"Texture.h"
SDL_Color Color(int r, int g, int b, int a){
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
}
SDL_Rect getRect(int x, int y, int w, int h){
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;	
}
Texture::Texture(std::string path,SDL_Renderer*renderer,int row, int col,Size windowSize)
	:renderer(renderer),col(col),row(row),drawRatio((float)windowSize.w/(float)WINDOW_W,(float)windowSize.h/(float)WINDOW_H){
	SDL_Surface*surface = IMG_Load(path.c_str());
	if (surface) {
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xff, 0x00, 0xff));//always pink
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		sprite.w = surface->w / col;
		sprite.h = surface->h / row;
		sprite.x = 0;
		sprite.y = 0;
		std::cout<<"Texture: loaded "<< path << std::endl;
	}
	else {
		std::cout << "Texture: failed to load texture " << path << std::endl;
	}
	SDL_FreeSurface(surface);
}

Texture::Texture(SDL_Surface*surface,SDL_Renderer*renderer,int col, int row,Size windowSize)
	:renderer(renderer),col(col),row(row),drawRatio((float)windowSize.w/(float)WINDOW_W,(float)windowSize.h/(float)WINDOW_H){
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		sprite.w = surface->w/col;
		sprite.h = surface->h/row;
		sprite.x = 0;
		sprite.y = 0;
}
Texture::~Texture(){
	SDL_DestroyTexture(texture);
	texture = NULL;
	renderer = NULL;
}
//use this function to render with the actual position - top-left
void Texture::render(int x, int y, int w, int h,double angle,int index){
	if(w==-1) w = sprite.w;
	if(h==-1) h = sprite.h;
	SDL_Rect rect = getRect(x*drawRatio.x,y*drawRatio.y,w*drawRatio.x,h*drawRatio.y);
	index%=getSpriteNum();//in case the input index is out of range
	sprite.x = sprite.w*(index%col);
	sprite.y = sprite.h*(index/col);
	SDL_RenderCopyEx(renderer,texture,&sprite,&rect,angle,NULL,SDL_FLIP_NONE);
}
/*
void Texture::render(vec2&pos,Size&size,double angle,int index){
	SDL_Rect rect = getRect(pos.x,pos.y,size.w,size.h);
	index%=getSpriteNum();//in case the input index is out of range
	sprite.x = sprite.w*(index%col);
	sprite.y = sprite.h*(index/col);
	SDL_RenderCopyEx(renderer,texture,&sprite,&rect,angle,NULL,SDL_FLIP_NONE);

}*/

//this function is to render the texture with it's center is position
//whenever using vec2-pos as position parameter
//in other case, use the above function to render with the actual position - top-left 
void Texture::render(vec2 pos,Size size,double angle,int index){
	SDL_Rect rect = getRect(
		(pos.x-size.w/2)*drawRatio.x,(pos.y-size.h/2)*drawRatio.y,
		size.w*drawRatio.x,size.h*drawRatio.y);
	index%=getSpriteNum();//in case the input index is out of range
	sprite.x = sprite.w*(index%col);
	sprite.y = sprite.h*(index/col);
	SDL_RenderCopyEx(renderer,texture,&sprite,&rect,angle,NULL,SDL_FLIP_NONE);
}
void Texture::setColor(SDL_Color&color){
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
}
void Texture::setColor(SDL_Color color){
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
}

int Texture::getSpriteNum()const{
	return row*col;
}
SDL_Rect&Texture::getSprite(){
	return sprite;
}

Size Texture::getSize(){
	return Size(sprite.w, sprite.h);
}
#ifndef ASSETS_H
#define ASSETS_H
#include"Texture.h"
#include<vector>
#include<map>
#include<fstream>
#include<sstream>
#include<mutex>
#define TextureMap std::map<std::string,Texture*>
#define MapMatrix std::vector<std::vector<int>>
class Assets{
public:
	bool loaded;
	TextureMap texturemap;
	//sound
	//map
	MapMatrix map;

	Assets();
	~Assets();
	void loadMap(std::string map_path);
	void loadSounds(std::string sound_path,SDL_Renderer*renderer);
	void loadTextures(std::string texture_path,SDL_Renderer*renderer,std::mutex&mutex,Size windowSize);
	Texture&getTexture(std::string name);
	MapMatrix&getMap();
};
#endif//ASSETS_H

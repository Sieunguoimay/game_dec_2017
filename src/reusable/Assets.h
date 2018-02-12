#ifndef ASSETS_H
#define ASSETS_H
#include"Texture.h"
#include<vector>
#include<map>
#include<fstream>
#include<sstream>
#include<mutex>
#include<string>
#define TextureMap std::map<std::string,Texture*>
#define MapMatrix std::vector<std::vector<int>>
class Assets{
	Assets();

	//this assignment need to be private
	Assets(const Assets&);
	Assets&operator=(const Assets&);
public:
	bool loaded;
	TextureMap texturemap;
	//sound
	//map
	MapMatrix map;
	static Assets&Instance();
	~Assets();
	void loadMap(std::string map_path);
	MapMatrix generateMap(int w, int h);
	void SetMap(MapMatrix m);
	void SetMap(std::string m);
	void loadSounds(std::string sound_path);
	void loadTextures(std::string texture_path,std::mutex&mutex);
	Texture&getTexture(std::string name);
	MapMatrix&getMap();
};
//to make life easier...
#define AssetsAccessor Assets::Instance()
#endif//ASSETS_H

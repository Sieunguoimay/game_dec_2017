#include"Assets.h"
Assets::Assets(){
	loaded = false;
}
Assets::~Assets(){
	for(auto&t:texturemap){
		SDL_Log("deleted texture %s",t.first.c_str());
		delete t.second;
	}
}
void Assets::loadMap(std::string map_path){
	std::ifstream file(map_path);
	std::string line;
	while(std::getline(file,line)){
		if(line.empty())
			continue;
		std::stringstream ss;
		ss.str(line);
		std::vector<int>m;
		while(!ss.eof()){
			int a;
			ss>>a;
			m.push_back(a);
		}
		if(!m.empty())
			map.push_back(m);
	}
	for(auto&m:map){
		for(auto&i:m)
			std::cout<<i<<" ";
		std::cout<<"\n";
	}
}

void Assets::loadSounds(std::string sound_path,SDL_Renderer*renderer){

}
void Assets::loadTextures(std::string texture_path,SDL_Renderer*renderer,std::mutex&mutex,Size windowSize){
	std::ifstream file(texture_path.c_str());
	std::string line,name;

	if(file.is_open()){
		while(std::getline(file,line)){
			if(line.empty())
				continue;
			int row = 1, col = 1;
			std::stringstream ss;
			ss.str(line);
			ss>>name;
			ss>>row;
			ss>>col;
			mutex.lock();
			Texture*texture = new Texture(name,renderer,row,col,windowSize);
			mutex.unlock();

			int pos = name.find('/');
			while(pos>=0&&pos<name.size()){
				name.erase(0,pos+1);
				pos = name.find('/');
			}
			pos = name.find('.');
			if(pos>=0&&pos<name.size())
				name.erase(pos,name.size()-pos);
			SDL_Log("%s %i %i",name.c_str(),row,col);
			texturemap[name] = texture;
		}
		file.close();
	}
	SDL_Log("Number of textures: %i",(int)texturemap.size());
	loaded = true;
}
Texture&Assets::getTexture(std::string name){
	return *texturemap[name];
}

MapMatrix&Assets::getMap(){
	return map;
}

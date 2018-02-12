#include"Assets.h"
Assets&Assets::Instance(){
	static Assets instance;
	return instance;
}


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
	for(int i = 1; i<map.size()-1;i++){
		for(int j = 1; j<map.at(i).size()-1;j++){
			if(map[i][j]!=0)
			if(map[i+1][j]==0||map[i][j+1]==0||map[i-1][j]==0||map[i][j-1]==0)
				map[i][j]=1;
			std::cout<<map[i][j]<<" ";
		}
		std::cout<<"\n";
	}
}
MapMatrix Assets::generateMap(int w, int h){
	//push all the map with number 2
	MapMatrix tmpMap;
	for(int i = 0; i<w;i++){
		std::vector<int>m;
		for(int j = 0;j<h;j++)
			m.push_back(2);
		tmpMap.push_back(m);
	}
	//random location on map
	int i = 0;
	int j = 0;
	int TURNNING_NUM = w*h*3;
	for(int k = 0; k<TURNNING_NUM;k++){
		int randNum = rand()%100;
		
		if(randNum>75){if(j>1)j--;}
		else if(randNum>50){if(j<h-1)j++;}
		else if(randNum>25){if(i>1)i--;}
		else {if(i<w-1)i++;}

		if(i>w-1) i=w-1;
		if(i<1) i=1;

		if(j>h-1) j=h-1;
		if(j<1) j=1;
		tmpMap[i][j] = 0;	
	}
	for(int i = 0; i<tmpMap.size();i++){
		for(int j = 0; j<tmpMap.at(i).size();j++){
			if(i==0||j==0||i==tmpMap.size()-1||j==tmpMap.at(i).size()-1){
				tmpMap[i][j]=1;
			}else{
				if(tmpMap[i][j]!=0)
				if(tmpMap[i+1][j]==0||tmpMap[i][j+1]==0||tmpMap[i-1][j]==0||tmpMap[i][j-1]==0)
					tmpMap[i][j]=1;
			}
			std::cout<<tmpMap[i][j]<<" ";
		}
		std::cout<<"\n";
	}
	return tmpMap;
}

void Assets::SetMap(MapMatrix m){
	this->map = m;
}

void Assets::SetMap(std::string passedin){
	map.clear();
	std::stringstream file(passedin);
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
	for(int i = 1; i<map.size()-1;i++){
		for(int j = 1; j<map.at(i).size()-1;j++){
			if(map[i][j]!=0)
			if(map[i+1][j]==0||map[i][j+1]==0||map[i-1][j]==0||map[i][j-1]==0)
				map[i][j]=1;
			std::cout<<map[i][j]<<" ";
		}
		std::cout<<"\n";
	}
}

void Assets::loadSounds(std::string sound_path){

}
void Assets::loadTextures(std::string texture_path,std::mutex&mutex){
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
			Texture*texture = new Texture(name,row,col);
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
	if(texturemap[name] == NULL)
		return *texturemap["null"];
	return *texturemap[name];
}

MapMatrix&Assets::getMap(){
	return map;
}

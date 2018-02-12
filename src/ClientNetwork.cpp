#include"ClientNetwork.h"
#include"Player.h"
#include"reusable/Assets.h"
#include<sstream>

ClientNetwork::ClientNetwork(){
	receivedMap = false;
	disconnected = false;
	disconnectNow = false;
}
ClientNetwork&ClientNetwork::Instance(){
	static ClientNetwork instance;
	return instance;
}
bool ClientNetwork::Init(const char*ipchar){
	SDLNet_Init();
	IPaddress ip;
	if(SDLNet_ResolveHost(&ip, ipchar,9000)==-1){
		SDL_Log("There was an error with resolving host");
		return false;
	}
	SDL_Log("Resolved host successfully");
	connection = SDLNet_TCP_Open(&ip);
	if(connection==NULL){
		SDL_Log("There was an error with TCP open");		
		return false;
	}
	SDL_Log("Opened Connection");
	server = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(server,connection);
	SDL_Log("Inited Client Network");
	return true;
}
ClientNetwork::~ClientNetwork(){
	SDL_Log("Destroyed Client Network");
	SDLNet_TCP_Close(connection);
	SDLNet_FreeSocketSet(server);
	SDLNet_Quit();
}
void ClientNetwork::Send(Player*p){
	if(p->isReady()&&!disconnectNow){//if player is 
		vec2 pos = p->getPosition();
		//1-id-position-angle
		//1 means send every frame
		sprintf(tmp,"1 %d %f %f %f\n",p->getID(),
			pos.x,pos.y,p->getAngle());
		int size = 0;
		int len = strlen(tmp)+1;
		while(size<len){
			size+=SDLNet_TCP_Send(connection,tmp+size,len-size);
		}
	}
}
void ClientNetwork::SendShot(Player*p,int id){
	if(p->isReady()&&!disconnectNow){
		sprintf(tmp,"3 %d %d",id,p->getID());
		int size = 0;
		int len = strlen(tmp)+1;
		while(size<len){
			size+=SDLNet_TCP_Send(connection,tmp+size,len-size);
		}
	}
}
void ClientNetwork::Recv(std::vector<HardObject*>&enemies,Player*p,std::vector<unsigned int>f){
	while(SDLNet_CheckSockets(server,0)>0&&SDLNet_SocketReady(connection)){
		int offset = 0;
		do{
			offset+=SDLNet_TCP_Recv(connection,tmp+offset,1400);
			if(offset<=0)
				return;
		}while(tmp[strlen(tmp)-1]!='\n');
		int type, id;
		sscanf(tmp,"%d %d",&type,&id);
		if(type==0){
			p->setID(id);
			p->ready();
			ID = id;
			AssetsAccessor.SetMap(std::string(tmp+4));
			receivedMap = true;
		}else if(type==1){
			int i;
			for(i = 0; i<enemies.size();i++){
				if(enemies[i]->getID()==id){
					//1-id-position-angle
					int tmp2;
					float x, y,angle;
					sscanf(tmp,"1 %d %f %f %f \n",&tmp2,&x,&y,&angle);
					enemies[i]->setPosition(vec2(x,y));
					enemies[i]->setAngle(angle);
					break;
				}
			}
			if(i>=enemies.size()){
				enemies.push_back(new HardObject(vec2(500,200),
					Size(AssetsAccessor.getTexture("player").getSize().w*0.5,
						AssetsAccessor.getTexture("player").getSize().h*0.5),"player"));
				enemies[enemies.size()-1]->setID(id);
				SDL_Log("Connected new enemies %d",id);
			}
		}else if(type==2){
			for(int i = 0; i<enemies.size();i++){
				if(enemies[i]->getID()==id){
					delete enemies[i];
					enemies.erase(enemies.begin()+i);
					SDL_Log("Disonnected enemies %d",id);
				}
			}
		}else if(type==3){
			int tmp1, tmp2;
			sscanf(tmp,"3 %d %d",&tmp1,&tmp2);
			SDL_Log("Shotted by %d",tmp2);
		}else if(type==4){//disconnect accepted
			SDL_Log("I got Accepted");
			disconnected = true;
		}
	}

	if(disconnectNow){
		SDL_Log("Disconnect now");
		sprintf(tmp,"2 %d \n",p->getID());
		int size = 0;
		int len = strlen(tmp)+1;
		while(size<len){
			size+=SDLNet_TCP_Send(connection,tmp+size,len-size);
		}
		//disconnectNow = false;
	}

}
void ClientNetwork::Disconnect(){
	//SDL_Log("Disconnected to server");
	disconnectNow = true;
}

void ClientNetwork::SendMsg(char msg[1400]){
	int size = 0;
	int len = strlen(msg)+1;
	while(size<len){
		size+=SDLNet_TCP_Send(connection,msg+size,len-size);
	}
}

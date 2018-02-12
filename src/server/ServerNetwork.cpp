#include<SDL2/SDL.h>
#include<cstring>
#include"ServerNetwork.h"
ServerNetwork::ServerNetwork(){
	playernum = 0;
	curid = 0;
}
ServerNetwork&ServerNetwork::Instance(){
	static ServerNetwork instance;
	return instance;
}
void ServerNetwork::Init(){
	SDLNet_Init();
	sockets = SDLNet_AllocSocketSet(MAX_CONNECTION_NUM);
	SDLNet_ResolveHost(&ip,NULL,PORT);//NULL mean this is server babe
	server = SDLNet_TCP_Open(&ip);	
	SDL_Log("Server listening on %d",PORT);
}
ServerNetwork::~ServerNetwork(){
	for(int i = 0;i<socketvector.size();i++){
		SDLNet_TCP_Close(socketvector[i].socket);
	}
	SDLNet_FreeSocketSet(sockets);
	SDLNet_TCP_Close(server);
	SDLNet_Quit();
	SDL_Log("Destroyed server");
}
std::string ServerNetwork::generateStringMap(int w, int h){
	std::vector<std::vector<int>> tmpMap;
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



	std::string m;
	for(int i = 0; i<tmpMap.size();i++){
		for(int j = 0;j<tmpMap[i].size();j++){
			if(tmpMap[i][j] == 0)
				m+="0 ";
			else if(tmpMap[i][j] == 1)
				m+="1 ";
			else if(tmpMap[i][j] == 2)
				m+="2 ";
		}
		m[m.length()-1] = '\n';
	}
	return m;
}

void ServerNetwork::Update(){
	//if someone try to connect to the server
	TCPsocket tmpsocket = SDLNet_TCP_Accept(server);
	if(tmpsocket){
		if(playernum<MAX_CONNECTION_NUM){
			SDLNet_TCP_AddSocket(sockets,tmpsocket);
			socketvector.push_back(data(tmpsocket,SDL_GetTicks(),curid));
			if(playernum==0)
				stringMap = generateStringMap(15,20);			
			sprintf(tmp,"0 %d %s\n",curid,getStringMap().c_str());
			playernum++;
			curid++;
			SDL_Log("New connection %d",curid);
		}else{
			sprintf(tmp,"3 \n");
		}
		SDLNet_TCP_Send(tmpsocket,tmp,strlen(tmp)+1);
	}
	//check for in comming data
	while(SDLNet_CheckSockets(sockets,0)>0){
		for(int i = 0;i<socketvector.size();i++){
			if(SDLNet_SocketReady(socketvector[i].socket)){
				socketvector[i].timeout = SDL_GetTicks();
				SDLNet_TCP_Recv(socketvector[i].socket,tmp,1400);
				//read the number
				int num = tmp[0]-'0';
				int j = 1;
				while(tmp[j]>='0'&&tmp[j]<='9'){
					num*=10;
					num+=tmp[j]-'0';
					j++;
				}
				//send the message to all the player
				if(num==1){
					for(int k=0; k<socketvector.size();k++){
						if(k==i)continue;
						SDLNet_TCP_Send(socketvector[k].socket,tmp,strlen(tmp)+1);
					}
				}else if(num==2){
					for(int k=0; k<socketvector.size();k++){
						if(k==i){
							char msg[] = "4 \n";
							int size = 0;
							int len = strlen(msg)+1;
							while(size<len){
								size+=SDLNet_TCP_Send(socketvector[i].socket,msg+size,len-size);
							}
						}
						SDLNet_TCP_Send(socketvector[k].socket,tmp,strlen(tmp)+1);
					}
					SDL_Log("Disconnected with player %d",socketvector[i].id);
					SDLNet_TCP_DelSocket(sockets,socketvector[i].socket);
					SDLNet_TCP_Close(socketvector[i].socket);
					socketvector.erase(socketvector.begin()+i);
					playernum--;
				}else if(num==3){//somebody shot other
					int t;
					sscanf(tmp,"3 %d",&t);
					for(int k=0; k<socketvector.size();k++){
						if(socketvector[k].id == t){
							SDLNet_TCP_Send(socketvector[k].socket,tmp,strlen(tmp)+1);
							break;
						}
					}
				}
			}
		}
	}

	//check if some connections is waiting too long, timeout
	for(int i = 0; i<socketvector.size();i++){
		if(SDL_GetTicks()-socketvector[i].timeout>5000){//5sec
			//send to all players to tell them that connection is deleted
			sprintf(tmp,"2 %d \n",socketvector[i].id);
			for(int j = 0; j<socketvector.size();j++){
				if(j==i)continue;
				SDLNet_TCP_Send(socketvector[j].socket,tmp,strlen(tmp)+1);
			}
			SDLNet_TCP_DelSocket(sockets,socketvector[i].socket);
			SDLNet_TCP_Close(socketvector[i].socket);
			socketvector.erase(socketvector.begin()+i);
			playernum--;
			SDL_Log("Disconnected by timeout %d\n",socketvector[i].id);
		}
		SDL_Delay(1);
	}
}

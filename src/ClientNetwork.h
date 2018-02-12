#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H
#include<iostream>
#include<vector>
#include<string>

#include<SDL2/SDL_net.h>
class HardObject;
class Player;
class ClientNetwork{
	SDLNet_SocketSet server;
	TCPsocket connection;
	char tmp[1400];
	
	ClientNetwork();

	ClientNetwork(const ClientNetwork&);
	ClientNetwork&operator=(const ClientNetwork&);
	void SendMsg(char msg[1400]);
	int ID;
	bool receivedMap;

	bool disconnected;
	bool disconnectNow;
public:
	static ClientNetwork&Instance();
	bool Init(const char*ip);
	~ClientNetwork();
	void Send(Player*p);
	void SendShot(Player*enemy,int id);
	void Recv(std::vector<HardObject*>&enemies,Player*p,std::vector<unsigned int>f);
	void Disconnect();
	bool HasDisconnected()const{return disconnected;}
	bool HasRecvedMap()const {return receivedMap;}
	std::vector<std::vector<int>> RequestMap();
};
#define Client ClientNetwork::Instance()
#endif//CLIENT_NETWORK_H
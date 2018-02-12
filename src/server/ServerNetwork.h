#ifndef SERVER_NETWORK_H
#define SERVER_NETWORK_H
#include<SDL2/SDL_net.h>
#include<iostream>
#include<vector>
#include<string>
#define SDL_reinternet_cast(type, expression) reinternet_cast<type>(expression)
#define MAX_CONNECTION_NUM 30
#define PORT 9000
struct data{
	TCPsocket socket;
	Uint32 timeout;
	int id;
	data(TCPsocket socket, Uint32 timeout, int id)
	:socket(socket),timeout(timeout),id(id){}
};
class ServerNetwork{
	ServerNetwork();
	ServerNetwork(const ServerNetwork&);
	ServerNetwork&operator=(const ServerNetwork&);
	char tmp[1400];
	std::vector<data> socketvector;
	std::string map;
	IPaddress ip;
	SDLNet_SocketSet sockets;
	TCPsocket server;

	int playernum;
	int curid;
	std::string stringMap;
	std::string generateStringMap(int w, int h);
	std::string getStringMap()const{return stringMap;}
public:
	~ServerNetwork();
	static ServerNetwork&Instance();
	void Init();
	void Update();
};
//to make life easier..
#define Server ServerNetwork::Instance()
#endif//NETWORK_H
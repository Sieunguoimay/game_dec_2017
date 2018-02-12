#include"ServerNetwork.h"
#include<iostream>
int main(int argc, char**argv){
	Server.Init();
	bool running = true;
	while(running){
		Server.Update();
	}
	return 0;	
}
#include<thread>
#include<mutex>
#include<condition_variable>
#include<iostream>
#include<string>
#include<time.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include"reusable/TheScreen.h"
#include"reusable/Assets.h"
#include"reusable/Camera.h"
#include"Game.h"
#include"ClientNetwork.h"
bool running;
bool loaded = false;
bool initiated = false;
std::mutex initiating_mutex;
std::condition_variable cv;

const int WIDTH = 1366;
const int HEIGHT = 768;

void load_data(){
	std::mutex renderer_mutex;
	//SDL_SetRenderDrawColor(renderer,0xff,0xff,0xff,0xff);
	srand(time(0));
	std::unique_lock<std::mutex>lk(initiating_mutex);
	cv.wait(lk,[]{
		SDL_Log("Waiting for initialization of SDL");
		return initiated;});

	//assets.loadMap("../game_dec_2017/assets/map.txt");
	std::cout<<"Loading Data...\n";
	AssetsAccessor.loadTextures("../game_dec_2017/assets/images/names.txt",renderer_mutex);
	std::cout<<"Generating Map...\n";
	AssetsAccessor.SetMap(AssetsAccessor.generateMap(
		HEIGHT/AssetsAccessor.getTexture("wallBrick").getSize().h+2,
		WIDTH/AssetsAccessor.getTexture("wallBrick").getSize().w+2));
	loaded = true;

	lk.unlock();
    cv.notify_one();//tell them we are done
}
void render_while_loading(){
	std::mutex loading_mutex;
	SDL_Event event;
	loading_mutex.lock();
	while(!loaded){
		loading_mutex.unlock();

/*		renderer_mutex.lock();
		SDL_SetRenderDrawColor(renderer,0xff,0xff,0x00,0xff);
		SDL_RenderClear(renderer);
		SDL_Delay(100);//no need to render fast here
		SDL_RenderPresent(renderer);
		renderer_mutex.unlock();
*/
		loading_mutex.lock();
	}		

}
char ip[40];
void main_loop(){
	/*window = SDL_CreateWindow("hello",
		SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
		windowSize.w,windowSize.h,SDL_WINDOW_SHOWN|SDL_WINDOW_BORDERLESS|SDL_WINDOW_FULLSCREEN_DESKTOP);
	renderer = SDL_CreateRenderer(window,-1,
		SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
	*/

	Screen.CreateWindow("Game Ban Nhau");
	std::cout<<"Wrong\n";
	Game game;
	//at this point, initiating is done, send the signal to the load_data thread to start
	{
		std::lock_guard<std::mutex> lk(initiating_mutex);
		initiated = true;
	}cv.notify_all();//tell them we are ready

	//network goes here...
	{
		std::unique_lock<std::mutex> lk(initiating_mutex);
		cv.wait(lk,[]{
			SDL_Log("Waiting for data... Draw anything here!");
			return loaded;});
	}
	game.connectServer(ip);

	SDL_Log("Enter main loop");
	SDL_Event event;
	game.init();
	while(game.isRunning()){
		while(SDL_PollEvent(&event)){
			game.handleEvent(event);
			CameraAccessor.HandleEvent(event);
		}
		game.update();
		CameraAccessor.Update();

		SDL_SetRenderDrawColor(Screen.GetRenderer(),0xff,0xff,0xff,0xff);
		SDL_RenderClear(Screen.GetRenderer());
		game.draw();
		SDL_RenderPresent(Screen.GetRenderer());
	}		
}
int main(int argc, char**argv){
	Size windowSize;
	srand(time(0));
	std::stringstream ss;
	for(int i = 1; i<argc;i++){
		ss<<argv[i]<<" ";
	}
	if(argc == 2){
		//float a;ss>>a;
		//windowSize.set(WIDTH*a,HEIGHT*a);
		ss>>ip;
		std::cout<<"Entered IP: "<<ip<<"\n";
	}
	else{
	//	windowSize.set(WIDTH, HEIGHT);
	//	ss>>windowSize.w;
	//	ss>>windowSize.h;
	}


	std::thread main_thread(main_loop);
	std::thread load_data_thread(load_data);
	std::thread render_while_loading_thread(render_while_loading);

	render_while_loading_thread.join();
	load_data_thread.join();
	main_thread.join();

	SDL_Log("Threads joined");
	return 0;
}

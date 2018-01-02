#include<thread>
#include<mutex>
#include<condition_variable>
#include<iostream>
#include<string>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"Game.h"
#include"Assets.h"
SDL_Window*window;
SDL_Renderer*renderer;
Size windowSize;
bool running;
bool loaded = false;
bool initiated = false;
std::mutex loading_mutex;
std::mutex renderer_mutex;
std::mutex initiating_mutex;
std::condition_variable cv;

Assets assets;

void load_data(){
	//SDL_SetRenderDrawColor(renderer,0xff,0xff,0xff,0xff);
	std::unique_lock<std::mutex>lk(initiating_mutex);
	cv.wait(lk,[]{
		SDL_Log("Waiting for initialization of SDL");
		return initiated;});

	std::cout<<"Loading Data...\n";
	assets.loadMap("../project_dec_2017/assets/map.txt");
	assets.loadTextures("../project_dec_2017/assets/images/names.txt",renderer,renderer_mutex,windowSize);
	loaded = true;

	lk.unlock();
    cv.notify_one();//tell them we are done
}
void render_while_loading(){
	SDL_Event event;
	loading_mutex.lock();
	while(!loaded){
		loading_mutex.unlock();

		renderer_mutex.lock();
		SDL_SetRenderDrawColor(renderer,0xff,0xff,0x00,0xff);
		SDL_RenderClear(renderer);
		SDL_Delay(100);//no need to render fast here
		SDL_RenderPresent(renderer);
		renderer_mutex.unlock();

		loading_mutex.lock();
	}		

}
void main_loop(){
	window = SDL_CreateWindow("hello",
		SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
		windowSize.w,windowSize.h,SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window,-1,
		SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
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

	SDL_Log("Enter main loop");
	SDL_Event event;
	running = true;
	Game game(assets);
	while(running){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
			}
			game.handleEvent(event);
		}
		game.update();
		SDL_SetRenderDrawColor(renderer,0xff,0xff,0xff,0xff);
		SDL_RenderClear(renderer);
		game.draw(renderer);
		SDL_RenderPresent(renderer);
	}		

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
int main(int argc, char**argv){
	std::stringstream ss;
	for(int i = 1; i<argc;i++){
		ss<<argv[i]<<" ";
	}
	if(argc == 2){
		float a;ss>>a;
		windowSize.set(1366*a,768*a);
	}
	else{
		windowSize.set(1366,768);
		ss>>windowSize.w;
		ss>>windowSize.h;
	}
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);


	std::thread main_thread(main_loop);
	std::thread load_data_thread(load_data);
	std::thread render_while_loading_thread(render_while_loading);

	render_while_loading_thread.join();
	load_data_thread.join();
	main_thread.join();

	IMG_Quit();
	SDL_Quit();
	return 0;
}
#include"Game.h"
#include"reusable/Assets.h"
#include"reusable/Camera.h"
#include"reusable/TheScreen.h"
#include"ClientNetwork.h"

Game::Game()
:player(vec2(500,200),Size(0,0))
{
	isOnline = true;
	running = true;
	timeout = -1;
}
Game::~Game(){
	//if(isOnline){
	//	Client.Disconnect(&player);
	//}
	for(int i = 0; i<bullets.size();i++){
		delete bullets[i];
		bullets[i] = NULL;
	}

	for(int i = 0; i<enemies.size();i++){
		delete enemies[i];
		enemies[i] = NULL;
	}
	SDL_Log("Destroyed Game");
}
void Game::connectServer(const char*ipEntered){
	char ip[40];
	if(isOnline){
		//std::cin.getline(ip,20);
		if(strlen(ipEntered)<5)
			strcpy(ip,"192.168.137.85");
		else
			strcpy(ip,ipEntered);
		SDL_Log("Host: %s",ip);
		isOnline = Client.Init(ip);
		if(!isOnline){
			SDL_Log("Offline");
			return;
		}
		SDL_Log("Received map from server");
		while(!Client.HasRecvedMap()){
			Client.Recv(enemies,&player,frame);
		}
	}
}
void Game::init(){
	environment.putBricks(AssetsAccessor.getMap(),AssetsAccessor.getTexture("wallBrick").getSize());
	player.setSize(Size(AssetsAccessor.getTexture("player").getSize().w*0.75,
			AssetsAccessor.getTexture("player").getSize().h*0.75));
	player.setPosition(environment.getInsideWallBricks().at(0)->getPosition());
	player.setBullet("bullet",AssetsAccessor.getTexture("bullet").getSize(),10.0f);

}
void Game::handleEvent(SDL_Event&event){
	if(event.type == SDL_KEYDOWN){
		if(event.key.keysym.sym == SDLK_LEFT||event.key.keysym.sym == SDLK_a)
			player.moveDir.left = true;
			//player.move(MOVE_LEFT);
		if(event.key.keysym.sym == SDLK_RIGHT||event.key.keysym.sym == SDLK_d)
			player.moveDir.right = true;
			//player.move(MOVE_RIGHT);
		if(event.key.keysym.sym == SDLK_UP||event.key.keysym.sym == SDLK_w)
			player.moveDir.up = true;
		if(event.key.keysym.sym == SDLK_DOWN||event.key.keysym.sym == SDLK_s)
			player.moveDir.down = true;
	}
	if(event.type == SDL_KEYUP){
		if(event.key.keysym.sym == SDLK_LEFT||event.key.keysym.sym == SDLK_a)
			player.moveDir.left = false;
			//player.move(MOVE_LEFT);
		if(event.key.keysym.sym == SDLK_RIGHT||event.key.keysym.sym == SDLK_d)
			player.moveDir.right = false;
			//player.move(MOVE_RIGHT);
		if(event.key.keysym.sym == SDLK_UP||event.key.keysym.sym == SDLK_w)
			player.moveDir.up = false;
		if(event.key.keysym.sym == SDLK_DOWN||event.key.keysym.sym == SDLK_s)
			player.moveDir.down = false;
		}
	if(event.type == SDL_MOUSEMOTION){
		vec2 dif((float)event.motion.x,(float)event.motion.y);
		dif = CameraAccessor.ToGameCoords(dif);
		//dif.x /= AssetsAccessor.getTexture("player").drawRatio.x;
		//dif.y /= AssetsAccessor.getTexture("player").drawRatio.y;
		dif -= player.getPosition();
		player.setDirection(dif);
	}
	if(event.type == SDL_MOUSEBUTTONDOWN){
		bullets.push_back(player.shoot());
	}

	if(event.type == SDL_QUIT){
		if(isOnline){
			Client.Disconnect();
			//isOnline = false;
			timeout = SDL_GetTicks();
		}
		else
			running = false;
	}

}
void Game::draw(){
	environment.draw();
	AssetsAccessor.getTexture(player.getName()).render(player.getPosition(),Size(-1,-1),player.getAngle());
	for(int i = 0; i<bullets.size();i++){
		AssetsAccessor.getTexture(bullets[i]->getName()).render(bullets[i]->getPosition(),Size(-1,-1),bullets[i]->getAngle());
	}
	for(int i =0; i<enemies.size();i++)
		AssetsAccessor.getTexture(enemies[i]->getName()).render(enemies[i]->getPosition(),Size(-1,-1),
			enemies[i]->getAngle());
}
bool Game::isRunning()const{
	return running;
}
void Game::update(){
	environment.wallCollide(player);
	environment.wallCollide(bullets);
	for(int i =0; i<enemies.size();i++)
		enemies[i]->collide(player);
	for(int i = 0; i<bullets.size();i++){
		Bullet*bullet = ((Bullet*)bullets[i]);
		bullet->move();
		if(bullet->hasCollided()){
			delete bullet;
			bullets.erase(bullets.begin()+i);
		}
	}
	player.move();
	player.update();
	CameraAccessor.SetTarget(player.getPosition()-vec2(Screen.GetSize().w/2,Screen.GetSize().h/2));
	if(isOnline){
		Client.Send(&player);
		Client.Recv(enemies,&player,frame);
		if(Client.HasDisconnected())
			running = false;
		if(timeout>-1)
		if(SDL_GetTicks()-timeout>2000)
			running = false;

	}
}


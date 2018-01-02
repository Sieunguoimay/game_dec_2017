#include"Game.h"

Game::Game(Assets&assets)
:assets(&assets)
,player(vec2(500,200),assets.getTexture("player").getSize())
,player2(vec2(500,200),assets.getTexture("player").getSize())
{
	environment.putBricks(assets.getMap(),assets.getTexture("wallBrick").getSize());
	player.setPosition(environment.getInsideWallBricks().at(0)->getPosition());
}
Game::~Game(){
	assets = NULL;
}
void Game::handleEvent(SDL_Event&event){
	if(event.type == SDL_KEYDOWN){
		if(event.key.keysym.sym == SDLK_LEFT)
			player.moveDir.left = true;
			//player.move(MOVE_LEFT);
		if(event.key.keysym.sym == SDLK_RIGHT)
			player.moveDir.right = true;
			//player.move(MOVE_RIGHT);
		if(event.key.keysym.sym == SDLK_UP)
			player.moveDir.up = true;
		if(event.key.keysym.sym == SDLK_DOWN)
			player.moveDir.down = true;
	}
	if(event.type == SDL_KEYUP){
		if(event.key.keysym.sym == SDLK_LEFT)
			player.moveDir.left = false;
			//player.move(MOVE_LEFT);
		if(event.key.keysym.sym == SDLK_RIGHT)
			player.moveDir.right = false;
			//player.move(MOVE_RIGHT);
		if(event.key.keysym.sym == SDLK_UP)
			player.moveDir.up = false;
		if(event.key.keysym.sym == SDLK_DOWN)
			player.moveDir.down = false;
	}
	if(event.type == SDL_MOUSEMOTION){
		vec2 dif((float)event.motion.x,(float)event.motion.y);
		dif.x /= assets->getTexture("player").drawRatio.x;
		dif.y /= assets->getTexture("player").drawRatio.y;
		dif -= player.getPosition();
		player.setDirection(dif);
	}
}
void Game::draw(SDL_Renderer*renderer){
	environment.draw(*assets);
	assets->getTexture("player").render(player.getPosition().x,player.getPosition().y,-1,-1,player.getAngle());
}
void Game::update(){
	environment.wallCollide(player);
	player.move();
	player.update();
	//player2.collide(player);
}


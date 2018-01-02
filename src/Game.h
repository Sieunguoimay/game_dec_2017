#ifndef GAME_H
#define GAME_H
#include"Assets.h"
#include"Player.h"
#include"Environment.h"
#define WINDOW_W 1366
#define WINDOW_H 768

class Game{
	Player player;
	Player player2;
	Environment environment;
public:
	Assets*assets;

	Game(Assets&asset);
	~Game();
	void handleEvent(SDL_Event&event);
	void draw(SDL_Renderer*renderer);
	void update();
	
};
#endif//GAME_H

#ifndef GAME_H
#define GAME_H
#include"Player.h"
#include"Environment.h"
#include"Bullet.h"
#define WINDOW_W 1366
#define WINDOW_H 768
class Game{
	Player player;
	Environment environment;
	std::vector<CollisionObject*>bullets;
	std::vector<NonCollisionObject*>enemies;
	std::vector<unsigned int> frame;
	bool isOnline;

public:

	Game();
	~Game();
	void connectServer();
	void init();
	void handleEvent(SDL_Event&event);
	void draw();
	void update();
	
};
#endif//GAME_H

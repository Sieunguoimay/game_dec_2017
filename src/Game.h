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
	std::vector<HardObject*>enemies;
	std::vector<unsigned int> frame;
	bool isOnline;
	bool running;
	int timeout;
public:

	Game();
	~Game();
	void connectServer(const char*ipEntered="");
	void init();
	void handleEvent(SDL_Event&event);
	void draw();
	void update();
	bool isRunning()const;
};
#endif//GAME_H

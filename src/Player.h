#ifndef PLAYER_H
#define PLAYER_H
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include"game_object.h"
#include"Bullet.h"

struct MoveDirection{
	bool left, right, up, down;
	MoveDirection()
	:left(false),right(false),up(false),down(false){}
};
class Player:public HardObject{
	bool isready;
	vec2 velocity;
	float speed;
	float newAngle;
	BulletType bulletType;
public:
	MoveDirection moveDir;
	Player(vec2 pos, Size size,float speed = 4.0f);
	~Player();

	void setBullet(std::string id, Size size,float speed);
	Bullet* shoot();
	void move();
	void update();
	void setDirection(const vec2&dir);//override
	float getAngle();//override

	//for networking goes here..
	bool isReady()const{return isready;}
	void ready(){isready = true;}
};

#endif//PLAYER_H

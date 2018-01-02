#ifndef PLAYER_H
#define PLAYER_H
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include"game_object.h"
struct MoveDirection{
	bool left, right, up, down;
	MoveDirection()
	:left(false),right(false),up(false),down(false){}
};
class Player:public HardObject{
	vec2 velocity;
	float speed;
	float angle;
	float newAngle;
public:
	MoveDirection moveDir;
	Player(vec2 pos, Size size);
	~Player();
	void move();
	void update();
	void setDirection(const vec2&dir);//override
	float getAngle();//override
};

#endif//PLAYER_H

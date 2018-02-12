#ifndef BULLET_H
#define BULLET_H
#include"game_object.h"
struct BulletType{
	Size size;
	float speed;
	std::string name;
	BulletType(std::string name = "null",Size size=Size(20,20), float speed = 5)
	:name(name),size(size),speed(speed){}
};

class Bullet:public CollisionObject{
	vec2 direction;
	float speed;
	bool collided;
public:
	Bullet(vec2 position,vec2 direction, Size size, std::string id = "null",float speed = 5.0f);
	~Bullet();
	void setDirection(vec2 direction);
	bool& hasCollided();
	void move();
	vec2 getVelocity();
	virtual void reactToCollision();
};
#endif//BULLET_H

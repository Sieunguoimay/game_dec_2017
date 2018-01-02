#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include"geometry.h"
#include<vector>
#include<algorithm>
class NonCollisionObject{
protected:
	vec2 position;
	Size size;
	float angle;
public:
	NonCollisionObject(vec2 position, Size size);
	~NonCollisionObject();

	virtual void setPosition(vec2 pos);
	virtual void setAngle(float a);
	virtual void setDirection(const vec2&dir);
	vec2 getDirection();
	vec2& getPosition();
	float getAngle();
	Size& getSize();
};

class CollisionObject:public NonCollisionObject{
protected:
	std::vector<vec2> corners;
public:
	CollisionObject(vec2 position, Size size);
	~CollisionObject();
	bool checkCollision(CollisionObject&other);
	std::vector<vec2> getCorners();
	std::vector<vec2> getAxis(std::vector<vec2>&c1,std::vector<vec2>&c2);
};

class HardObject:public CollisionObject{
public:
	HardObject(vec2 position, Size size);
	~HardObject();
	bool collide(CollisionObject&collide);
};
#endif//GAME_OBJECT_H

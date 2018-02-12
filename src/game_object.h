#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include"reusable/geometry.h"
#include<vector>
#include<algorithm>
class NonCollisionObject{
protected:
	vec2 position;
	Size size;
	float angle;
	std::string name;
	int id;
public:
	NonCollisionObject(vec2 position, Size size,std::string name = "null");
	~NonCollisionObject();

	virtual void setPosition(vec2 pos);
	virtual void setAngle(float a);
	virtual void setDirection(const vec2&dir);
	vec2 getDirection();
	vec2& getPosition();
	float getAngle();
	Size& getSize();
	std::string& getName();
	void setID(int id){this->id = id;}
	int getID()const{return this->id;}
};

class CollisionObject:public NonCollisionObject{
protected:
	std::vector<vec2> corners;
public:
	CollisionObject(vec2 position, Size size,std::string id = "null");
	~CollisionObject();
	bool checkCollision(CollisionObject&other);
	std::vector<vec2> getCorners();
	std::vector<vec2> getAxis(std::vector<vec2>&c1,std::vector<vec2>&c2);
	virtual void reactToCollision(){}
};

class HardObject:public CollisionObject{
public:
	HardObject(vec2 position, Size size,std::string id = "null");
	~HardObject();
	bool collide(CollisionObject&collide);
};
#endif//GAME_OBJECT_H

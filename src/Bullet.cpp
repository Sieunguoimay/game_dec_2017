#include"Bullet.h"
Bullet::Bullet(vec2 position,vec2 direction, Size size, std::string id,float speed)
:CollisionObject(position, size, id),direction(direction),speed(speed)
{
	collided = false;
}
Bullet::~Bullet(){

}

void Bullet::move(){
	setAngle(atan2(direction.y,direction.x)*180.0f/3.141592645f);
	position+=direction*speed;
}
void Bullet::setDirection(vec2 direction){
	this->direction = direction;
}

vec2 Bullet::getVelocity(){
	return direction*speed;
}
void Bullet::reactToCollision(){
	collided = true;
}
bool& Bullet::hasCollided(){
	return collided;
}
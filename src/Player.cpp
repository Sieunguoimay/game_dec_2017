#include"Player.h"

Player::Player(vec2 pos, Size size,float speed)
:HardObject(pos,size,"player"),speed(speed)
{
	angle = 0;
	newAngle = 0;
	isready = false;
}
Player::~Player(){
}
void Player::setBullet(std::string name, Size size,float speed){
	bulletType.name = name;
	bulletType.size = size;
	bulletType.speed = speed;
}

Bullet* Player::shoot(){
	return new Bullet(position+getDirection()*size.w,getDirection(),
		bulletType.size,bulletType.name,bulletType.speed);
}

void Player::move(){
	if(moveDir.left) velocity.x = -1;
	if(moveDir.right) velocity.x = 1;
	if(moveDir.up) velocity.y = -1;
	if(moveDir.down) velocity.y = 1;
}

void Player::update(){

	if(velocity.x!=0.0f||velocity.y!=0.0f){
		//newAngle = atan2(velocity.y,velocity.x)*180.0f/3.141592645f;
		velocity.normalize();
		velocity*=speed;
		position+=velocity;
		velocity.set(0,0);
	}
	//update angle incase newAngle is found
	if(newAngle-angle!=0.0f){
		float difference = (newAngle-angle);
		if(angle<0.0f)
			if(newAngle-angle>180.0f)
				difference = (newAngle-angle) - 360.0f;
		if(angle>=0.0f){
			if(angle-newAngle>180.0f)
				difference = 360.0f+(newAngle-angle);
		}
		angle += difference*0.1f;//chasing the new angle with the rate 0.2f time difference

		//keep angle in the rangle -180 to 180 after updating
		if(angle<=-180.0f)
			angle+=360.0f;
		else if(angle>180.0f)
			angle-=360.0f;
	}
}
void Player::setDirection(const vec2&dir){
	NonCollisionObject::setDirection(dir);
	newAngle = NonCollisionObject::getAngle();
}

float Player::getAngle(){//override return following angle - get angle to draw
	return angle;
}
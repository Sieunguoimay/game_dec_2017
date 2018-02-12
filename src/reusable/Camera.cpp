#include"Camera.h"
#include"TheScreen.h"


Camera::Camera(vec2 position)
:MovingObject(position)
{
	std::cout<<"Created Camera"<<position.x<<" "<<position.y<<std::endl;
	maxForce = 1.0f;
	maxSpeed = 12.0f;
	zoom = 100;
	zoomTarget = 50;
	sizeRatio = 1.0f;
}

Camera&Camera::Instance(){
	static Camera instance;
	return instance;
}

Camera::~Camera(){
	sizeRatio = 0;
	SDL_Log("Destroyed Camera");
}
void Camera::HandleEvent(SDL_Event&mainEvent){
	SDL_PumpEvents();
	int x;
	int y;
	static bool pressed = false;
	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		if(!pressed){
			anchor.set(x,y);
			oldTarget = target;
			pressed = true;
		}
		vec2 diff = vec2(x,y) - anchor;
		//diff.y = -diff.y;
		target = oldTarget - diff;
	}else {
		if(pressed)
			pressed = false;
	}
	//wheel event checking
	if(mainEvent.wheel.y == 1){//scroll up
		if(zoomTarget>20)
			zoomTarget-=10;
	}else if(mainEvent.wheel.y == -1){//scroll down
		if(zoomTarget<200)
			zoomTarget+=10;
	}
}
void Camera::Update(){
	if(std::abs(zoomTarget-zoom)>1)
		zoom+=float(zoomTarget-zoom)*0.2f;
	applyForce(seek(target));
	move();
}
void Camera::SetTarget(vec2 target){
	this->target = target;
}
vec2 Camera::ToDeviceCoords(const vec2&point){
	vec2 deviceCoordsPos = point - this->position;
	//deviceCoordsPos.y = -deviceCoordsPos.y;
	//deviceCoordsPos*=(sizeRatio*zoom*0.01f);
	deviceCoordsPos.x+= Screen.GetSize().w/2;
	deviceCoordsPos.y+= Screen.GetSize().h/2;
	return deviceCoordsPos;
}
vec2 Camera::ToGameCoords(const vec2&point){
	//point is in device frame

	vec2 gameCoordsPos = position + point;// - vec2(Screen.GetSize().w/2,Screen.GetSize().h/2);
	//gam eCoordsPos.y = -gameCoordsPos.y;

	//gameCoordsPos *= (100.0f/(zoom*(sizeRatio)));
	return gameCoordsPos;
}




//////////////////////////////////////////////////////////////////////////
//movingobject defintion
static float linear(float value, float minOfValue, float maxOfValue, float minOfNewValue, float 	maxOfNewValue) {
		float a = value - minOfValue;
		float rate = a / (maxOfValue - minOfValue);
		float b = rate*(maxOfNewValue - minOfNewValue);
		b += minOfNewValue;
		return b;
}

MovingObject::MovingObject(	
	
		vec2 position,
		vec2 velocity,
		vec2 acceleration,
		float maxSpeed,
		float maxForce,
		float mass
	)
	:position(position),velocity(velocity),acceleration(acceleration)
	,target(position)
	,maxSpeed(maxSpeed),maxForce(maxForce),mass(mass)
{
	slowDownRadian = 100.0f;
}
MovingObject::~MovingObject(){
	
}
void MovingObject::applyForce(vec2 force){
	force*=(1.0f/mass);
	acceleration += force;//this function is called several times in each loop for each force
}
void MovingObject::move(){
	
	velocity+=acceleration;
	position+=velocity;
	acceleration*=0.0f;//because acceleration is addition of all forces 
}
vec2 MovingObject::seek(vec2 target){
	
	vec2 desired = target - position;
	float distance = desired.length();
	desired.normalize();

	if (distance < slowDownRadian) {

		float m = linear(distance, 0.0f, slowDownRadian, 0.0f, maxSpeed);
		desired *= m;
	}else
		desired *= maxSpeed;
	
	vec2 steer = desired - velocity;
	steer.truncate(maxForce);
	return steer;
}

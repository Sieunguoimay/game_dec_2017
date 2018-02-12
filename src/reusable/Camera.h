#ifndef CAMERA_H
#define CAMERA_H
#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H
#include"vec2.h"
class MovingObject{
public:
	MovingObject(
	vec2 position = vec2(0.0f,0.0f),
	vec2 velocity= vec2(0.0f,0.0f),
	vec2 acceleration= vec2(0.0f,0.0f),
	float maxSpeed = 2.0f,
	float maxForce = 0.05f,
	float mass = 1.0f);

	~MovingObject();

	vec2 seek(vec2 target);
	void applyForce(vec2 force);
	void move();
	float mass;
	float slowDownRadian;

	vec2 target;
	vec2 position;
	vec2 velocity;
	vec2 acceleration;
	
	float maxSpeed;
	float maxForce;
};
#endif//MOVING_OBJECT_H

#include<SDL2/SDL.h>

class Camera:public MovingObject{
	Camera(vec2 position = vec2(0.0f,0.0f));
	Camera(const Camera&);
	Camera&operator=(const Camera&);
public:
	static Camera&Instance();
	~Camera();
	void HandleEvent(SDL_Event&mainEvent);
	void Update();
	void SetTarget(vec2 target);
	vec2 ToDeviceCoords(const vec2&point);
	vec2 ToGameCoords(const vec2&point);
	vec2&GetPosition(){return position;}
	float zoom;
	float sizeRatio;
private:
	int zoomTarget;
	vec2 anchor;
	vec2 oldTarget;
};
#define CameraAccessor Camera::Instance()
#endif//CAMERA_H
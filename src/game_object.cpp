#include"game_object.h"

NonCollisionObject::NonCollisionObject(vec2 position, Size size,std::string name)
:position(position),size(size),angle(0),name(name),id(-1){}
NonCollisionObject::~NonCollisionObject(){}

void NonCollisionObject::setPosition(vec2 pos){
	position.set(pos);
}
vec2& NonCollisionObject::getPosition(){
	return position;
}
void NonCollisionObject::setAngle(float a){
/*	a*=3.141592645f/180.0f;
	direction.set(cosf(a),sinf(a));
*/
	angle = a;
}

void NonCollisionObject::setDirection(const vec2&direction){
	angle = (atan2(direction.y,direction.x)*180.0f/3.141592645f);
}
vec2 NonCollisionObject::getDirection(){
	float a =angle*3.141592645f/180.0f;
	return vec2(cosf(a),sinf(a));
}
float NonCollisionObject::getAngle(){
	return angle;
}
Size& NonCollisionObject::getSize(){
	return size;
}
std::string& NonCollisionObject::getName(){
	return name;
}

//

CollisionObject::CollisionObject(vec2 position, Size size,std::string name)
:NonCollisionObject(position,size,name){
}
CollisionObject::~CollisionObject(){}

bool CollisionObject::checkCollision(CollisionObject&other){
	std::vector<vec2> c1 = getCorners();
	std::vector<vec2> c2 = other.getCorners();
	std::vector<vec2> axis = getAxis(c1,c2);
	vec2 mtv(100000,10000);
	for(int i = 0; i<axis.size();i++){
		float s1max = -100000,s1min = 100000,s2max = -100000,s2min = 100000;
		for(auto&c:c1){
			float scalar = axis[i].dot(c);//get scalar only
			if(s1max<scalar) s1max = scalar;
			if(s1min>scalar) s1min = scalar;
			
		}
		for(auto&c:c2){
			float scalar = axis[i].dot(c);//get scalar only
			if(s2max<scalar) s2max = scalar;
			if(s2min>scalar) s2min = scalar;
		}
		if(s2min>s1max||s2max<s1min)
			return false;
	}
	return true;
}
std::vector<vec2> CollisionObject::getCorners(){
	float rot = getAngle();
	std::vector<vec2>corners;
	vec2 direction = getDirection();
	vec2 normal(direction.y,-direction.x);
	corners.push_back(position-direction*size.w/2+normal*size.h/2);
	corners.push_back(position+direction*size.w/2+normal*size.h/2);
	corners.push_back(position+direction*size.w/2-normal*size.h/2);
	corners.push_back(position-direction*size.w/2-normal*size.h/2);
	return corners;
}
std::vector<vec2> CollisionObject::getAxis(std::vector<vec2>&c1,std::vector<vec2>&c2){
	std::vector<vec2>axis;
	axis.push_back((c1[1]-c1[0]).normalize());
	axis.push_back((c1[3]-c1[0]).normalize());
	axis.push_back((c2[1]-c2[0]).normalize());
	axis.push_back((c2[3]-c2[0]).normalize());
	return axis;
}


HardObject::HardObject(vec2 position, Size size,std::string name)
:CollisionObject(position,size,name){}
HardObject::~HardObject(){}

bool HardObject::collide(CollisionObject&other){
	std::vector<vec2> c1 = getCorners();
	std::vector<vec2> c2 = other.getCorners();
	std::vector<vec2> axis = getAxis(c1,c2);
	vec2 mtv(100000,10000);
	for(int i = 0; i<axis.size();i++){
		float s1max = -100000,s1min = 100000,s2max = -100000,s2min = 100000;
		for(auto&c:c1){
			float scalar = axis[i].dot(c);//get scalar only
			if(s1max<scalar) s1max = scalar;
			if(s1min>scalar) s1min = scalar;
			
		}
		for(auto&c:c2){
			float scalar = axis[i].dot(c);//get scalar only
			if(s2max<scalar) s2max = scalar;
			if(s2min>scalar) s2min = scalar;
		}
		if(s2min>s1max||s2max<s1min)
			return false;
		//at this point there's a collapse on this axis
		//now we find the collapse values
		//by using them we can find the min collapse value, and hence the vector of moving direction
		float collapse;
		if(s1max>s2max){
			collapse = s1min-s2max;//negative
		}else{
			collapse = s1max-s2min;//positive
		}
		if(fabsf(collapse)<mtv.length())
			mtv = axis[i]*collapse;
	}
	other.setPosition(other.getPosition()+mtv);
	return true;
}

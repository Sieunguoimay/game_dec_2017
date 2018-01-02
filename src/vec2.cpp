#include"vec2.h"

vec2::vec2(float x, float y):x(x),y(y){}
float vec2::dot(const vec2&v)const{
	return (x*v.x+y*v.y);
}
vec2 vec2::cross(const vec2&v)const{
	return vec2();
}
float vec2::length_squared()const{
	return (x*x+y*y);
}
float vec2::length() const{
	return sqrtf(length_squared());
}
vec2& vec2::normalize(){
	float l = length();
	if(l != 0)
		(*this)/=l;
	return *this;
}

void vec2::set(float x, float y){
	this->x = x;
	this->y = y;
}
void vec2::set(vec2 v){
	this->x = v.x;
	this->y = v.y;
}
vec2 vec2::getPerpendicular() const{
	return vec2(y,-x).normalize();
}

vec2 vec2::operator+(const vec2&v)const{
	return vec2(x+v.x,y+v.y);
}
vec2 vec2::operator-(const vec2&v)const{
	return vec2(x-v.x,y-v.y);
}
vec2 vec2::operator*(float n)const{
	return vec2(x*n,y*n);
}
vec2 vec2::operator/(float n)const{
	if(n!=0)
		return vec2(x/n,y/n);
	else 
		return vec2();
}
vec2&vec2::operator+=(const vec2&v){
	x+=v.x;
	y+=v.y;
	return (*this);
}
vec2&vec2::operator-=(const vec2&v){
	x-=v.x;
	y-=v.y;
	return (*this);
}
vec2&vec2::operator*=(float n){
	x*=n;
	y*=n;
	return (*this);
}
vec2&vec2::operator/=(float n){
	if(n != 0){
		x/=n;
		y/=n;
	}
	return (*this);
}

bool vec2::operator==(const vec2&v){
	return (x==v.x&&y==v.y);
}
bool vec2::operator!=(const vec2&v){
	return (x!=v.x||y!=v.y);
}

std::ostream&operator<<(std::ostream&os, const vec2&v){
	os<<v.x<<" "<<v.y;
	return os;
}


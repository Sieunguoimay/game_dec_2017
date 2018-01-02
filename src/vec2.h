#ifndef VEC2_H
#define VEC2_H
#include<iostream>
#include<cmath>
#include<sstream>

struct Size{
	int w, h;
	Size(int w = 0, int h = 0):w(w),h(h){}	
	void set(int w, int h){
		this->w = w;
		this->h = h;
	}
	void set(Size&s){
		w = s.w;
		h = s.h;
	}
};
class vec2{
public:
	float x, y;
	
	vec2(float x = 0.0f, float y = 0.0f);
	float dot(const vec2&v)const;
	vec2 cross(const vec2&v)const;
	float length()const;
	float length_squared()const;
	vec2& normalize();
	void set(float x, float y);
	void set(vec2 v);
	vec2 getPerpendicular() const;
	vec2 operator+(const vec2&v)const;
	vec2 operator-(const vec2&v)const;
	vec2 operator*(float n)const;
	vec2 operator/(float n)const;
	vec2&operator+=(const vec2&v);
	vec2&operator-=(const vec2&v);
	vec2&operator*=(float n);
	vec2&operator/=(float n);

	bool operator==(const vec2&v);
	bool operator!=(const vec2&v);

	friend std::ostream&operator<<(std::ostream&os, const vec2&v);
	
};
#endif//VEC2_H

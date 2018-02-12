#include"geometry.h"
vec2 projection(const vec2&v1, const vec2&v2){
	float v2_ls = v2.length_squared();
	if(v2_ls>0.000005f)
		return v2*(v2.dot(v1)/v2_ls);
	else 
		return vec2(0,0);
}

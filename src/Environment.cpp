#include"Environment.h"
Environment::Environment(){

}
Environment::~Environment(){
	for(int i = 0; i<insideBricks.size();i++)
		delete insideBricks[i];
	for(int i = 0; i<wallBricks.size();i++)
		delete wallBricks[i];
	for(int i = 0; i<outsideBricks.size();i++)
		delete outsideBricks[i];

}
void Environment::putBricks(MapMatrix&map,Size size){

	for(int i = 0;i<map.size();i++){
		for(int j = 0; j<map[i].size();j++){
			if(map[i][j]==0)
				insideBricks.push_back(new NonCollisionObject(vec2(j*size.w,i*size.h),size));
			else if(map[i][j]==1)
				wallBricks.push_back(new HardObject(vec2(j*size.w,i*size.h),size));
			else if(map[i][j]==2)
				outsideBricks.push_back(new NonCollisionObject(vec2(j*size.w,i*size.h),size));
		}
	}
}
void Environment::draw(Assets&assets){
	for(int i = 0; i<insideBricks.size();i++)
		assets.getTexture("insideBrick").render(insideBricks[i]->getPosition(),insideBricks[i]->getSize());
	for(int i = 0; i<wallBricks.size();i++)
		assets.getTexture("wallBrick").render(wallBricks[i]->getPosition(),wallBricks[i]->getSize());
	for(int i = 0; i<outsideBricks.size();i++)
		assets.getTexture("outsideBrick").render(outsideBricks[i]->getPosition(),outsideBricks[i]->getSize());
}
void Environment::wallCollide(std::vector<CollisionObject*>&objects){
	for(int i = 0; i<wallBricks.size();i++)
		for(int j = 0; j<objects.size();j++)
		wallBricks[i]->collide(*objects[j]);
}
void Environment::wallCollide(CollisionObject&object){//overloading
	for(int i = 0; i<wallBricks.size();i++)
		wallBricks[i]->collide(object);

}
std::vector<NonCollisionObject*>& Environment::getInsideWallBricks(){
	return insideBricks;
}

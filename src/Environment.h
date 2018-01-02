#ifndef ENVIRONMENT
#define ENVIRONMENT
#include"game_object.h"
#include"Assets.h"
class Environment{
	std::vector<HardObject*>wallBricks;
	std::vector<NonCollisionObject*>outsideBricks;
	std::vector<NonCollisionObject*>insideBricks;
public:
	Environment();
	~Environment();
	void putBricks(MapMatrix&map,Size size = Size(100,100));
	void draw(Assets&assets);
	void wallCollide(std::vector<CollisionObject*>&objects);
	void wallCollide(CollisionObject&object);//overloading
	std::vector<NonCollisionObject*>& getInsideWallBricks();
};
#endif //ENVIRONMENT
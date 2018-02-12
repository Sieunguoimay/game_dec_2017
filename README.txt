28/12/2017
I am starting a new project, a 2D action Game
which is about a watermelo fighting again all kinds of vegetatble on the field

the stages:
	1- Setup Programming Environment
	2- Work on Game basically
	3- Network implementation - basic setup
	4- Menu on console
	5- Work on Game extensionally
	6- Menu and other alert windows
	7- Advanced and whole game adjusts
	8-...
what gonna do for the stage 1 - Setup Programming Environment:
-setup SDL SDL_image, Box2D, SDL_mix
-vector2d instead of using b2vec2 . use a function to convert between vector2d and b2vec2 when neccessary
-Texture,Sound,Text,Assets - loading and all, Body Creating functions
(using std::vector to pack all the objects)


30/12/2017
	- in windows dir /b >> names.txt
	- in ubuntu printf '%s\n' * >> names.txt
2/1/2017
	Uploaded onto Github - available online access :))
	g++ src/*.cpp -o bin/main -IE:/CodingSpace/Libraries/SDL_HOME/include -LE:/CodingSpace/Libraries/SDL_HOME/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

g++ src/*.o src/reusable/*.o -o bin/main -LE:/CodingSpace/Libraries/SDL_HOME/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net

g++ -c ClientNetwork.cpp -IE:/CodingSpace/Libraries/SDL_HOME/include
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


class Actor : public GraphObject {
public:
	
	Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth);



};

class Iceman : public Actor {
public:
	Iceman( int startX, int startY, Direction startDirection, float size, unsigned int depth);
	void doSomething();
private:
	int i_health;
	int i_squirt;
	int i_sonarCharge;
	int i_gold_sack ;
	Direction i_facing;
};
//
//class Protester : public Actor {
//public:
//	Protester(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);
//	
//};
//
//class HardcoreProtester : public Actor {
//public:
//	HardcoreProtester(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);
//};
//
//
//class Collectable : public GraphObject {
//
//	Collectable(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0);
//};



// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_

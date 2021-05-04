#include "Actor.h"
#include "StudentWorld.h"


/*
Creating the constructors of the Actor, potestor that inherits from actor, Iceman inherits from Actor. same goes for protester and
HardcoreProtester.
*/

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0): 
	GraphObject(imageID, startX, startY, startDirection, size, depth)
{
	setVisible(true);
	

}

//Collectable::Collectable(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0) :
//	GraphObject(imageID, startX, startY, startDirection, size, depth)
//{
//
//}

Iceman::Iceman( int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0):
 Actor( IID_PLAYER, 30, 60, startDirection, size, depth), i_health(10), i_squirt(5), i_sonarCharge (1),
 i_gold_sack (0), i_facing (right) {}

void Iceman::doSomething()
{
	if (i_health == 0) // this signifies if the Iceman is dead.
	{
		return;
	}


}


//Protester::Protester(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0):
//	Actor(imageID, startX, startY, startDirection, size, depth) {
//
//}
//
//HardcoreProtester::HardcoreProtester(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0) :
//	Actor(imageID, startX, startY, startDirection, size, depth) {
//
//}

// thee is still more to declare such as ice, boulders, etc...

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

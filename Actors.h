#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

//--------------------------------------------------------Actor-----------------------------------------------------------------------------

class Actor : public GraphObject {
public:

	Actor(int imageID, int startX, int startY, Direction startDirection, StudentWorld* world, float size, unsigned int depth);
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
	virtual ~Actor() {};
private:
	StudentWorld* sw;
};

//--------------------------------------------------------Iceman-----------------------------------------------------------------------------

class Iceman : public Actor {
public:
	Iceman(int startX, int startY, Direction startDirection, StudentWorld* world, float size, unsigned int depth);
	void doSomething();
	bool isAlive();
private:
	int i_health;
	int i_squirt;
	int i_sonarCharge;
	int i_gold_sack;
	Direction i_facing;
};

//--------------------------------------------------------Ice-----------------------------------------------------------------------------

class Ice : public Actor {
public:
	Ice(int x, int y, StudentWorld* world) :
		Actor(IID_ICE, x, y, right, world, .25, 3) {}
	void doSomething() { /*does nothing*/ }
};


class Boulder :public Actor
{
public:
	Boulder(int x, int y, StudentWorld* world):
	Actor(IID_BOULDER, x, y, down, world, 1, 1.0), Alive(true) {
		setVisible(true);
	}

	void doSomething();
	bool isAlive();
	void drop();


private:
	bool Alive;
};
#endif

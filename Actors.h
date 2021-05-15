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
	virtual bool isAlive();
	virtual void setAliveStatus(bool status);
	virtual ~Actor() {};
private:
	StudentWorld* sw;
	bool alive;
};

//--------------------------------------------------------Iceman-----------------------------------------------------------------------------

class Iceman : public Actor {
public:
	Iceman(int startX, int startY, Direction startDirection, StudentWorld* world, float size, unsigned int depth);
	void doSomething();
	bool isAlive();
	void damage(int damage);
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

//--------------------------------------------------------Boulder-----------------------------------------------------------------------------

class Boulder :public Actor
{
public:
	Boulder(int x, int y, StudentWorld* world) :
		Actor(IID_BOULDER, x, y, down, world, 1, 1.0) {
		setVisible(true);
	}

	void doSomething();
	void drop();


private:

};


//-------------------------------------------------------Oil Barrel--------------------------------------------------------------------------


class Oil_Barrel : public Actor
{
public:
	Oil_Barrel(int x, int y, StudentWorld* world) :
		Actor(IID_BARREL, x, y, right, world, 1.0, 2), Alive(true) {
		setVisible(false);
	}

	virtual bool isAlive();
	void doSomething();
	void ProximityCheck();
	void EraseOil();

	// barrel needs to reward player a.k.a iceman 1000 points for picking up oil.
private:

	bool Alive;
};



#endif

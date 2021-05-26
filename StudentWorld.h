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

	virtual void search();


	//--------------------making damage a base class here for protesters and iceman ---------
	virtual void damage();
	//---------------------------------------------------------------------------------------



	virtual ~Actor() {};
private:
	StudentWorld* sw;
	bool alive;
};


//--------------------------------------------------------Regular Protester-----------------------------------------------------------------------------


class Regular_protester : public Actor {
public:
	Regular_protester(int x, int y, StudentWorld* world):
		Actor (IID_PROTESTER, x, y, left, world, 1.0, 0), delay(15), cooldown(1)
	{
		setVisible(true);
	}
	void damage(int damage); // this damage function right now is only intended to harm the iceman. so protesters hurt iceman

	void coolDown();
	void resetcoolDown(int x);

	void doSomething();

private:
	int cooldown; // this is a state variable that will regulate the delay. this will trigger the delay. 1 its ready to shout. 2 its not ready to shoot.
	int delay; // this is to make the protester wait 15 ticks so it can shout again.

};


//--------------------------------------------------------Iceman-----------------------------------------------------------------------------

class Iceman : public Actor {
public:
	Iceman(int startX, int startY, Direction startDirection, StudentWorld* world, float size, unsigned int depth);
	void doSomething();
	bool isAlive();

	void damage(int damage);
	int getHealth() const;

	int getNumGold() const;
	void addGold(int g);

	int getNumSquirt() const;
	void addWtr(int w);

	int getNumSonar() const;
	void addSonar(int s);


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
		Actor(IID_BOULDER, x, y, down, world, 1, 1.0), delay(30) {
		setVisible(true);
	}

	void doSomething();
	void drop();


private:
	int delay;
};


//-------------------------------------------------------Oil Barrel--------------------------------------------------------------------------


class Oil_Barrel : public Actor
{
public:
	Oil_Barrel(int x, int y, StudentWorld* world) :
		Actor(IID_BARREL, x, y, right, world, 1.0, 2) {
		setVisible(false);
	}

	void search();
	void doSomething();
	void ProximityCheck();
	void EraseOil();

	// barrel needs to reward player a.k.a iceman 1000 points for picking up oil.
private:

};



//-------------------------------------------------------Gold Nugget--------------------------------------------------------------------------

class Gold : public Actor
{
public:
	Gold(int x, int y, StudentWorld* world, int s) :
		Actor(IID_GOLD, x, y, right, world, 1.0, 2), state(s), delay(30) {
		if (state == 1)
			setVisible(false);
		else if (state == 0)
			setVisible(true);

		setAliveStatus(true);
	}
	
	void search();

	void doSomething();
	bool ProximityCheck(int prox, Actor* a);

private:
	int state; // 0 - dropped 1 - spawned
	int delay;
};

class Sonar : public Actor {
public:

	Sonar(int x, int y, StudentWorld* world, int s) :
		Actor(IID_SONAR, x, y, right, world, 1.0, 2), state(s), delay(getspan()) {
		setVisible(true);
		setAliveStatus(true);

		/*
		this is not correct. come back to or it is currently under investigation.

		problem is that the sonar kits are spawning right off the bat and they must not be that way.
		they should only spawn atfer a 1 and g chance of ticks by the expression below:

		int G = current_level_number * 25 + 300 found on page 21.
		
		*/
	}
	
	void doSomething();
	void ProximityCheck();
	int getspan();

private:
	int state; // 1 alive, 0 dead
	int delay;

};




#endif

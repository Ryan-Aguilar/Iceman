#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <vector>



// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), p1(nullptr)
	{
	}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	void destroyIce(int x, int y);
	//sets any ice object that comes in contact with the Iceman to not visible

	bool checkIce(int x, int y);
	//returns true if there is ice at the location parameter

	Iceman* getPlayer();
	void addActor(Actor* a);
	void setDisplayText();

	int getOilLeft() const { return numOilLeft; }
	void updateOilLeft(int o) { numOilLeft -= o; };

	int getSonarLeft() const { return numSonarLeft; }  // ryan made the getter and setter function with same implementation that
	                                                   // Andrew designed. Only thing done is I changed variable and name.
	int SonarLifeSpan();
	int chance();

	int countsonar(int x);
	void updateSonarLeft(int o);


	~StudentWorld();
private:
	std::vector<Actor*> actors; // part A
	Iceman* p1;
	Ice* iceField[64][64]{ nullptr };
	int numOilLeft;
	int numSonarLeft;
};

#endif // STUDENTWORLD_H_

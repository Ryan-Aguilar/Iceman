#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <array>



// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), p1(nullptr), ticksPassed(0)
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

	int getSonarLeft() const { return numSonarLeft; }
	int updateSonarLeft(int x);
	// (+) to increment and (-) to decrement

	bool protesterGold(int x, int y);

	void call();
	void damagecall(std::string s1, std::string who, int x, int y);

	void shortestpath(int startX, int startY, int x, int y, Actor* actor);

	void checkocupancy();

	~StudentWorld();
private:
	std::vector<Actor*> actors; // part A
	Iceman* p1;
	Ice* iceField[64][64]{ nullptr };

	// finding the optimal path
	int pathfield[64][64] = { -1 };


	int numOilLeft;
	int numSonarLeft;
	int ticksPassed;
};


/*


*/
#endif // STUDENTWORLD_H_

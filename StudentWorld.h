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

	~StudentWorld();
private:
	std::vector<Actor*> actors; // part A
	Iceman* p1;
	Ice* iceField[64][64];

};

#endif // STUDENTWORLD_H_

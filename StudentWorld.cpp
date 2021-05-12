#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init()
{
	p1 = new Iceman(30, 60, GraphObject::Direction::right, this, 1, 0); // the iceman

	int rows = 64;
	int columns = 64;


	for (int y = 0; y < rows - 4; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			iceField[x][y] = new Ice(x, y, this);
		}
	}

	for (int y = 0; y < rows - 4; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			if (x >= 30 && x < 34 && y > 3)
				iceField[x][y]->setVisible(false);
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.



	p1->doSomething();

	decLives();
	return GWSTATUS_CONTINUE_GAME; //return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
	delete p1; //delete the Iceman

	for (auto a : actors)
	{
		delete a; //delete all other actors
	}

	for (int y = 0; y < 64; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			if (iceField[x][y] != nullptr)
				delete iceField[x][y];
		}
	}
}

void StudentWorld::destroyIce(int x, int y)
{
	if (iceField[x][y] != nullptr && iceField[x][y]->isVisible())
	{
		playSound(SOUND_DIG);
		iceField[x][y]->setVisible(false);
	}
}

StudentWorld::~StudentWorld()
{
	delete p1; //delete the Iceman

	for (auto a : actors)
	{
		delete a; //delete all other actors
	}

	for (int y = 0; y < 64; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			if (iceField[x][y] != nullptr)
				delete iceField[x][y];
		}
	}
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

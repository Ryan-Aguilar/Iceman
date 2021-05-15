#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init()
{
	//Create the player character - Iceman
	p1 = new Iceman(30, 60, GraphObject::Direction::right, this, 1, 0); // the iceman

	//create an ice field
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

	//add all other actors

	int numBoulders = min(((int)getLevel() / 2) + 2, 9);
	int numGold = max((5 - (int)getLevel()) / 2, 2);
	int numOil = min((2 + (int)getLevel()), 21);

	for (int i = 0; i < numBoulders; i++)
	{
		int tempX{};
		int tempY{};
		tempX = rand() % 60;
		tempY = rand() % 40 + 16;

		actors.push_back(new Oil_Barrel(20,60, this)); // should be tempX and tempY
		actors.push_back(new Boulder(tempX, tempY, this));

		for (int x = tempX; x < tempX + 4; x++)
		{
			for (int y = tempY; y < tempY + 4; y++)
			{
				if (iceField[x][y]->isVisible())
				{
					iceField[x][y]->setVisible(false);
				}
			}
		}
	}
	for (int i = 0; i < numGold; i++)
	{

	}
	for (int i = 0; i < numOil; i++)
	{

	}

	//actors.push_back(new Boulder(50, 56, this));

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	setGameStatText("Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000 tick: ");

	if (!(p1->isAlive()))
	{
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}
	//check which actors are dead and remove them from the game
	for (auto a : actors)
	{

	}

	p1->doSomething();

	for (auto a : actors)
	{
		a->doSomething();
	}

	decLives();
	return GWSTATUS_CONTINUE_GAME; //return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
	delete p1; //delete the Iceman

	for (auto a : actors)
	{
		delete a; //delete all other actors
		actors.pop_back();
	}

	for (int y = 0; y < 64; y++) //delete the ice field
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

bool StudentWorld::checkIce(int x, int y)
{
	if (iceField[x][y]->isVisible())
		return true;

	return false;
}

Iceman* StudentWorld::getPlayer()
{
	return p1;
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

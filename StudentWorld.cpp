#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::countsonar(int x) {
	return numSonarLeft += x;
}

void StudentWorld::call()
{
	for (auto z : actors)
	{
		z->search();
	}

}

int StudentWorld::SonarLifeSpan()
{
	return max(100, 300 - 10 * (int)getLevel());
}

int StudentWorld::init()
{
	//Create the player character - Iceman
	p1 = new Iceman(30, 60, GraphObject::Direction::right, this, 1, 0);

	
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
	numOilLeft = numOil;
	//int numSon = 1;//max(100, 300 - 10 * (int)getLevel());

	for (int i = 0; i < numBoulders; i++) //add boulders
	{
		int tempX{};
		int tempY{};
		tempX = rand() % 60;
		tempY = rand() % 40 + 16;

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
	for (int i = 0; i < numGold; i++) // add gold
	{
		int tempX{};
		int tempY{};
		tempX = rand() % 60;
		tempY = rand() % 60;

		actors.push_back(new Oil_Barrel(tempX, tempY, this));

	}
	for (int i = 0; i < numOil; i++) // add oil
	{
		int tempX{};
		int tempY{};
		tempX = rand() % 60;
		tempY = rand() % 60;

		actors.push_back(new Gold(tempX, tempY, this, 1));
	}
	

	// -------------------------sonar spawn below---------------------------
	/*
	here there is no specific amount of sonars that will spawn. it is all based up chance. which is determined below.
	
	*/

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::chance() {
	return getLevel() * 25 + 300;
}
/*

int tick = 30;

for (int x =0; x<tick; x++)
{
}
*/

//WORK IN PROGRESS
void StudentWorld::setDisplayText()
{
	int level = getLevel() + 1;
	int lives = getLives();
	int health = p1->getHealth();
	int squirts = p1->getNumSquirt();
	int gold = p1->getNumGold();
	int barrelsLeft = numOilLeft;
	int sonar = p1->getNumSonar();
	int score = getScore();



	string newDisplay = (string("Lvl: ") + to_string(level) + string(" Lives: ") + to_string(lives) + string(" Hlth: ") + to_string(health * 10) + "% "
		+ string(" Wtr: ") + to_string(squirts) + string(" Gld: ") + to_string(gold) + string(" Oil Left: ") + to_string(barrelsLeft)
		+ string(" Sonar: ") + to_string(sonar) + string(" Score: ") + to_string(score));

	setGameStatText(newDisplay);
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	setDisplayText();

	if (!(p1->isAlive()))
	{
		decLives();
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}
	else if (getOilLeft() == 0)
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}


	////check which actors are dead and remove them from the game
	actors.erase(remove_if(actors.begin(), actors.end(), [](auto a) {return !(a->isAlive()); }), actors.end());

	p1->doSomething();

	for (auto a : actors)
	{
		a->doSomething();
	}


	// testing

	int g = chance();
	int spawn = rand() % (g + 1);

	/*
	limit the number of sonars that can be in the map. so make a function that counts the total number of sonars on the map

	1 sonar at time.
	
	*/

	if (spawn == g ) 
	{
		if (numSonarLeft == 0) 
		{
			actors.push_back(new Sonar(0, 60, this, 1));
			countsonar(1);
			
		}

	}


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

void StudentWorld::addActor(Actor* a)
{
	actors.push_back(a);
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


void StudentWorld::updateSonarLeft(int o)
{
	if (o < 0)
	{
		numSonarLeft -= o;
	}
	else 
	{
		numSonarLeft += o;
	}
}




// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

#include "StudentWorld.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <ctime>
#include <queue>

using namespace std;

class coordinate {
public:
	coordinate(int x, int y): xpoint(x),ypoint(y) {}
	int xpoint;
	int ypoint;
};

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init()
{
	//Create the player character - Iceman
	p1 = new Iceman(this);

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
	for (int i = 0; i < numOil; i++) // add oil
	{
		int tempX{};
		int tempY{};
		tempX = rand() % 60;
		tempY = rand() % 60;

		actors.push_back(new Oil_Barrel(tempX, tempY, this));

	}
	for (int i = 0; i < numGold; i++) // add gold
	{
		int tempX{};
		int tempY{};
		tempX = rand() % 60;
		tempY = rand() % 60;

		actors.push_back(new Gold(tempX, tempY, this, 1));
	}

	actors.push_back(new Regular_protester(60, 60, this));

	actors.push_back(new Sonar(0, 60, this));

	//actors.push_back(new Gold(45, 60, this, 0));

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setDisplayText()
{
	int level = getLevel() + 1; // 2 digits
	int lives = getLives(); // 1 digit
	int health = p1->getHealth(); // 3 digits : _75% or 100%
	int squirts = p1->getNumSquirt(); // 2 digits
	int gold = p1->getNumGold(); // 2 digits
	int barrelsLeft = numOilLeft; // 2 digits
	int sonar = p1->getNumSonar(); // 2 digits
	int score = getScore(); // 6 digits with leading zeros, not leading spaces

	ostringstream oss; //for formatting leading spaces

	oss << "Level:" << oss.fill(' ') << setw(2) << level;
	oss << " Lives:" << oss.fill(' ') << lives;
	oss << " Hlth:" << oss.fill(' ') << setw(3) << (health * 10) << "%";
	oss << " Wtr:" << oss.fill(' ') << setw(2) << squirts;
	oss << " Gld:" << oss.fill(' ') << setw(2) << gold;
	oss << " Oil Left:" << oss.fill(' ') << setw(2) << barrelsLeft;
	oss << " Sonar:" << oss.fill(' ') << setw(2) << sonar;
	oss << " Score:" << oss.fill('0') << setw(6) << score;

	string newDisplay = oss.str();


	setGameStatText(newDisplay);
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	ticksPassed++;

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

	//Add water and sonar to the game
	int randomChance = rand() % ((25 * getLevel()) + 300); // 1 in int G = current_level_number * 25 + 300
	if (randomChance == 1)
	{
		int random = rand() % 5;
		if (random == 1)
		{
			actors.push_back(new Sonar(0, 60, this));
		}
		else
		{
			int tempX{};
			int tempY{};
			tempX = rand() % 60;
			tempY = rand() % 60;

			actors.push_back(new Water(tempX, tempY, this));
		}
	}

	int spawnProtester = std::max(25, 200 - (int)getLevel());

	if (ticksPassed % spawnProtester == 0)
	{
		int numProtester = 0;
		for (auto a : actors)
		{
			if (a->type() == "RegularProtester" || a->type() == "HardcoreProtester")
			{
				numProtester++;
			}
		}

		if (numProtester < std::min(15, 2 + (int)(getLevel() * 1.5)))
		{
			int probOfHardcore = std::min(90, (int)getLevel() * 10 + 30);
			int random = rand() % probOfHardcore;
			if (random == 1)
			{
				//spawn hardcore
			}
			else
				addActor(new Regular_protester(60, 60, this));
		}
	}


	//Do Something
	p1->doSomething();

	for (auto a : actors)
	{
		a->doSomething();
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
	if (iceField[x][y] != nullptr)
	{
		if (iceField[x][y]->isVisible())
			return true;
	}

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

//Sonar Functions
int StudentWorld::updateSonarLeft(int x) {
	return numSonarLeft += x;
}

void StudentWorld::call()
{
	for (auto z : actors)
	{
		z->sonarSearch();
	}

}

bool StudentWorld::protesterGold(int x, int y)
{

	for (auto a : actors)
	{
		int ax = a->getX();
		int ay = a->getY();
		if (a->type() == "RegularProtester")
		{
			if (x <= ax + 3 && x >= ax - 3)
			{
				if (y <= ay + 3 && y >= ay - 3)
				{
					a->gotGold();
					return true;
				}
			}
		}
	}

	return false;
}

void StudentWorld::checkocupancy()
{

	for (auto z : actors)
	{
		std::string s = z->type();

			if (s == "Boulder")
			{
				int A = z->getX();
				int B = z->getY();

				for (int f = A; A < A + 4; A++)
				{
					for (int g = B; g < g + 4; g++)
					{
						pathfield[f][g] = -2;
					}
				}
			}
	}


	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 64; y++)
		{

			if (checkIce(x, y)) 
			{
				pathfield[x][y] = -2;
			}
		}
	}
}



//Squirt Functions

void StudentWorld::damagecall(std::string punisher, std::string who, int x, int y)
{
	for (auto z : actors)
	{
		std::string s = z->type();
		int ax = z->getX();
		int ay = z->getY();



		if (s == who && punisher == "Squirt")
		{
			if (x <= ax + 6 && x >= ax - 6)
			{
				if (y <= ay + 6 && y >= ay - 6)
				{
					std::string hit = "hit";
					z->damageintake(2);
				}
			}
		}
		else if (s == who && punisher == "Boulder")
		{
			if (x <= ax + 3 && x >= ax - 3)
			{
				if (y == ay + 3)
				{
					std::string hit = "hit";
					z->damageintake(100);
					increaseScore(500);
				}
			}
		}
	}
	//std::string g = "missed";
	return;
}

//Destructor
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


void StudentWorld::shortestpath(int startX, int startY, int x, int y, Actor* actor) // this can possibly be used in three senarios, 2 for finding an exit and then the protester to chase iceman.
{

	checkocupancy();


	/*
		where fbs starts
		x= 60
		y= 60
		
		int startX has the protesters getX().

		int startY has the protesters getY();
		
		
		
		do breath first search from the destination at (60,60)
		
		push the coordinate onto the queue.

		check if what has been pushed is the protester's location.

		if it is not, push up, down, left and right onto the queue.

		now we would have a local variable that keeps track of the distance from the destination to the protester.

		if what we push is out of bounds, then we can just continue and ignore those spots.




		if it has a different number than -1 then we know it is visited.

		check if there is a boulder or ice in its path.




	*/

	int distance = 0;
	
	queue <coordinate*> path;

	coordinate* temp = new coordinate(x, y);

	path.push(temp);

	while (!path.empty())
	{
		int CX = temp->xpoint;
		int CY = temp->ypoint;
		if (CX != startX && CY != startY)
		{
			pathfield[CX][CY] = distance;
			 // this is to make sure that the search stays within the 64x64 field. 
			
				if (CX <= 64)
				{
					if (pathfield[CX + 1][CY] == -1) 
					{
						path.push(new coordinate(CX+ 1, y)); //right
						pathfield[CX + 1][CY] = pathfield[CX][CY] + 1;
					}
				}
				if (CX >= 0)
				{
					path.push(new coordinate(CX - 1, y)); // left
					pathfield[CX - 1][CY] = pathfield[CX][CY] + 1;
				}
				if (CY <= 64)
				{
					path.push(new coordinate(CX, y + 1)); // up
					pathfield[CX][CY + 1] = pathfield[CX][CY] + 1;
				}
				if (CY >= 0)
				{
					path.push(new coordinate(CX, y - 1)); // down
					pathfield[CX][CY - 1] = pathfield[CX][CY] + 1;
				}

			

		}
		distance++;
		path.pop();
	}


	//--------------------------------movement-----------------------

	if (pathfield[startX + 1][startY] == -1 && startX < 64 && startX >= 0)
	{
		actor->moveTo(startX + 1, startY);
	}
	else if (pathfield[startX - 1][startY] == -1 && startX < 64 && startX >= 0)
	{
		actor->moveTo(startX - 1, startY);
	}
	else if (pathfield[startX][startY + 1] == -1 && startY < 64 && startY >= 0)
	{
		actor->moveTo(startX, startY + 1);
	}
	else if (pathfield[startX][startY - 1] == -1 && startY < 64 && startY >= 0)
	{
		actor->moveTo(startX - 1, startY);
	}






}



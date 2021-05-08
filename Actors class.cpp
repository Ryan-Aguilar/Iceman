#include "Actor.h"
#include "StudentWorld.h"


/*
Creating the constructors of the Actor, potestor that inherits from actor, Iceman inherits from Actor. same goes for protester and
HardcoreProtester.
*/

//--------------------------------------------------------Actor-----------------------------------------------------------------------------

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, StudentWorld* world, float size = 1.0, unsigned int depth = 0) :
	GraphObject(imageID, startX, startY, startDirection, size, depth), sw(world)  // we did not add a studentworld pointer in the graph object constructor
	// because the graphobject ctor does not need one. 
{
	setVisible(true);
}

StudentWorld* Actor::getWorld() const
{
	return sw;
}

//--------------------------------------------------------Iceman-----------------------------------------------------------------------------

Iceman::Iceman(int startX, int startY, Direction startDirection, StudentWorld* world, float size = 1.0, unsigned int depth = 0) :
	Actor(IID_PLAYER, 30, 60, startDirection, world, size, depth), i_health(10), i_squirt(5), i_sonarCharge(1),
	i_gold_sack(0), i_facing(right) {}

bool Iceman::isAlive()
{
	return i_health > 0;
}

void Iceman::doSomething()
{
	if (i_health == 0) // this signifies if the Iceman is dead.
	{
		return;
	}

	int icemanX = getX();
	int icemanY = getY();

	for (int x = icemanX; x < icemanX + 4; x++)
	{
		for (int y = icemanY; y < icemanY + 4; y++)
		{
			getWorld()->destroyIce(x, y);
		}
	}


	/*
	What to do is we want to make our own variabe that has the same number as the constants for key_press
	
	
	*/


	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		switch (ch)
		{
		case KEY_PRESS_UP:
			if (getY() == 60) break;
			setDirection(up);
			this->moveTo(getX(), getY() + 1); break;
		case 8:
			if (getY() == 60) break;
			setDirection(up);
			this->moveTo(getX(), getY() + 1); break;
		case 'W':
			if (getY() == 60) break;
			setDirection(up);
			this->moveTo(getX(), getY() + 1); break;
		case 'w' :
			if (getY() == 60) break;
			setDirection(up);
			this->moveTo(getX(), getY() + 1); break;

//-----------------------------------------------------------
		
		case 'd':
			if (getX() == 60) break;
			setDirection(right);
			this->moveTo(getX() + 1, getY()); break;
		case 'D':
			if (getX() == 60) break;
			setDirection(right);
			this->moveTo(getX() + 1, getY()); break;
		case 6:
			if (getX() == 60) break;
			setDirection(right);
			this->moveTo(getX() + 1, getY()); break;
		case KEY_PRESS_RIGHT:
			if (getX() == 60) break;
			setDirection(right);
			this->moveTo(getX() + 1, getY()); break;
//-----------------------------------------------------------
		case KEY_PRESS_LEFT:
			if (getX() == 0) break;
			setDirection(left);
			this->moveTo(getX() - 1, getY()); break;
		case 'a':
			if (getX() == 0) break;
			setDirection(left);
			this->moveTo(getX() - 1, getY()); break;
		case 'A':
			if (getX() == 0) break;
			setDirection(left);
			this->moveTo(getX() - 1, getY()); break;
		case 4:
			if (getX() == 0) break;
			setDirection(left);
			this->moveTo(getX() - 1, getY()); break;
//-----------------------------------------------------------
		case KEY_PRESS_DOWN:
			if (getY() == 0) break;
			setDirection(down);
			this->moveTo(getX(), getY() - 1); break;
		case 's':
			if (getY() == 0) break;
			setDirection(down);
			this->moveTo(getX(), getY() - 1); break;
		case 'S':
			if (getY() == 0) break;
			setDirection(down);
			this->moveTo(getX(), getY() - 1); break;
		case 2:
			if (getY() == 0) break;
			setDirection(down);
			this->moveTo(getX(), getY() - 1); break;
		}
	}

}

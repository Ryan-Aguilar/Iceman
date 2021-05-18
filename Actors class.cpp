#include "Actor.h"
#include "StudentWorld.h"


/*
Creating the constructors of the Actor, potestor that inherits from actor, Iceman inherits from Actor. same goes for protester and
HardcoreProtester.
*/

//--------------------------------------------------------Actor-----------------------------------------------------------------------------

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, StudentWorld* world, float size = 1.0, unsigned int depth = 0) : alive(true),
GraphObject(imageID, startX, startY, startDirection, size, depth), sw(world)  // we did not add a studentworld pointer in the graph object constructor
// because the graphobject ctor does not need one. 
{
	setVisible(true);
}

StudentWorld* Actor::getWorld() const
{
	return sw;
}

bool Actor::isAlive()
{
	return alive;
}

void Actor::setAliveStatus(bool status)
{
	alive = status;
	if (alive == false)
	{
		setVisible(false);
	}
}

//--------------------------------------------------------Iceman-----------------------------------------------------------------------------

Iceman::Iceman(int startX, int startY, Direction startDirection, StudentWorld* world, float size = 1.0, unsigned int depth = 0) :
	Actor(IID_PLAYER, 30, 60, startDirection, world, size, depth), i_health(10), i_squirt(5), i_sonarCharge(1),
	i_gold_sack(0), i_facing(right), i_points(0) {}

bool Iceman::isAlive()
{
	return i_health > 0;
}

void Iceman::damage(int damage)
{
	i_health -= damage;
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


	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		switch (ch)
		{
		case KEY_PRESS_RIGHT:
			if (getX() == 60) break;
			setDirection(right);
			this->moveTo(getX() + 1, getY()); break;
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
		case KEY_PRESS_UP:
			if (getY() == 60) break;
			setDirection(up);
			this->moveTo(getX(), getY() + 1); break;
		case 'w':
			if (getY() == 60) break;
			setDirection(up);
			this->moveTo(getX(), getY() + 1); break;
		case 'W':
			if (getY() == 60) break;
			setDirection(up);
			this->moveTo(getX(), getY() + 1); break;
		case 8:
			if (getY() == 60) break;
			setDirection(up);
			this->moveTo(getX(), getY() + 1); break;
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


int Iceman::AccumulatePoints(int points)
{
	return i_points += points;
}

//--------------------------------------------------------Boulder-----------------------------------------------------------------------------





void Boulder::drop()
{
	Iceman* player = getWorld()->getPlayer();

	int bX = getX();
	int bY = getY();

	int pX = player->getX();
	int pY = player->getY();

	for (int x1 = bX; x1 < bX + 4; x1++)
	{
		for (int x2 = pX; x2 < pX + 4; x2++)
		{
			if (x1 == x2 && bY == (pY + 3))
			{
				player->damage(100);
				break;
			}

			if (getWorld()->checkIce(x1, bY - 2))
			{
				setAliveStatus(false);
			}

			// if it comes into contact with protestor, cause 100 damage and keep falling
		}
	}


	getWorld()->playSound(SOUND_FALLING_ROCK);
	this->moveTo(getX(), getY() - 1);
}

void Boulder::doSomething()
{
	int bX = getX();
	int bY = getY();

	if (bY == 0)
		setAliveStatus(false);

	if (!isAlive())
	{
		return;
	}


	//check if in contact with Protestors, Iceman



	bool canDrop = false;
	for (int x = bX; x < bX + 4; x++)
	{

		if (getWorld()->checkIce(x, bY - 1))
		{
			canDrop = false;
			break;
		}

		canDrop = true;
	}

	if (canDrop)
		drop();

}

//-------------------------------------------------------Oil Barrel-------------------------------------------------------------


bool Oil_Barrel::isAlive()
{
	return Alive;
}

void Oil_Barrel::ProximityCheck()
{
	Iceman* player = getWorld()->getPlayer();

	int oX = getX();
	int oY = getY();

	int pX = player->getX();
	int pY = player->getY();

	/*for (int x1 = oX; x1 < oX + 4; x1++)
	{
		for (int x2 = pX; x2 < pX + 4; x2++)
		{
			if (x1 == x2 && oY == (pY + 3))
			{
				setVisible(true);
				return;
			}
		}
	}*/


	//for (int Oil_X = oX; oX < oX + 4; oX++) // up,down, left and right
	//{

	//}
	//for (int Oil_X = oX; oX > oX - 4 && oX < oX; oX++)
	//{

	//}


	//if (((oX - 4 >= pX) && (pX <= oX + 4)) && ((oY - 4 >= pY) && (pY <= oY + 4))) {
		//setVisible(true);
	//}

	if (((pX >= oX - 4 ) && (pX <= oX + 4)) && ((pY >= oY - 4 ) && (pY <= oY + 4))) {
		setVisible(true);
	}


}

void Oil_Barrel::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	
	ProximityCheck();
	EraseOil();

}

void Oil_Barrel::EraseOil()
{
	Iceman* player = getWorld()->getPlayer();

	int oX = getX();
	int oY = getY();

	int pX = player->getX();
	int pY = player->getY();
	
	if (((pX >= oX - 3) && (pX <= oX + 3)) && ((pY >= oY - 3) && (pY <= oY + 3))) {
		
		Alive = false;
		
		getWorld()->playSound(SOUND_FOUND_OIL);
		setVisible(false);
		player->AccumulatePoints(1000);
	}


	/*if (((pX >= oX - 1) && (pX <= oX + 1)) && ((pY >= oY - 1) && (pY <= oY + 1))) {

	}*/

}




// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

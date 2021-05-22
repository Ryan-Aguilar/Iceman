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
	i_gold_sack(0), i_facing(right) {}

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
		case 'z':
			if (i_gold_sack == 0) break;
			getWorld()->addActor(new Gold(icemanX, icemanY, getWorld(), 0));
			addGold(-1);  break;
		case 'Z':
			if (i_gold_sack == 0) break;
			getWorld()->addActor(new Gold(icemanX, icemanY, getWorld(), 0));
			addGold(-1);  break;
		}
	}

}

int Iceman::getHealth() const
{
	return i_health;
}
int Iceman::getNumGold() const
{
	return i_gold_sack;
}
int Iceman::getNumSquirt() const
{
	return i_squirt;
}
int Iceman::getNumSonar() const
{
	return i_sonarCharge;
}

void Iceman::addGold(int g)
{
	i_gold_sack += g;
}
void Iceman::addSonar(int s)
{
	i_sonarCharge += s;
}
void Iceman::addWtr(int w)
{
	i_squirt += w;
}

//--------------------------------------------------------Boulder-----------------------------------------------------------------------------





void Boulder::drop()
{
	if (delay > 0)
	{
		delay--;
		return;
	}

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



void Oil_Barrel::ProximityCheck()
{
	Iceman* player = getWorld()->getPlayer();

	int oX = getX();
	int oY = getY();

	int pX = player->getX();
	int pY = player->getY();

	if (((pX >= oX - 4) && (pX <= oX + 4)) && ((pY >= oY - 4) && (pY <= oY + 4))) {
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

		setAliveStatus(false);

		getWorld()->updateOilLeft(1);

		getWorld()->playSound(SOUND_FOUND_OIL);
		setVisible(false);
	}

}

//-------------------------------------------------------Gold Nugget--------------------------------------------------------------------------

bool Gold::ProximityCheck(int prox, Actor* a)
{

	int gX = getX();
	int gY = getY();

	int pX = a->getX();
	int pY = a->getY();

	if (((pX >= gX - prox) && (pX <= gX + prox)) && ((pY >= gY - prox) && (pY <= gY + prox))) {
		return true;
	}
	return false;
}

void Gold::doSomething()
{
	if (!isAlive())
	{
		setAliveStatus(false);
		return;
	}

	if (!(isVisible()) && ProximityCheck(4, getWorld()->getPlayer()))
	{
		setVisible(true);
		return;
	}
	else if (state == 1 && ProximityCheck(3, getWorld()->getPlayer()))
	{
		setAliveStatus(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getWorld()->getPlayer()->addGold(1);
	}

	/*
	else if (state == 0 && ProximityCheck(3, getWorld()->getPlayer())) //getProtestor?
	{
		setAliveStatus(false);
		getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
		//update the actions of the protester
		//increase points 25
	}
	*/

	if (state == 0)
	{
		if (delay > 0)
			delay--;
		else
			setAliveStatus(false);
	}
}


//------------------------------------------------------------Sonar----------------------------------------------------------

int Sonar::getspan()
{
	return getWorld()->SonarLifeSpan();
}

void Sonar::ProximityCheck()
{
	Iceman* player = getWorld()->getPlayer();

	int oX = getX();
	int oY = getY();

	int pX = player->getX();
	int pY = player->getY();

	if (((pX >= oX - 3) && (pX <= oX + 3)) && ((pY >= oY - 3) && (pY <= oY + 3))) {

		setAliveStatus(false);

		getWorld()->updateSonarLeft(1);

		getWorld()->playSound(SOUND_GOT_GOODIE);
		setVisible(false);
	}
}


void Sonar::doSomething() {
	if (!isAlive())
	{
		return;
	}

	ProximityCheck();

	if (state == 1)
	{
		if (delay > 0)
			delay--;
		else
			setAliveStatus(true);
	}


}

//Protester::Protester(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0):
//	Actor(imageID, startX, startY, startDirection, size, depth) {
//
//}
//
//HardcoreProtester::HardcoreProtester(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0) :
//	Actor(imageID, startX, startY, startDirection, size, depth) {
//
//}

// thee is still more to declare such as ice, boulders, etc...

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

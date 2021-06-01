#include "Actor.h"
#include "StudentWorld.h"

#include <algorithm>

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

void Actor::damageintake(int x) {
	return;
}
std::string Actor::type() {
	return "Actor";
}

bool Actor::proximityCheck(int prox, Actor* a)
{

	int gX = getX();
	int gY = getY();

	int aX = a->getX();
	int aY = a->getY();

	if (((aX >= gX - prox) && (aX <= gX + prox)) && ((aY >= gY - prox) && (aY <= gY + prox))) {
		return true;
	}
	return false;
}

//--------------------------------------------------------Iceman-----------------------------------------------------------------------------


//bool Iceman::isAlive()
//{
//	return i_health > 0;
//}

void Iceman::damage(int damage)
{
	i_health -= damage;
}

std::string Iceman::type()
{
	std::string x = "Iceman";
	return x;
}

void Iceman::doSomething()
{
	if (i_health <= 0) // this signifies if the Iceman is dead.
	{
		setAliveStatus(false);
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
		case KEY_PRESS_RIGHT: //move right
		case 'd':
		case 'D':
		case 6:
			if (getX() == 60) break;
			setDirection(right);
			this->moveTo(getX() + 1, getY()); break;
		case KEY_PRESS_LEFT: //move left
		case 'a':
		case 'A':
		case 4:
			if (getX() == 0) break;
			setDirection(left);
			this->moveTo(getX() - 1, getY()); break;
		case KEY_PRESS_UP: //move up
		case 'w':
		case 'W':
		case 8:
			if (getY() == 60) break;
			setDirection(up);
			this->moveTo(getX(), getY() + 1); break;
		case KEY_PRESS_DOWN: //move down
		case 's':
		case 'S':
		case 2:
			if (getY() == 0) break;
			setDirection(down);
			this->moveTo(getX(), getY() - 1); break;

		case '	': //drop gold
			if (i_gold_sack == 0) break;
			getWorld()->addActor(new Gold(icemanX, icemanY, getWorld(), 0));
			addGold(-1);  break;
		case ' ': //squirt water
			if (i_squirt == 0) break;
			if (getDirection() == right)
				getWorld()->addActor(new Squirt(icemanX + 1, icemanY, getWorld()));
			else if (getDirection() == left)
				getWorld()->addActor(new Squirt(icemanX - 1, icemanY, getWorld()));
			else if (getDirection() == up)
				getWorld()->addActor(new Squirt(icemanX, icemanY + 1, getWorld()));
			else if (getDirection() == down)
				getWorld()->addActor(new Squirt(icemanX, icemanY - 1, getWorld()));
			addWtr(-1);  break;
		case 'z':
		case 'Z': //sonar charge
			if (i_sonarCharge == 0) break;
			getWorld()->call();
			getWorld()->playSound(SOUND_SONAR);
			addSonar(-1); break;
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

std::string Boulder::type()
{
	std::string x = "Boulder";
	return x;
}

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

	std::string p = "RegularProtester";
	getWorld()->damagecall(type(), p, getX(), getY());

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



	//check if in contact with Protestors


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

void Oil_Barrel::sonarSearch()
{
	if (proximityCheck(12, getWorld()->getPlayer()))
	{
		setVisible(true);
	}
}

std::string Oil_Barrel::type()
{
	std::string x = "Oil";
	return x;
}

void Oil_Barrel::doSomething()
{
	if (!isAlive())
	{
		return;
	}

	if (proximityCheck(4, getWorld()->getPlayer()))
	{
		setVisible(true);
	}
	if (isVisible() && proximityCheck(3, getWorld()->getPlayer()))
	{
		setAliveStatus(false);

		getWorld()->updateOilLeft(1);
		getWorld()->increaseScore(1000);
		getWorld()->playSound(SOUND_FOUND_OIL);
		setVisible(false);
	}

}
//-------------------------------------------------------Gold Nugget--------------------------------------------------------------------------

void Gold::sonarSearch()
{
	if (proximityCheck(12, getWorld()->getPlayer()))
	{
		setVisible(true);
	}
}

std::string Gold::type()
{
	std::string x = "Gold";
	return x;
}

void Gold::doSomething()
{
	if (!isAlive())
	{
		setAliveStatus(false);
		return;
	}

	if (!(isVisible()) && proximityCheck(4, getWorld()->getPlayer()))
	{
		setVisible(true);
		return;
	}
	else if (state == 1 && proximityCheck(3, getWorld()->getPlayer()))
	{
		setAliveStatus(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getWorld()->getPlayer()->addGold(1);
	}


	else if (state == 0)
	{
		if (getWorld()->protesterGold(getX(), getY()))
		{
			setAliveStatus(false);
			return;
		}
	}


	if (state == 0)
	{
		if (delay != 0)
			delay--;
		else
			setAliveStatus(false);
	}
}


//-------------------------------------------------------Water--------------------------------------------------------------------------

Water::Water(int x, int y, StudentWorld* world) :
	Actor(IID_WATER_POOL, x, y, right, world, 1.0, 2)
{
	setVisible(true);
	int t = (int)(300 - (10 * getWorld()->getLevel()));
	delay = std::max(100, t);
}

std::string Water::type()
{
	std::string x = "Water";
	return x;
}

void Water::doSomething()
{
	if (!isAlive())
	{
		setAliveStatus(false);
		return;
	}

	if (proximityCheck(3, getWorld()->getPlayer()))
	{
		setAliveStatus(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(100);
		getWorld()->getPlayer()->addWtr(5);
	}

	if (delay != 0)
		delay--;
	else
		setAliveStatus(false);

}

//-------------------------------------------------------Squirt--------------------------------------------------------------------------
Squirt::Squirt(int x, int y, StudentWorld* world) :
	Actor(IID_WATER_SPURT, x, y, right/*temporary direction*/, world, 1.0, 1), distanceLeft(4), hit(0)
{
	Iceman* player = getWorld()->getPlayer();
	Direction squirtDir = player->getDirection();
	setDirection(squirtDir); //set the direction

	setVisible(true);
}

int Squirt::hitRegulator(int x)
{
	return hit += x;
}

std::string Squirt::type()
{
	std::string x = "Squirt";
	return x;
}


void Squirt::damage()
{
	/*
		squirt causes 2 damage.
		how can i damage

		we can use a enhanced for loop to traverse through the array and we can see which is the close to the squirt.
		we can use a for loop then to then damage the protesters health

	*/

	int sx = getX();
	int sy = getY();

	std::string protester = "RegularProtester";
	std::string hardcore = "Hardcore";


	getWorld()->damagecall(type(), protester, sx, sy);

	return;


}

void Squirt::doSomething()
{
	if (!isAlive())
	{
		setAliveStatus(false);
		return;
	}
	//check its location with Protestors, Boulders, and Ice, a water squirt cannot pass through these Actors

	if (hitRegulator(0) == 0)
	{
		hitRegulator(1);
		damage();
	}


	if (distanceLeft == 0) setAliveStatus(false);
	else
	{
		int sX = getX(), sY = getY();

		for (int x = sX; x < sX + 4; x++)
		{
			for (int y = sY; y < sY + 4; y++)
			{
				if (getWorld()->checkIce(x, y))
				{
					setAliveStatus(false);
					return;
				}
			}
		}

		if (getDirection() == right)
			moveTo(getX() + 1, getY());
		else if (getDirection() == left)
			moveTo(getX() - 1, getY());
		else if (getDirection() == up)
			moveTo(getX(), getY() + 1);
		else if (getDirection() == down)
			moveTo(getX(), getY() - 1);

		distanceLeft--;
	}

	//check next location 

}


//------------------------------------------------------------Sonar----------------------------------------------------------

Sonar::Sonar(int x, int y, StudentWorld* world) :
	Actor(IID_SONAR, x, y, right, world, 1.0, 2) {
	setVisible(true);
	setAliveStatus(true);
	int t = std::max(100, 300 - 10 * (int)getWorld()->getLevel());
	delay = t;
}

std::string Sonar::type()
{
	std::string x = "Sonar";
	return x;
}

void Sonar::doSomething() {
	if (!isAlive())
	{
		return;
	}

	Iceman* player = getWorld()->getPlayer();

	if (proximityCheck(3, player))
	{

		setAliveStatus(false);


		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->updateSonarLeft(-1);
		player->addSonar(1);
		getWorld()->increaseScore(75);
	}


	if (delay > 0)
	{
		delay--;
	}
	else
	{
		setAliveStatus(false);
		getWorld()->updateSonarLeft(-1);
	}


}


//--------------------------------------------------------Regular_protesters-----------------------------------------------------------------
Regular_protester::Regular_protester(int x, int y, StudentWorld* world) :
	Actor(IID_PROTESTER, x, y, left, world, 1.0, 0), delay(0), cooldown(1), p_health(5), state(1), numStepsLeft(10), ticks(200)
{
	setVisible(true);
	int tickDelay = std::max(0, (3 - (int)getWorld()->getLevel()) / 4);
}

std::string Regular_protester::type()
{
	std::string x = "RegularProtester";
	return x;
}

void Regular_protester::damageintake(int x)
{
	p_health -= x;
	if (p_health < 0)
	{
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		state = 0;
	}

}

void Regular_protester::damage(int damage)
{
	bool trigger = false;
	Iceman* player = getWorld()->getPlayer();

	int pX = getX();
	int pY = getY();

	int iX = player->getX();
	int iY = player->getY();

	if (cooldown == 1)
	{
		if (pX >= iX - 4) // checks left
		{

			trigger = true;
			if (trigger)
			{
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				player->damage(2);
				return;
			}
		}
		else if (pX <= iX + 4) // checks right
		{
			trigger = true;
			if (trigger)
			{
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				player->damage(2);
				return;
			}
		}
		else if (pY >= iY - 4) // checks down
		{
			trigger = true;
			if (trigger)
			{
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				player->damage(2);
				return;
			}
		}
		else if (pY <= iY + 4) // checks up
		{
			trigger = true;
			if (trigger)
			{
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				player->damage(2);
				return;
			}
		}
		delay = 5;
		//cooldown = 0;
		return;
	}
	else if (cooldown == 0)
	{
		//coolDown();
		return;
	}


}

//void Regular_protester::resetcoolDown(int x)
//{
//	delay += x;
//	cooldown = 1;
//}
//
//void Regular_protester::coolDown()
//{
//	if (delay > 0)
//	{
//		delay--;
//		return;
//	}
//
//	if (delay == 0)
//	{
//		resetcoolDown(15);
//		cooldown = 1;
//		return;
//	}
//}

void Regular_protester::gotGold()
{
	state = 0;
}

void Regular_protester::p_move()
{

	if (getDirection() == right && getX() < 60)
	{
		if (!getWorld()->checkIce(getX() + 4, getY()) && !getWorld()->checkIce(getX() + 4, getY() + 1)
			&& !getWorld()->checkIce(getX() + 4, getY() + 2) && !getWorld()->checkIce(getX() + 4, getY() + 3))
		{
			moveTo(getX() + 1, getY());
		}
		else
			changeDir();
	}
	else if (getDirection() == left && getX() > 0)
	{
		if (!getWorld()->checkIce(getX() - 1, getY()) && !getWorld()->checkIce(getX() - 1, getY() + 1)
			&& !getWorld()->checkIce(getX() - 1, getY() + 2) && !getWorld()->checkIce(getX() - 1, getY() + 3))
		{
			moveTo(getX() - 1, getY());
		}
		else
			changeDir();
	}
	else if (getDirection() == up && getY() < 60)
	{
		if (!getWorld()->checkIce(getX(), getY() + 4) && !getWorld()->checkIce(getX() + 1, getY() + 4)
			&& !getWorld()->checkIce(getX() + 2, getY() + 4) && !getWorld()->checkIce(getX() + 3, getY() + 4))
		{
			moveTo(getX(), getY() + 1);
		}
		else
			changeDir();
	}
	else if (getDirection() == down && getY() > 0)
	{
		if (!getWorld()->checkIce(getX(), getY() - 1) && !getWorld()->checkIce(getX() + 1, getY() - 1)
			&& !getWorld()->checkIce(getX() + 2, getY() - 1) && !getWorld()->checkIce(getX() + 3, getY() - 1))
		{
			moveTo(getX(), getY() - 1);
		}
		else
			changeDir();
	}
	else
		numStepsLeft = 1;


	numStepsLeft--;
}

void Regular_protester::changeDir()
{
	int dir = rand() % 4;

	Direction currentDir = getDirection();

	if (dir == 0 || getX() <= 0)
	{
		for (int y = getY(); y < getY() + 4; y++)
		{
			if (getWorld()->checkIce(getX() + 4, y))
			{
				return;
			}
		}
		setDirection(right);
		if (currentDir == up || currentDir == down)
		{
			ticks = 200;
		}
	}
	else if (dir == 1 || getX() >= 60)
	{
		for (int y = getY(); y < getY() + 4; y++)
		{
			if (getWorld()->checkIce(getX() - 1, y))
			{
				return;
			}
		}
		setDirection(left);
		if (currentDir == up || currentDir == down)
		{
			ticks = 200;
		}
	}
	else if (dir == 2 || getY() <= 0)
	{
		for (int x = getX(); x < getX() + 4; x++)
		{
			if (getWorld()->checkIce(x, getY() + 4))
			{
				return;
			}
		}
		setDirection(up);
		if (currentDir == left || currentDir == right)
		{
			ticks = 200;
		}
	}
	else if (dir == 3 || getY() >= 60)
	{
		for (int x = getX(); x < getX() + 4; x++)
		{
			if (getWorld()->checkIce(x, getY() - 1))
			{
				return;
			}
		}
		setDirection(down);
		if (currentDir == right || currentDir == left)
		{
			ticks = 200;
		}
	}



	numStepsLeft = rand() % 52 + 8;
}

void Regular_protester::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	Iceman* player = getWorld()->getPlayer();


	ticks--; //200 ticks, move perpendicular


	if (state == 2) //waiting state
	{
		if (restDelay > 0)
		{
			restDelay--;
			return;
		}
		else
		{
			state = 1;
			restDelay = std::max(0, 3 - ((int)getWorld()->getLevel() / 4));
		}
	}
	else if (state == 1) //alive state
	{
		//------------damage iceman

		if (proximityCheck(4, getWorld()->getPlayer()))
		{
			if (delay <= 0)
			{
				damage(2);
				delay = 5;
			}
			else
				delay--;

			state = 2;
			return;
		}

		//------------movement

		bool canMove = true;


		//After 200 ticks, move perpendicularly
		if (ticks <= 0)
		{
			if (getDirection() == left || getDirection() == right)
			{
				if (getY() > 0 && !getWorld()->checkIce(getX(), getY() - 1) && !getWorld()->checkIce(getX() + 1, getY() - 1)
					&& !getWorld()->checkIce(getX() + 2, getY() - 1) && !getWorld()->checkIce(getX() + 3, getY() - 1))
				{
					ticks = 200;
					setDirection(down);
					p_move();
				}

				else if (getY() < 60 && !getWorld()->checkIce(getX(), getY() + 4) && !getWorld()->checkIce(getX() + 1, getY() + 4)
					&& !getWorld()->checkIce(getX() + 2, getY() + 4) && !getWorld()->checkIce(getX() + 3, getY() + 4))
				{
					ticks = 200;
					setDirection(up);
					p_move();
				}
			}
			else if (getDirection() == up || getDirection() == down)
			{
				if (getX() > 0 && !getWorld()->checkIce(getX() - 1, getY()) && !getWorld()->checkIce(getX() - 1, getY() + 1)
					&& !getWorld()->checkIce(getX() - 1, getY() + 2) && !getWorld()->checkIce(getX() - 1, getY() + 3))
				{
					ticks = 200;
					setDirection(left);
					p_move();
				}

				else if (getX() < 60 && !getWorld()->checkIce(getX() + 4, getY()) && !getWorld()->checkIce(getX() + 4, getY() + 1)
					&& !getWorld()->checkIce(getX() + 4, getY() + 2) && !getWorld()->checkIce(getX() + 4, getY() + 3))
				{
					ticks = 200;
					setDirection(right);
					p_move();
				}
			}
		}


		//Line of sight on Iceman
		if (canMove && player->getX() > (getX() - 4) && player->getX() < (getX() + 4))
		{
			if (player->getY() > getY())
			{
				for (int x = getX(); x < getX() + 4; x++)
				{
					if (getWorld()->checkIce(x, getY() + 4))
					{
						canMove = false;
					}
				}
				if (canMove)
				{
					setDirection(up);
					moveTo(getX(), getY() + 1);
				}
				else
					p_move();
			}
			else
			{
				for (int x = getX(); x < getX() + 4; x++)
				{
					if (getWorld()->checkIce(x, getY() - 1))
					{
						canMove = false;
					}
				}
				if (canMove)
				{
					setDirection(down);
					moveTo(getX(), getY() - 1);
				}
				else
					p_move();
			}
			state = 2;
			numStepsLeft = 0;
			return;
		}
		else if (canMove && player->getY() > (getY() - 4) && player->getY() < (getY() + 4))
		{
			if (player->getX() > getX())
			{
				for (int y = getY(); y < getY() + 4; y++)
				{
					if (getWorld()->checkIce(getX() + 4, y))
					{
						canMove = false;
					}
				}
				if (canMove)
				{
					setDirection(right);
					moveTo(getX() + 1, getY());
				}
				else
					p_move();
			}
			else
			{
				for (int y = getY(); y < getY() + 4; y++)
				{
					if (getWorld()->checkIce(getX() - 1, y))
					{
						canMove = false;
					}
				}
				if (canMove)
				{
					setDirection(left);
					moveTo(getX() - 1, getY());
				}
				else
					p_move();
			}
			if (canMove)
				numStepsLeft = 0;

			canMove = true;
			state = 2;
			return;
		}


		//Default Movement
		if (numStepsLeft == 0)
		{
			changeDir();
		}

		p_move();

		state = 2;
	}
	else if (state == 0) //dead state
	{
		getWorld()->increaseScore(100);
		getWorld()->shortestpath(getX(), getY(), 60, 60, this);
		setAliveStatus(false);
	}
}
//-------------------------------Hardcore Protester----------------------------------


hardcoreprotester::hardcoreprotester(int x, int y, StudentWorld* world)
	:Actor(IID_HARD_CORE_PROTESTER, x, y, left, world, 1.0, 0), hitpoints(20), cooldown(1), delay(0)
{
	setVisible(true);
}

void hardcoreprotester::damageintake(int x)
{
	hitpoints -= x;
}

void hardcoreprotester::damage()
{
	bool trigger = false;
	Iceman* player = getWorld()->getPlayer();

	int pX = getX();
	int pY = getY();

	int iX = player->getX();
	int iY = player->getY();

	if (cooldown == 1)
	{
		if (pX >= iX - 4) // checks left
		{

			trigger = true;
			if (trigger)
			{
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				player->damage(2);
				return;
			}
		}
		else if (pX <= iX + 4) // checks right
		{
			trigger = true;
			if (trigger)
			{
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				player->damage(2);
				return;
			}
		}
		else if (pY >= iY - 4) // checks down
		{
			trigger = true;
			if (trigger)
			{
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				player->damage(2);
				return;
			}
		}
		else if (pY <= iY + 4) // checks up
		{
			trigger = true;
			if (trigger)
			{
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				player->damage(2);
				return;
			}
		}
		delay = 5;
		//cooldown = 0;
		return;
	}
	else if (cooldown == 0)
	{
		//coolDown();
		return;
	}

}


void hardcoreprotester::doSomething()
{
	if (!isAlive())
	{
		return;
	}
}





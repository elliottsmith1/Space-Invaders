#include "Bullet.h"
#include "Game.h"
#include "Player.h"

void Bullet::loadBullet(std::shared_ptr<ASGE::Renderer> renderer)
{
	//create and set position of sprite
	is_alive = false; 
	bullet = renderer->createSprite();
	bullet->position[0] = -10;
	bullet->position[1] = -10;
	bullet->scale = 4;
	bullet->loadTexture("..\\..\\Resources\\Textures\\Bullet.png");
}



void Bullet::setBullet(int x, int y)
{
	//set bullet at point of firing and set to alive
	bullet->position[0] = x;
	bullet->position[1] = y;

	is_alive = true;
}



void Bullet::moveBullet(int direction)
{
	//if bullet is off screen then kill it
	if ((bullet->position[1] < -20) || (bullet->position[1] > 700))
	{
		is_alive = false; 
		missed = true;
	}

	if (is_alive)
	{
		//if bullet is live then move up or down
		bullet->position[1] += direction;
	}

	else
	{
		//hide bullet if dead
		bullet->position[0] = -10;
		bullet->position[1] = -10;
	}
}



const int Bullet::getXPosition()
{
	if (bullet)
	{
		return bullet->position[0];
	}
}



const int Bullet::getYPosition()
{
	if (bullet)
	{
		return bullet->position[1];
	}
}

void Bullet::setMissed(bool msd)
{
	missed = msd;
}



const bool Bullet::getMissed()
{
	return missed;
}

#include "Barrier.h"
#include "Game.h"

Barrier::Barrier()
{
	health = 3;
}



void Barrier::loadBarrier(std::shared_ptr<ASGE::Renderer> renderer)
{
	//create and set position of sprite
	barrier = renderer->createSprite();
	barrier->position[0] = -10;
	barrier->position[1] = -10;
	barrier->scale = 1;
	barrier->loadTexture("..\\..\\Resources\\Textures\\Barrier.png");
}



const int Barrier::getXPosition()
{
	if (barrier)
	{
		return barrier->position[0];
	}
}



const int Barrier::getYPosition()
{
	if (barrier)
	{
		return barrier->position[1];
	}
}



void Barrier::changeBarrier()
{
	//set barrier sprite depending on current health
	if (health == 3)
	{
		barrier->loadTexture("..\\..\\Resources\\Textures\\Barrier.png");
	}

	else if (health == 2)
	{
		barrier->loadTexture("..\\..\\Resources\\Textures\\Barrier2.png");
	}

	else if (health == 1)
	{
		barrier->loadTexture("..\\..\\Resources\\Textures\\Barrier3.png");
	}

	//if health == 0 then barrier is dead
	else
	{
		is_alive = false;
	}
}



void Barrier::resetBarrier()
{
	//reset barrier back to full health for new game
	is_alive = true;
	health = 3;
}

#include "Mothership.h"
#include "Game.h"

Mothership::Mothership()
{
	health = 1;
	is_alive = false;
}



void Mothership::loadEnemy(std::shared_ptr<ASGE::Renderer> renderer)
{
	//create and set position of sprite
	mothership = renderer->createSprite();
	mothership->position[0] = 50;
	mothership->position[1] = 50;
	mothership->scale = 0.05f;
	mothership->loadTexture("..\\..\\Resources\\Textures\\SpaceShip.png");
}



const int Mothership::getXPosition()
{
	if (mothership)
	{
		return mothership->position[0];
	}
}



const int Mothership::getYPosition()
{
	if (mothership)
	{
		return mothership->position[1];
	}
}






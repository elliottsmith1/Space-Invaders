#include "Enemy.h"
#include "Game.h"

Enemy::Enemy()
{
	health = 1;
}



void Enemy::loadEnemy(std::shared_ptr<ASGE::Renderer> renderer)
{
	//create and set position of sprite
	enemy = renderer->createSprite();
	enemy->position[0] = 50;
	enemy->position[1] = start_y;
	enemy->scale = 0.07f;
	enemy->loadTexture("..\\..\\Resources\\Textures\\Alien1.png");
}



const int Enemy::getDirection()
{
	return direction;
}



void Enemy::setDirection(int drct)
{
	direction = drct;
}



const int Enemy::getXPosition()
{
	if (enemy)
	{
		return enemy->position[0];
	}
}



const int Enemy::getYPosition()
{
	if (enemy)
	{
		return enemy->position[1];
	}
}



const bool Enemy::getCanShoot()
{
	return can_shoot;
}



void Enemy::setCanShoot(bool cnshoot)
{
	can_shoot = cnshoot;
}



void Enemy::setStartY(float strt)
{
	start_y = strt;
}



const int Enemy::getStartY()
{
	return start_y;
}





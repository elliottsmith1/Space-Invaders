#include "Player.h"
#include "Game.h"

Player::Player()
{
	health = 3;
}



void Player::loadPlayer(std::shared_ptr<ASGE::Renderer> renderer)
{
	//create and set position of sprite
	player = renderer->createSprite();
	player->position[0] = 500;
	player->position[1] = 675;
	player->scale = 0.05f;
	player->loadTexture("..\\..\\Resources\\Textures\\Player.png");
}



void Player::movePlayer(float speed)
{
	player->position[0] += speed;
}



const int Player::getXPosition()
{
	if (player)
	{
		return player->position[0];
	}
}



const int Player::getYPosition()
{
	if (player)
	{
		return player->position[1];
	}
}



const int Player::getScore()
{
	return score;
}



void Player::setScore(int scre)
{
	score = scre;
}



const int Player::getMultiplier()
{
	return multiplier;
}



void Player::setMultiplier(int multp)
{
	multiplier = multp;
}



void Player::setDeath(bool dth)
{
	death = dth;
}



const bool Player::getDeath()
{
	return death;
}
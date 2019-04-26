#include "GameActor.h"

const int GameActor::getHealth()
{
	return health;
}



void GameActor::setHealth(int hlth)
{
	health = hlth;
}



const bool GameActor::getAlive()
{
	return is_alive;
}



void GameActor::setAlive(bool alve)
{
	is_alive = alve;
}
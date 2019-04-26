#pragma once
#include <string>

class GameActor
{
public:
	GameActor() = default;
	~GameActor() = default;

	const int getHealth(); //get health
	const bool getAlive(); //get is alive
	void setAlive(bool alve); //set is alive
	void setHealth(int hlth); //set health

protected:
	int health =    10; //health integer
	bool is_alive = true; //whether actor is alive or not
};
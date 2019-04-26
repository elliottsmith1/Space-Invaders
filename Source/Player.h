#pragma once
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

#include <string>

#include "GameActor.h"

namespace ASGE {
	class Renderer;
	class Sprite;
}

class Player :
	public GameActor
{
public:
	Player();
	~Player() = default;

	void movePlayer(float speed); //move player sprite
	void loadPlayer
	(std::shared_ptr<ASGE::Renderer> renderer); //load player sprite
		
	const int getXPosition(); //get x coord
	const int getYPosition(); //get y coord
	const int getScore(); //get score
	void setScore(int scre); //set score
	const int getMultiplier(); //get multiplier
	void setMultiplier(int mltp); //set multiplier
	void setDeath(bool dth); //set death 
	const bool getDeath(); //get death

	std::unique_ptr<ASGE::Sprite> player = nullptr; //player sprite

private:
	int score =      0; //player score
	int multiplier = 1; //score multiplier
	bool death =     false; //death state when shot
};


#pragma once
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

#include <string>
#include <vector>

#include "GameActor.h"

namespace ASGE {
	class Renderer;
	class Sprite;
}

class Enemy :
	public GameActor
{
public:
	Enemy();
	~Enemy() = default;

	const int getDirection(); //get direction
	const bool getCanShoot(); //get can shoot
	void setCanShoot(bool cnshoot); //set can shoot
	void setDirection(int drct); //set direction
	const virtual int getXPosition(); //get x coord
	const virtual int getYPosition(); //get y coord
	const int getStartY(); //get starting y position
	void setStartY(float strt); //set starting y position
	virtual void loadEnemy(
		std::shared_ptr<ASGE::Renderer> renderer); //load enemy sprite

	std::unique_ptr<ASGE::Sprite> enemy = nullptr; //enemy sprite

private:
	float direction = 30; //enemy direction and speed
	float start_y =	  100; //enemy starting y position
	bool can_shoot =  false; //if enemy can shoot
};


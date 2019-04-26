#pragma once
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

#include <string>

#include "Enemy.h"

namespace ASGE {
	class Renderer;
	class Sprite;
}

class Mothership :
	public Enemy
{
public:
	Mothership();
	~Mothership() = default;

	const virtual int getXPosition(); //get x coord
	const virtual int getYPosition(); //get y coord

	//load enemy sprite
	virtual void loadEnemy(std::shared_ptr<ASGE::Renderer> renderer); 

	std::unique_ptr<ASGE::Sprite> mothership = nullptr; //mothership sprite
};


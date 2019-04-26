#pragma once
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

#include <string>

#include "GameActor.h"

namespace ASGE {
	class Renderer;
	class Sprite;
}

class Barrier :
	public GameActor
{
public:
	Barrier();
	~Barrier() = default;
	
	//load barrier sprite details
	void loadBarrier(std::shared_ptr<ASGE::Renderer> renderer); 
	void changeBarrier(); //change sprite
	void resetBarrier(); //reset to full health

	const int getXPosition(); //get x coord
	const int getYPosition(); //get y coord

	std::unique_ptr<ASGE::Sprite> barrier = nullptr; //barrier sprite
};


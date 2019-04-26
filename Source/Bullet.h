#pragma once
#include <Engine/OGLGame.h>
#include <Engine/Sprite.h>

#include <string>

#include "GameActor.h"

namespace ASGE {
	class Renderer;
	class Sprite;
}

class Bullet :
	public GameActor
{
public:
	Bullet() = default;
	~Bullet() = default;

	//load bullet sprite	
	void loadBullet(std::shared_ptr<ASGE::Renderer> renderer); 
	void moveBullet(int direction); //move fired bullet 
	
	void setMissed(bool mds); //set bullet missed
	const bool getMissed();  //get bullet missed
	void setBullet(int x, int y); //place bullet for firing
	const int getXPosition(); //get x coord
	const int getYPosition(); //get y coord

	std::unique_ptr<ASGE::Sprite> bullet = nullptr; //bullet sprite

private:
	bool missed = false; //whether bullet has missed
};


#pragma once
#include <Engine/OGLGame.h>
#include <irrKlang.h>

#include <string>

#include "Actions.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Barrier.h"
#include "Mothership.h"

struct GameFont;

/**
*  Invaders Game. An OpenGL Game based on ASGE.
*/

class Player;
class Enemy;
class Bullet;
class Barrier;
class Mothership;

// forward declaration or irrKlang
namespace irrklang
{
	class ISoundEngine;
}

class InvadersGame:
	public ASGE::OGLGame
{
public:
	InvadersGame();
	~InvadersGame();

	enum class GameState
	{
		MAIN_MENU = 0,
		PLAYING =   1,
		PAUSE =     2,
		EXIT =      3,
		OPTIONS =   4,
		GAME_OVER = 5
	};

	// Inherited via Game
	virtual bool run() override;
	bool shouldExit() const;
	void render();

	// Inherited via OGLGame
	virtual bool init();
	virtual void drawFrame();

	//audio
	const bool initAudio(); //initialise audio engine
	void playAlarm(); //play alarm sound

	//player
	const void checkPlayerAlive(); //check player alive status
	void deathDelay(); //delay when player loses a life
	
	//aliens
	void loadEnemies(); //load in aliens
	void moveAliens(); //move alien tick
	const void renderAliens(); //render alien sprites
	void enemyShoot(); //enemy shooting
	void changeAlienSpeed(); //change enemy movement tick speed
	const void checkAlienLives(); //check if all aliens are dead
	void resetEnemies(); //reset enemies to start positions
	
	//barriers
	void loadBarriers(); //load barrier sprites
	const void renderBarriers(); //render barrier sprites
	const void resetBarriers(); //set barriers back to full health
	const void changeBarriers(); //change barrier sprite

	//mothership
	const void renderMothership(); //render mothership sprite
	void deployMothership(); //enable and move mothership

	//bullets
	const void loadBullets(); //load alien bullet sprites
	const void renderBullets(); //render bullet sprites

	//input
	void stateInput(int key, int action); // menu state input

	//collisions
	void checkCollision(); // check if aliens have been shot
	void checkBarrierCollision(); //check if barrier has been shot
	void checkPlayerCollision(); //check if player has been shot
	void spawnExplosion(int x, int y); //spawn explosion sprite
	void checkMothershipCollision(); //check if mothership has been shot

	//game updates
	void updateGame(); //playing tick
	const void updateMenu(); //main menu
	const void updateOptions(); //control screen
	const void updatePause(); //pause screen
	const void updateGameOver(); //game over screen

	//logic
	void resetGame(); //reset game back to start

	//GUI
	void loadUI(); //load graphical user interface
	void renderUI(); //render graphical user interface
	const void loadControls(); //show control scheme
	const void renderMenuUI(); //render graphical user interface for menu	

private:
	void processGameActions(); //respond to user input
	void input(int key, int action); //user input
	
	/**< Input Callback ID. The callback ID assigned by the game engine. */
	int  callback_id = -1;      
	/**< Exit boolean. If true the game loop will exit. */
	bool exit = false;     

	//menu sprite
	std::unique_ptr<ASGE::Sprite>         invader = nullptr;  

	//player sprite
	std::unique_ptr<Player>               player_one = nullptr; 

	//player bullet sprite
	std::unique_ptr<Bullet>               bullet_one = nullptr;

	//mothership sprite
	std::unique_ptr<Mothership>           mothership_one = nullptr;

	//enemy alien sprites
	std::vector<std::unique_ptr<Enemy>>   aliens;

	//alien bullet sprites
	std::vector<std::unique_ptr<Bullet>>  bullets;

	//barrier sprites
	std::vector<std::unique_ptr<Barrier>> barriers;

	//player life sprites
	std::vector<std::unique_ptr<Player>>  lives;

	//explosion sprite
	std::unique_ptr<ASGE::Sprite>         explosion = nullptr;
	
	//keyboard control sprites
	std::unique_ptr<ASGE::Sprite> left = nullptr;
	std::unique_ptr<ASGE::Sprite> right = nullptr;
	std::unique_ptr<ASGE::Sprite> letterP = nullptr;
	std::unique_ptr<ASGE::Sprite> escape = nullptr;
	std::unique_ptr<ASGE::Sprite> space = nullptr;
	std::unique_ptr<ASGE::Sprite> one = nullptr;
	std::unique_ptr<ASGE::Sprite> two = nullptr;
	std::unique_ptr<ASGE::Sprite> three = nullptr;

	//starting game state
	GameState game_state = GameState::MAIN_MENU;

	int state_callback_id = -1;
	int game_callback_id =  -1;

	//game playing tick
	float time_difference =        0;

	//delay counter for death 
	float death_counter =          0;

	//time counter for spawning mothership
	float mothership_spawn_timer = 0;

	//tick counter to move alien enemies
	float alien_move_counter =     0;

	////enemy movement tick speed
	float alien_move_speed =       1.2;

	//delay on playing alarm sound 
	float alarm_counter =          0.75;

	//enemy shooting frequency
	int alien_shoot_speed =        20000;

	int move_id = 0; // 0 = none, 1 = left, 2 = right

	// unique pointer to destroy engine automagically
	std::unique_ptr<irrklang::ISoundEngine> audio_engine = nullptr;
};


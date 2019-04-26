#include "Game.h"
#include "Actions.h"
#include "Constants.h"
#include "GameFont.h"

#include <iostream>
#include <sstream>

#include <Engine/Input.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

#include <thread>
#include <chrono>
#include <random>

/**
*   @brief   Default Constructor.
*/
InvadersGame::InvadersGame()
{

}



/**
*   @brief   Destructor.
*   @details Remove any non-managed memory and callbacks.
*/
InvadersGame::~InvadersGame()
{
	audio_engine->stopAllSounds();

	this->inputs->unregisterCallback(callback_id);
}



/**
*   @brief   Initialises the game.
*   @details The game window is created and all assets required to
			 run the game are loaded. The input callback should also
			 be set in the initialise function. 
*   @return  True if the game initialised correctly.
*/
bool InvadersGame::init()
{
	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
	if (!initAPI())
	{
		return false;
	}

	renderer->setWindowTitle("Invaders - Exercise 1");
	renderer->setClearColour(ASGE::COLOURS::BLACK);
	toggleFPS();

	// input callback function
	state_callback_id = this->inputs->addCallbackFnc(
	&InvadersGame::stateInput, this);

	// load fonts we need
	GameFont::fonts[0] = new GameFont(renderer->loadFont(
	"..\\..\\Resources\\Fonts\\Comic.ttf", 42), "default", 42);
	
	if (GameFont::fonts[0]->id == -1)
	{
		return false;
	}

	// load space invader sprite
	invader = renderer->createSprite();
	invader->position[0] = 700;
	invader->position[1] = 250;

	if (!invader->loadTexture("..\\..\\Resources\\Textures\\Invader.jpg"))
	{
		return false;
	}

	//load explosion sprite
	explosion = renderer->createSprite();
	explosion->loadTexture("..\\..\\Resources\\Textures\\Explosion.png");
	explosion->scale = 0.1;
	explosion->position[0] = -30;
	explosion->position[1] = -30;

	loadUI();

	// load player and player sprite
	player_one = std::make_unique<Player>();
	player_one->loadPlayer(renderer);

	// load bullet and bullet sprite
	bullet_one = std::make_unique<Bullet>();
	bullet_one->loadBullet(renderer);

	// load mothership and mothership sprite
	mothership_one = std::make_unique<Mothership>();
	mothership_one->loadEnemy(renderer);

	loadEnemies();

	loadBullets();

	loadBarriers();

	initAudio();

	return true;
}



/**
*   @brief   The main game loop. 
*   @details The main loop should be responsible for updating the game
			 and rendering the current scene. Runs until the shouldExit
			 signal is received.
*   @return  True if the game ran correctly. 
*/
bool InvadersGame::run()
{
	while (!shouldExit())
	{		
		//update tick start
		auto start = std::chrono::steady_clock::now();

		//menu
		if (game_state == GameState::MAIN_MENU)
		{
			updateMenu();
		}

		//control screen
		else if (game_state == GameState::OPTIONS)
		{
			updateOptions();
		}

		//game play
		else if (game_state == GameState::PLAYING)
		{
			updateGame();
		}

		//game paused
		else if (game_state == GameState::PAUSE)
		{
			updatePause();
		}

		//game over screen
		else if (game_state == GameState::GAME_OVER)
		{
			updateGameOver();
		}
				
		//exit game
		else if (game_state == GameState::EXIT)
		{
			this->exit = true;
		}

		//update tick end
		auto end = std::chrono::steady_clock::now();

		//update tick time
		auto difference = std::chrono::duration<float>(end - start);
		time_difference = difference.count();
	}

	return false;
}



/**
*   @brief   Should the game exit?
*   @details Has the renderer terminated or the game requesting to exit?
*   @return  True if the game should exit
*/
bool InvadersGame::shouldExit() const
{
	return (renderer->exit() || this->exit);
}



/**
*   @brief   Renders the scene
*   @details Prepares the renderer subsystem before drawing the 
			 current frame. Once the current frame is has finished
			 the buffers are swapped accordingly and the image shown.
*   @return  void
*/
void InvadersGame::render()
{
	beginFrame();
	drawFrame();
	endFrame();
}



/**
*   @brief   Renderers the contents for this frame 
*   @details All game objects that need rendering should be done
			 in this function, as it ensures they are completed
			 before the buffers are swapped.
*   @return  void
*/
void InvadersGame::drawFrame()
{
	renderer->setFont(GameFont::fonts[0]->id);
	renderer->renderText("Space Invaders", 375, 325, 1.0, ASGE::COLOURS::RED);
}



void InvadersGame::loadUI()
{
	renderer->setFont(GameFont::fonts[0]->id);

	//x position of life sprites
	int counter = 1060;

	//load life sprites
	for (int i = 0; i < 3; i++)
	{
		lives.push_back(std::make_unique<Player>());
		lives[i]->loadPlayer(renderer);
		lives[i]->player->position[0] = counter;
		lives[i]->player->position[1] = 375;

		counter += 70;
	}

	//escape button sprite
	escape = renderer->createSprite();
	escape->loadTexture("..\\..\\Resources\\Textures\\computer_key_Esc.png");
	escape->scale = 0.5;
	escape->position[0] = 1200;
	escape->position[1] = 60;

	//P button sprite
	letterP = renderer->createSprite();
	letterP->loadTexture("..\\..\\Resources\\Textures\\computer_key_P.png");
	letterP->scale = 0.5;
	letterP->position[0] = 420;
	letterP->position[1] = 260;

	//space button sprite
	space = renderer->createSprite();
	space->scale = 0.5;
	space->position[0] = 420;
	space->position[1] = 360;
	space->loadTexture(
	"..\\..\\Resources\\Textures\\computer_key_space_Bar.png");
	
	//A button sprite
	left = renderer->createSprite();
	left->loadTexture("..\\..\\Resources\\Textures\\computer_key_A.png");
	left->scale = 0.5;
	left->position[0] = 420;
	left->position[1] = 460;

	//D button sprite
	right = renderer->createSprite();
	right->loadTexture("..\\..\\Resources\\Textures\\computer_key_D.png");
	right->scale = 0.5;
	right->position[0] = 470;
	right->position[1] = 460;

	//1 button sprite
	one = renderer->createSprite();
	one->scale = 0.5;
	one->position[0] = 310;
	one->position[1] = 340;
	one->loadTexture(
	"..\\..\\Resources\\Textures\\computer_key_num_row_1.png");

	//2 button sprite
	two = renderer->createSprite();
	two->scale = 0.5;
	two->position[0] = 310;
	two->position[1] = 425;
	two->loadTexture(
	"..\\..\\Resources\\Textures\\computer_key_num_row_2.png");

	//3 button sprite
	three = renderer->createSprite();
	three->scale = 0.5;
	three->position[0] = 310;
	three->position[1] = 505;
	three->loadTexture(
	"..\\..\\Resources\\Textures\\computer_key_num_row_3.png");	
}



void InvadersGame::renderUI()
{
	//gameplay GUI

	//return button
	renderer->renderText("RETURN", 1060, 100, 0.75, ASGE::COLOURS::GREEN);

	escape->render(renderer);

	//player score
	renderer->renderText("SCORE: ", 1060, 200, 0.75, ASGE::COLOURS::GREEN);

	std::string playerScore = std::to_string(player_one->getScore());

	renderer->renderText(
	playerScore.c_str(), 1155, 200, 0.75, ASGE::COLOURS::WHITE);

	//player score multiplier
	renderer->renderText("x", 1060, 250, 0.75, ASGE::COLOURS::GREEN);

	std::string playerMultiplier = std::to_string(player_one->getMultiplier());

	renderer->renderText(
	playerMultiplier.c_str(), 1090, 250, 0.75, ASGE::COLOURS::WHITE);

	//player lives
	renderer->renderText("LIVES: ", 1060, 350, 0.75, ASGE::COLOURS::GREEN);

	//show life sprites depending on player life
	for (int i = 0; i < player_one->getHealth(); i++)
	{
		lives[i]->player->render(renderer);
	}


}



/**
*   @brief   Processes any key inputs and translates them to a GameAction
*   @details This function is added as a callback to handle the game's 
			 input. Key presseses are translated in to GameActions which 
			 are then processed by the main game loop.
*   @param   key is the key the action relates to
*   @param   action whether the key was released or pressed
*   @return  void
*/
void InvadersGame::input(int key, int action) 
{
	if (game_state == GameState::GAME_OVER)
	{
		//can only return from game over screen
		if (key == ASGE::KEYS::KEY_ESCAPE)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				game_action = GameAction::RETURN;
			}

			if (action == ASGE::KEYS::KEY_RELEASED)
			{
				game_action = GameAction::NONE;
			}
		}
	}

	else if (game_state == GameState::OPTIONS)
	{
		//can only return from control screen
		if (key == ASGE::KEYS::KEY_ESCAPE)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				game_action = GameAction::RETURN;
			}

			if (action == ASGE::KEYS::KEY_RELEASED)
			{
				game_action = GameAction::NONE;
			}
		}
	}

	else if (game_state == GameState::MAIN_MENU)
	{
		//exit game
		if (key == ASGE::KEYS::KEY_ESCAPE)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				game_action = GameAction::EXIT;
			}

			if (action == ASGE::KEYS::KEY_RELEASED)
			{
				game_action = GameAction::NONE;
			}
		}

		//play game
		if (key == ASGE::KEYS::KEY_1)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				resetGame();

				game_action = GameAction::PLAY;
			}

			if (action == ASGE::KEYS::KEY_RELEASED)
			{
				game_action = GameAction::NONE;
			}
		}

		//show control scheme
		else if (key == ASGE::KEYS::KEY_2)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				game_action = GameAction::OPTIONS;
			}

			if (action == ASGE::KEYS::KEY_RELEASED)
			{
				game_action = GameAction::NONE;
			}
		}

		//exit game
		else if (key == ASGE::KEYS::KEY_3)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				game_action = GameAction::EXIT;
			}

			if (action == ASGE::KEYS::KEY_RELEASED)
			{
				game_action = GameAction::NONE;
			}
		}
	}

	//pause gameplay
	else if (game_state == GameState::PAUSE)
	{
		if (key == ASGE::KEYS::KEY_P)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				game_state = GameState::PLAYING;
			}

			if (action == ASGE::KEYS::KEY_RELEASED)
			{
				game_action = GameAction::NONE;
			}
		}
	}

	//return to main menu
	else if (game_state == GameState::PLAYING)
	{
		if (key == ASGE::KEYS::KEY_ESCAPE)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				game_action = GameAction::RETURN;
			}

			if (action == ASGE::KEYS::KEY_RELEASED)
			{
				game_action = GameAction::NONE;
			}
		}

		//pause
		if (key == ASGE::KEYS::KEY_P)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{				
				game_state = GameState::PAUSE;
			}

			if (action == ASGE::KEYS::KEY_RELEASED)
			{
				game_action = GameAction::NONE;
			}
		}

		//shoot if alive
		else if (key == ASGE::KEYS::KEY_SPACE)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				if (player_one->getDeath() == false)
				{
					game_action = GameAction::SHOOT;
				}
			}				

			else if (action == ASGE::KEYS::KEY_RELEASED)
			{
				game_action = GameAction::NONE;
			}
		}

		//movement if alive
		else if (key == ASGE::KEYS::KEY_A)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				if (player_one->getDeath() == false)
				{
					game_action = GameAction::LEFT;
				}
			}

		else if (action == ASGE::KEYS::KEY_RELEASED)
			{
				if (game_action == GameAction::LEFT)
				{
					if (player_one->getDeath() == false)
					{
						game_action = GameAction::NONE;
					}
				}
			}
		}

		else if (key == ASGE::KEYS::KEY_D)
		{
			if (action == ASGE::KEYS::KEY_PRESSED)
			{
				game_action = GameAction::RIGHT;
			}

			else if (action == ASGE::KEYS::KEY_RELEASED)
			{
				if (game_action == GameAction::RIGHT)
				{
					game_action = GameAction::NONE;
				}
			}
		}
	}
}



void InvadersGame::stateInput(int key, int action)
{
	//starting input on main menu first start up
	if (action == ASGE::KEYS::KEY_PRESSED)
	{
		if (game_state == GameState::MAIN_MENU)
		{
			//play new game
			if (key == ASGE::KEYS::KEY_1)
			{
				resetGame();

				game_action = GameAction::PLAY;

				game_callback_id = this->inputs->addCallbackFnc(
				&InvadersGame::input, this);
			}

			//show control screen
			else if (key == ASGE::KEYS::KEY_2)
			{
				game_action = GameAction::OPTIONS;

				game_callback_id = this->inputs->addCallbackFnc(
				&InvadersGame::input, this);
			}

			//exit
			else if ((key == ASGE::KEYS::KEY_3) || 
				(key == ASGE::KEYS::KEY_ESCAPE)) 
			{
				game_state = GameState::EXIT;
			}
		}
	}
}



/**
*   @brief   Processes the next game action
*   @details Uses the game action that was a direct result of
*            user input. It allows input to processed in a different
			 thread and the game actions performed in the main thread.
*   @return  void
*/
void InvadersGame::processGameActions()
{
	//exit program
	if (game_action == GameAction::EXIT)
	{
		game_state = GameState::EXIT;
	}

	//return to menu
	else if (game_action == GameAction::RETURN)
	{
		game_state = GameState::MAIN_MENU;
	}

	//play game
	else if (game_action == GameAction::PLAY)
	{
		game_state = GameState::PLAYING;
	}

	//show controls
	else if (game_action == GameAction::OPTIONS)
	{
		game_state = GameState::OPTIONS;
	}

	//un-pause game
	if (game_state == GameState::PAUSE)
	{
		if (game_action == GameAction::PAUSE)
		{
			game_action = GameAction::NONE;

			game_state = GameState::PLAYING;
		}
	}

	else if (game_state == GameState::PLAYING)
	{
		//pause game
		if (game_action == GameAction::PAUSE)
		{
			game_action = GameAction::NONE;

			game_state = GameState::PAUSE;
		}

		//move player sprite right
		if (game_action == GameAction::RIGHT)
		{
			if (player_one->getXPosition() < 990)
			{
				player_one->movePlayer(400 * time_difference);
			}
		}

		//move player sprite left
		else if (game_action == GameAction::LEFT)
		{
			if (player_one->getXPosition() > 10)
			{
				player_one->movePlayer(-300 * time_difference);
			}
		}

		//player shoot
		if (game_action == GameAction::SHOOT)
		{
			game_action = GameAction::NONE;

			if (bullet_one->getAlive() != true)
			{
				int x = player_one->getXPosition() + 24;
				int y = player_one->getYPosition();

				bullet_one->setBullet(x, y);

				audio_engine->play2D(
				"..\\..\\Resources\\Audio\\Laser1.wav", false);
			}
		}
	}
}



void InvadersGame::updateGame()
{
	beginFrame();
	renderer->setFont(GameFont::fonts[0]->id);	

	//check if player is currently respawning
	deathDelay();

	//check if player is dead
	checkPlayerAlive();

	//render GUI
	renderUI();

	//check what sprite each barrier should be showing
	changeBarriers();

	//render barrier sprites
	renderBarriers();

	//render player bullet
	bullet_one->bullet->render(renderer);

	//render alien sprites
	renderAliens();

	//move alien sprites
	moveAliens();

	//render mothership sprite
	renderMothership();

	//spawn and move mothership
	deployMothership();

	//change speed of aliens
	changeAlienSpeed();

	//move player bullet
	bullet_one->moveBullet(-800 * time_difference);

	//if player bullet missed then reset multipler
	if (bullet_one->getMissed() == true)
	{
		player_one->setMultiplier(1);
		bullet_one->setMissed(false);
	}

	//alien shooting
	enemyShoot();

	//aliens being shot
	checkCollision();

	//check if all aliens are dead
	checkAlienLives();

	//check if player has been shot
	checkPlayerCollision();

	//check if barriers have been shot
	checkBarrierCollision();

	//check if mothership has been shot
	checkMothershipCollision();

	//render explosion
	explosion->render(renderer);

	//hide explosion
	explosion->position[0] = -30;
	explosion->position[1] = -30;	

	//render enemy bullets
	renderBullets();

	processGameActions();
	endFrame();
}



const void InvadersGame::updateMenu()
{
	//main menu GUI
	beginFrame();
	renderer->setFont(GameFont::fonts[0]->id);
	renderer->renderText(
	"Space Invaders\n", 375, 325, 1.0, ASGE::COLOURS::WHITE);

	renderMenuUI();

	renderer->renderText(
	"Start\n\nControls\n\nExit", 375, 375, 1.0, ASGE::COLOURS::GREEN);
	invader->render(renderer);
	processGameActions();
	endFrame();
}



const void InvadersGame::updateOptions()
{
	//show control scheme
	beginFrame();
	renderer->setFont(GameFont::fonts[0]->id);
	renderer->renderText("CONTROLS", 300, 100, 1.0, ASGE::COLOURS::GREEN);
	loadControls();
	processGameActions();
	endFrame();
}



const void InvadersGame::updatePause()
{
	//pause screen
	beginFrame();
	renderer->setFont(GameFont::fonts[0]->id);

	renderUI();

	renderBarriers();

	checkPlayerAlive();

	bullet_one->bullet->render(renderer);
	explosion->render(renderer);

	renderAliens();

	renderMothership();

	renderBullets();

	renderer->renderText("PAUSED", 500, 325, 2, ASGE::COLOURS::WHITE);

	processGameActions();
	endFrame();
}



const void InvadersGame::updateGameOver()
{
	//game over screen
	beginFrame();
	renderer->setFont(GameFont::fonts[0]->id);

	renderUI();

	renderBarriers();

	checkPlayerAlive();

	bullet_one->bullet->render(renderer);
	explosion->render(renderer);

	renderAliens();

	renderMothership();

	renderBullets();

	renderer->renderText("GAME OVER", 500, 325, 2, ASGE::COLOURS::WHITE);

	processGameActions();
	endFrame();
}



void InvadersGame::loadEnemies()
{
	//spawn coords
	int pos_x = 300;
	int pos_y = 100;
	int counter = 0;

	//for each 55 aliens (5x11)
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			//add to vector and set position
			aliens.push_back(std::make_unique<Enemy>());
			aliens[counter]->loadEnemy(renderer);
			aliens[counter]->enemy->position[0] = pos_x;
			aliens[counter]->enemy->position[1] = pos_y;
			aliens[counter]->setDirection(10);

			//if top 2 rows then different sprite texture and size
			if (counter < 22)
			{
				aliens[counter]->enemy->loadTexture(
				"..\\..\\Resources\\Textures\\Alien2.png");

				aliens[counter]->enemy->scale = 0.12f;
				aliens[counter]->enemy->position[0] += 3;
			}

			//if spawning on bottom row then automatically can shoot
			if (counter > 43)
			{
				aliens[counter]->setCanShoot(true);
			}

			counter++;
			pos_x += 50;
		}
		pos_x = 300;
		pos_y += 40;
	}
}



void InvadersGame::loadBarriers()
{
	int pos_x = 200;

	for (int i = 0; i < 3; i++)
	{
		//add to vector and set position of each barrier
		barriers.push_back(std::make_unique<Barrier>());
		barriers[i]->loadBarrier(renderer);
		barriers[i]->barrier->position[0] = pos_x;
		barriers[i]->barrier->position[1] = 600;

		pos_x += 300;
	}
}



void InvadersGame::resetGame()
{
	//reset game actors for new game
	resetEnemies();

	resetBarriers();

	//reset movement speed
	alien_move_speed = 1.2;

	//spawn at specified y coord
	for (int i = 0; i < aliens.size(); i++)
	{
		aliens[i]->setStartY(100);
	}

	//reset player
	player_one->setAlive(true);
	player_one->setHealth(3);
	player_one->setScore(0);
	player_one->player->position[0] = 500;

	for (int i = 0; i < 5; i++)
	{
		bullets[i]->setAlive(false);
	}

	bullet_one->setAlive(false);
}



void InvadersGame::resetEnemies()
{
	//respawn enemies
	int pos_x = 300;
	int counter = 0;

	//this determines how far down they start spawning
	int pos_y = aliens[0]->getStartY();

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			aliens[counter]->setAlive(true);
			aliens[counter]->enemy->position[0] = pos_x;
			aliens[counter]->enemy->position[1] = pos_y;
			aliens[counter]->setDirection(10);
			aliens[counter]->setCanShoot(false);

			if (counter < 22)
			{
				aliens[counter]->enemy->position[0] += 3;
			}

			if (counter > 43)
			{
				aliens[counter]->setCanShoot(true);
			}

			counter++;
			pos_x += 50;
		}
		pos_x = 300;
		pos_y += 40;
	}
}



const void InvadersGame::resetBarriers()
{
	//reset barriers for new game
	for (int i = 0; i < barriers.size(); i++)
	{
		barriers[i]->resetBarrier();
	}
}



const void InvadersGame::changeBarriers()
{
	//change barrier sprites
	for (int i = 0; i < 3; i++)
	{
		barriers[i]->changeBarrier();
	}
}



const void InvadersGame::renderAliens()
{
	//render alien sprites if alive
	for (int i = 0; i < aliens.size(); i++)
	{
		if (aliens[i]->getAlive() == true)
		{
			aliens[i]->enemy->render(renderer);
		}
	}
}



const void InvadersGame::renderBarriers()
{
	//render barrier sprites if alive
	for (int i = 0; i < barriers.size(); i++)
	{
		if (barriers[i]->getAlive() == true)
		{
			barriers[i]->barrier->render(renderer);
		}
	}
}



void InvadersGame::moveAliens()
{
	//move enemy alien sprites

	//change direction boolean
	bool change = false;

	//movement tick
	alien_move_counter += time_difference;

	//if movement tick reaches threshold
	if (alien_move_counter >= 0.4)
	{
		for (int c = 0; c < aliens.size(); c++)
		{
			//if each alien is alive and is out of bounds then change direction
			if (aliens[c]->getAlive() == true)
			{
				if ((aliens[c]->getXPosition() < 30) || 
					(aliens[c]->getXPosition() > 970))
				{
					change = true;
				}
			}
		}

		if (change)
		{
			//if changing direction 
			for (int i = 0; i < aliens.size(); i++)
			{

				//if aliens are below certain threshold then game ends
				if (aliens[i]->getYPosition() > 620)
				{
					if (aliens[i]->getAlive() == true)
					{
						player_one->setAlive(false);
					}
				}

				//move alien sprite
				aliens[i]->enemy->position[1] += 30;

				//if alien is alive and over either 
				//threshold then bring back into bounds
				if (aliens[i]->getAlive() == true)
				{
					if (aliens[i]->getXPosition() < 30)
					{
						for (int c = 0; c < aliens.size(); c++)
						{
							aliens[c]->enemy->position[0] += 10;
						}
					}

					else if (aliens[i]->getXPosition() > 970)
					{
						for (int c = 0; c < aliens.size(); c++)
						{
							aliens[c]->enemy->position[0] -= 10;
						}
					}
				}

				//reset enemy alien movement direction to other way
				aliens[i]->setDirection(aliens[i]->getDirection() * -1);
			}
		}
			
		else
		{
			//if not changing direction then move as normal per tick
			for (int j = 0; j < aliens.size(); j++)
			{
				aliens[j]->enemy->position[0] += aliens[j]->getDirection();
			}
		}

		//reset tick counter
		alien_move_counter = 0;
	}
}



void InvadersGame::changeAlienSpeed()
{
	//increase enemy alien speed and shooting freuqency depending on height
	if ((aliens[0]->enemy->position[1] < 370) && 
		(aliens[0]->enemy->position[1] >= 190))
	{
		alien_move_speed = 0.8;
		alien_shoot_speed = 15000;
	}

	else if ((aliens[0]->enemy->position[1] < 430) && 
		(aliens[0]->enemy->position[1] >= 370))
	{
		alien_move_speed = 0.4;
		alien_shoot_speed = 10000;
	}

	else if (aliens[0]->enemy->position[1] >= 430)
	{
		alien_move_speed = 0.1;
		alien_shoot_speed = 5000;
	}
}



void InvadersGame::checkCollision()
{
	if (bullet_one->getAlive() == true)
	{
		for (int i = 0; i < aliens.size(); i++)
		{
			if (aliens[i]->getAlive() == true)
			{
				//if player bullet is live and each enemy alien is alive
				//if player bullet sprite crosses over 
				//screen space with alien sprite
				if ((aliens[i]->getXPosition() + 35) >= 
					bullet_one->getXPosition())
				{
					if (aliens[i]->getXPosition() <= 
						(bullet_one->getXPosition() + 5))
					{
						if ((aliens[i]->getYPosition() + 20) >= 
							bullet_one->getYPosition())
						{
							if (aliens[i]->getYPosition() <= 
								(bullet_one->getYPosition() + 5))
							{
								//show explosion, kill alien and bullet
								spawnExplosion(aliens[i]->getXPosition(), 
								aliens[i]->getYPosition());

								aliens[i]->setAlive(false);

								audio_engine->play2D(
								"..\\..\\Resources\\Audio\\
								Explosion2.wav", false);

								bullet_one->setAlive(false);

								//increase player score depending on alien
								int score_addition = 0;

								if (i < 22)
								{
									score_addition = 30;
								}

								else
								{
									score_addition = 10;
								}

								//multiple score by multiplier
								player_one->setScore(player_one->getScore() 
								+ (player_one->getMultiplier() * 
								score_addition));

								player_one->setMultiplier
								(player_one->getMultiplier() + 1);
							}
						}
					}
				}
			}
		}
	}
}



const void InvadersGame::checkAlienLives()
{
	//checking if all aliens are dead
	for (int i = 0; i < aliens.size(); i++)
	{
		if (aliens[i]->getAlive() == true)
		{
			return;
		}
	}
		
	//if wave cleared then give back a lost life
	if (player_one->getHealth() < 3)
	{
		player_one->setHealth(player_one->getHealth() + 1);
	}

	//spawn enemies lower each round
	if (aliens[0]->getStartY() < 450)
	{
		for (int i = 0; i < aliens.size(); i++)
		{
			aliens[i]->setStartY(aliens[i]->getStartY() + 50);
		}
	}

	resetEnemies();
}



void InvadersGame::spawnExplosion(int x, int y)
{
	//spawn explosion sprite at given location
	explosion->position[0] = x;
	explosion->position[1] = y;
}



const void InvadersGame::loadBullets()
{
	//load enemy bullet sprites
	for (int i = 0; i < 5; i++)
	{
		bullets.push_back(std::make_unique<Bullet>());
		bullets[i]->loadBullet(renderer);
	}
}



const void InvadersGame::renderBullets()
{
	//render enemy bullet spites
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->bullet->render(renderer);

		//move bullets
		if (game_state == GameState::PLAYING)
		{
			bullets[i]->moveBullet(650 * time_difference);
		}
	}
}



void InvadersGame::enemyShoot()
{
	//determine when enemies shoot
	for (int i = 0; i < aliens.size(); i++)
	{
		//if not bottom row & the alien below is dead but could shoot
		//then this alien can now shoot
		if (i < 43)
		{
			if ((aliens[i + 11]->getCanShoot() == true) && 
				(aliens[i + 11]->getAlive() == false))
			{
				aliens[i]->setCanShoot(true);
			}
		}

		if (aliens[i]->getCanShoot() == true)
		{
			for (int j = 0; j < bullets.size(); j++)
			{
				//for each bullet random chance to fire if not already
				std::random_device rd;
				std::mt19937 rng(rd());
				std::uniform_int_distribution<int> uni(0, alien_shoot_speed);

				auto randomNum = uni(rng);

				if (randomNum == 1)
				{
					if (bullets[j]->getAlive() == false)
					{
						if (aliens[i]->getAlive() == true)
						{
							//spawn bullet below selected alien 
							int x = aliens[i]->getXPosition() + 15;
							int y = aliens[i]->getYPosition() + 5;

							bullets[j]->setBullet(x, y);

							audio_engine->play2D(
							"..\\..\\Resources\\Audio\\Laser2.wav", false);
						}
					}
				}
			}
		}
	}
}



void InvadersGame::checkPlayerCollision()
{
	//check if any live alien bullet sprites overlap with player sprite
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getAlive() == true)
		{
			if ((player_one->getXPosition() + 50) >= 
				bullets[i]->getXPosition())
			{
				if (player_one->getXPosition() <= 
					(bullets[i]->getXPosition() + 5))
				{
					if ((player_one->getYPosition() + 20) >= 
						bullets[i]->getYPosition())
					{
						if (player_one->getYPosition() <= 
							(bullets[i]->getYPosition() + 5))
						{
							//if hit
							//kill bullet, spawn explosion
							bullets[i]->setAlive(false);

							spawnExplosion(player_one->getXPosition(), 
							player_one->getYPosition());

							//take life off of player and create respawn delay
							int hlth = player_one->getHealth();
							player_one->setDeath(true);
							player_one->setHealth(hlth - 1);
							player_one->setMultiplier(1);

							audio_engine->play2D(
							"..\\..\\Resources\\Audio\\Explosion1.wav", 
							false);

							//reset player position
							player_one->player->position[0] = 500;

							//if no more lives then player is dead
							if (player_one->getHealth() == 0)
							{
								player_one->setAlive(false);
							}
						}
					}
				}
			}
		}
	}
}



void InvadersGame::checkBarrierCollision()
{
	//check if any live barrier sprites overlap 
	//with any live enemy bullet sprites
	for (int j = 0; j < barriers.size(); j++)
	{
		if (barriers[j]->getAlive() == true)
		{
			for (int i = 0; i < bullets.size(); i++)
			{
				if (bullets[i]->getAlive() == true)
				{
					if ((barriers[j]->getXPosition() + 80) >= 
						bullets[i]->getXPosition())
					{
						if (barriers[j]->getXPosition() <= 
							(bullets[i]->getXPosition() + 5))
						{
							if ((barriers[j]->getYPosition() + 50) >= 
								bullets[i]->getYPosition())
							{
								if (barriers[j]->getYPosition() <= 
									(bullets[i]->getYPosition() + 5))
								{
									//kill bullet
									bullets[i]->setAlive(false);

									//spawn explosion
									spawnExplosion(barriers[j]->getXPosition(),
									barriers[j]->getYPosition());

									//take health from barrier
									int hlth = barriers[j]->getHealth();
									barriers[j]->setHealth
									(barriers[j]->getHealth() - 1);

									audio_engine->play2D(
									"..\\..\\Resources\\Audio\\
									Explosion2.wav", false);

									//if no barrier lives 
									//left then kill barrier
									if (barriers[j]->getHealth() == 0)
									{
										barriers[j]->setAlive(false);
									}
								}
							}
						}
					}
				}
			}

			//check if player bullet sprite overlaps with any live barriers
			if (bullet_one->getAlive() == true)
			{
				if ((barriers[j]->getXPosition() + 80) >= 
					bullet_one->getXPosition())
				{
					if (barriers[j]->getXPosition() <= 
						(bullet_one->getXPosition() + 5))
					{
						if ((barriers[j]->getYPosition() + 50) >= 
							bullet_one->getYPosition())
						{
							if (barriers[j]->getYPosition() <= 
								(bullet_one->getYPosition() + 5))
							{
								//kill player bullet, spawn explosion
								bullet_one->setAlive(false);

								spawnExplosion(barriers[j]->
								getXPosition(), 
								barriers[j]->getYPosition());

								//take health from barrier
								int hlth = barriers[j]->getHealth();
								barriers[j]->setHealth
								(barriers[j]->getHealth() - 1);

								audio_engine->play2D(
								"..\\..\\Resources\\Audio\\Explosion2.wav",
								false);

								//reset player multiplier for missing enemies
								player_one->setMultiplier(1);

								//if barrier is out of lives then kill
								if (barriers[j]->getHealth() == 0)
								{
									barriers[j]->setAlive(false);
								}
							}
						}
					}
				}
			}
		}
	}
}



const void InvadersGame::checkPlayerAlive()
{
	//check if player is still alive
	//otherwise end game
	if (player_one->getAlive() == false)
	{
		game_state = GameState::GAME_OVER;
	}

	//only render play if not in respawn delay
	else
	{
		if (player_one->getDeath() == false)
		{
			player_one->player->render(renderer);
		}
	}
}



const void InvadersGame::loadControls()
{
	//show control scheme sprites on screen
	renderer->renderText("RETURN", 1060, 100, 0.75, ASGE::COLOURS::GREEN);

	escape->render(renderer);

	renderer->renderText("PAUSE", 300, 300, 0.75, ASGE::COLOURS::GREEN);

	letterP->render(renderer);

	renderer->renderText("SHOOT", 300, 400, 0.75, ASGE::COLOURS::GREEN);

	space->render(renderer);

	renderer->renderText("MOVE", 300, 500, 0.75, ASGE::COLOURS::GREEN);

	left->render(renderer);
	right->render(renderer);
}



const void InvadersGame::renderMenuUI()
{
	//show menu button sprites 
	one->render(renderer);

	two->render(renderer);

	three->render(renderer);
}



void InvadersGame::deathDelay()
{
	//if player is hit then delay respawn
	if (player_one->getDeath() == true)
	{
		death_counter += time_difference;

		if (death_counter >= 1)
		{
			player_one->setDeath(false);

			death_counter = 0;
		}
	}
}



const void InvadersGame::renderMothership()
{
	//render mothership sprite
	if (mothership_one->getAlive() == true)
	{
		mothership_one->mothership->render(renderer);
	}
}



void InvadersGame::deployMothership()
{
	//if mothership not already going
	//spawn when time counter reaches threshold
	if (mothership_one->getAlive() == false)
	{
		mothership_spawn_timer += time_difference;

		if (mothership_spawn_timer >= 30)
		{
			mothership_one->setAlive(true);
			mothership_one->mothership->position[0] = -10;
			mothership_spawn_timer = 0;
		}
	}

	//if mothership alive then move across until out of bounds then kill
	else if (mothership_one->getAlive() == true)
	{
		if (mothership_one->getXPosition() > 990)
		{
			mothership_one->setAlive(false);
			return;
		}

		mothership_one->mothership->position[0] += (300 * time_difference);

		//play alarm sound
		playAlarm();
	}
}



void InvadersGame::checkMothershipCollision()
{
	//check if live mothership sprite overlaps with live player bullet sprite
	if (bullet_one->getAlive() == true)
	{
		for (int i = 0; i < aliens.size(); i++)
		{
			if (mothership_one->getAlive() == true)
			{
				if ((mothership_one->getXPosition() + 45) >= 
					bullet_one->getXPosition())
				{
					if (mothership_one->getXPosition() <= 
						(bullet_one->getXPosition() + 5))
					{
						if ((mothership_one->getYPosition() + 20) >= 
							bullet_one->getYPosition())
						{
							if (mothership_one->getYPosition() <=								
								(bullet_one->getYPosition() + 5))
							{
								//spawn explosion
								spawnExplosion(mothership_one->getXPosition(), 
								mothership_one->getYPosition());

								//kill mothersip and bullet
								mothership_one->setAlive(false);

								audio_engine->play2D(
								"..\\..\\Resources\\Audio\\
								Explosion2.wav", false);

								bullet_one->setAlive(false);

								//add score to player
								int score_addition = 200;

								player_one->setScore(player_one->getScore()
								+ (player_one->getMultiplier() * 
								score_addition));

								player_one->setMultiplier
								(player_one->getMultiplier() + 1);
							}
						}
					}
				}
			}
		}
	}
}



const bool InvadersGame::initAudio()
{
	//initialise irrKland audio engine
	using namespace irrklang;
	audio_engine.reset(createIrrKlangDevice());

	if (!audio_engine)
	{
		return false; // error starting up the engine
	}
	return true;
}



void InvadersGame::playAlarm()
{
	//play alarm on timer so it doesn't continuously play
	if (mothership_one->getAlive() == true)
	{
		alarm_counter += time_difference;
		if (alarm_counter >= 0.75)
		{
			audio_engine->play2D("..\\..\\Resources\\Audio\\Alarm.wav", false);
			alarm_counter = 0;
		}
	}
}
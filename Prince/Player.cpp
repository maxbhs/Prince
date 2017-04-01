#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 10
#define JUMP_HEIGHT 17
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, START_MOVE_LEFT, START_MOVE_RIGHT, STOP_MOVE_LEFT, STOP_MOVE_RIGHT,
	SWITCH_TO_LEFT, SWITCH_TO_RIGHT, SWITCH_TO_LEFT_RUNNING, SWITCH_TO_RIGHT_RUNNING, START_JUMP_LEFT_UP, START_JUMP_RIGHT_UP, JUMP_LEFT_UP, JUMP_RIGHT_UP,
	STOP_JUMP_LEFT_UP, STOP_JUMP_RIGHT_UP, START_JUMP_LEFT, START_JUMP_RIGHT, JUMP_LEFT, JUMP_RIGHT, STOP_JUMP_LEFT, STOP_JUMP_RIGHT, START_JUMP_LEFT_RUN, START_JUMP_RIGHT_RUN, JUMP_LEFT_RUN, JUMP_RIGHT_RUN, CLIMB_LEFT, CLIMB_RIGHT, CLIMB_LEFT_UP, CLIMB_RIGHT_UP,
	DOWN_LEFT, DOWN_RIGHT, STAND_DOWN_LEFT, STAND_DOWN_RIGHT, MOVE_DOWN_LEFT, MOVE_DOWN_RIGHT, UP_LEFT, UP_RIGHT, WALK_LEFT, WALK_RIGHT, START_WALK_LEFT,
	START_WALK_RIGHT, FALL_LEFT, FALL_RIGHT, LAND_LEFT, LAND_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	leftright = true; // true = left / false = right
	bJumping = false;
	bDown = false;
	bFalling = false;
	//canClimb = true;
	downPressed = false;
	upPressed = false;
	collLeft = false;
	collRight = false;
	spritesheet.loadFromFile("sprites/sprites-prince.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64,64), glm::vec2(0.05f, 0.05f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(48);


		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.95f, 0.0f));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));

		sprite->setAnimationSpeed(START_WALK_LEFT, 8);
		sprite->addKeyframe(START_WALK_LEFT, glm::vec2(0.95f, 0.0f));
		sprite->addKeyframe(START_WALK_LEFT, glm::vec2(0.95f, 0.0f));

		sprite->setAnimationSpeed(START_WALK_RIGHT, 8);
		sprite->addKeyframe(START_WALK_RIGHT, glm::vec2(0.0f, 0.0f));
		sprite->addKeyframe(START_WALK_RIGHT, glm::vec2(0.0f, 0.0f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		for (int i = 4; i <= 6; i++){
			sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (1 - (i / 20.0f)), 0.05f));
		}

		for (int i = 6; i > 3; i--){
			sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f + (1 - (i / 20.0f)), 0.05f));
		}

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		for (int i = 3; i < 6; i++){
			sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.05f));
		}
		for (int i = 5; i > 2; i--){
			sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.05f));
		}

		sprite->setAnimationSpeed(START_MOVE_LEFT, 8);
		for (int i = 1; i <= 3; i++){
			sprite->addKeyframe(START_MOVE_LEFT, glm::vec2(0.0f + (1 - (i / 20.0f)), 0.05f));
		}

		sprite->setAnimationSpeed(START_MOVE_RIGHT, 8);
		for (int i = 0; i < 3; i++){
			sprite->addKeyframe(START_MOVE_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.05f));
		}

		sprite->setAnimationSpeed(STOP_MOVE_LEFT, 8);
		for (int i = 8; i <= 10; i++){
			sprite->addKeyframe(STOP_MOVE_LEFT, glm::vec2(0.0f + (1 - (i / 20.0f)), 0.05f));
		}

		sprite->setAnimationSpeed(STOP_MOVE_RIGHT, 8);
		for (int i = 7; i < 10; i++){
			sprite->addKeyframe(STOP_MOVE_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.05f));
		}
		
		sprite->setAnimationSpeed(SWITCH_TO_LEFT, 8);
		for (int i = 1; i <= 7; i++){
			sprite->addKeyframe(SWITCH_TO_LEFT, glm::vec2(0.0f + 1 - (i / 20.0f), 0.5f));
		}
		
		sprite->setAnimationSpeed(SWITCH_TO_RIGHT, 8);
		for (int i = 0; i < 6; i++){
			sprite->addKeyframe(SWITCH_TO_RIGHT, glm::vec2(0.0f + (i / 20.0f), 0.5f));
		}

		sprite->setAnimationSpeed(SWITCH_TO_LEFT_RUNNING, 8);
		for (int i = 3; i < 6; i++){
			sprite->addKeyframe(SWITCH_TO_LEFT_RUNNING, glm::vec2(0.0f + (i / 20.0f), 0.55f));
		}

		sprite->setAnimationSpeed(SWITCH_TO_RIGHT_RUNNING, 8);
		for (int i = 4; i <= 7; i++){
			sprite->addKeyframe(SWITCH_TO_RIGHT_RUNNING, glm::vec2(0.0f + 1- (i / 20.0f), 0.55f));
		}

		sprite->setAnimationSpeed(JUMP_LEFT_UP, 8);
		sprite->addKeyframe(JUMP_LEFT_UP, glm::vec2(0.55, 0.6f));

		sprite->setAnimationSpeed(JUMP_RIGHT_UP, 8);
		sprite->addKeyframe(JUMP_RIGHT_UP, glm::vec2(0.4, 0.6f));
		

		sprite->setAnimationSpeed(START_JUMP_LEFT_UP, 8);
		for (int i = 1; i <= 8; i++){
			sprite->addKeyframe(START_JUMP_LEFT_UP, glm::vec2(0.f + 1 - (i / 20.0f), 0.6f));
		}

		sprite->setAnimationSpeed(START_JUMP_RIGHT_UP, 8);
		for (int i = 0; i < 7; i++){
			sprite->addKeyframe(START_JUMP_RIGHT_UP, glm::vec2(0.f + (i / 20.0f), 0.6f));
		}

		sprite->setAnimationSpeed(STOP_JUMP_LEFT_UP, 8);
		for (int i = 9; i <= 10; i++){
			sprite->addKeyframe(STOP_JUMP_LEFT_UP, glm::vec2(0.f + 1 - (i / 20.0f), 0.75f));
		}

		sprite->setAnimationSpeed(STOP_JUMP_RIGHT_UP, 8);
		for (int i = 8; i < 10; i++){
			sprite->addKeyframe(STOP_JUMP_RIGHT_UP, glm::vec2(0.f + (i / 20.0f), 0.75f));
		}

		sprite->setAnimationSpeed(JUMP_LEFT_RUN, 8);
		for (int i = 4; i <= 10; i++){
			sprite->addKeyframe(JUMP_LEFT_RUN, glm::vec2(0.f + (1 - (i / 20.0f)), 0.9f));
		}

		sprite->setAnimationSpeed(JUMP_RIGHT_RUN, 8);
		for (int i = 3; i < 10; i++){
			sprite->addKeyframe(JUMP_RIGHT_RUN, glm::vec2(0.f + (i / 20.0f), 0.9f));
		}

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		for (int i = 6; i <= 10; i++){
			sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.1f));
		}

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		for (int i = 5; i < 10; i++){
			sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.1f));
		}

		sprite->setAnimationSpeed(START_JUMP_LEFT_RUN, 8);
		for (int i = 1; i <= 3; i++){
			sprite->addKeyframe(START_JUMP_LEFT_RUN, glm::vec2(0.f + (1 - (i / 20.0f)), 0.9f));
		}

		sprite->setAnimationSpeed(START_JUMP_RIGHT_RUN, 8);
		for (int i = 0; i < 3; i++){
			sprite->addKeyframe(START_JUMP_RIGHT_RUN, glm::vec2(0.f + (i / 20.0f), 0.9f));
		}

		sprite->setAnimationSpeed(START_JUMP_LEFT, 8);
		for (int i = 1; i <= 5; i++){
			sprite->addKeyframe(START_JUMP_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.1f));
		}

		sprite->setAnimationSpeed(START_JUMP_RIGHT, 8);
		for (int i = 0; i < 5; i++){
			sprite->addKeyframe(START_JUMP_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.1f));
		}

		sprite->setAnimationSpeed(STOP_JUMP_LEFT, 8);
		for (int i = 1; i <= 6; i++){
			sprite->addKeyframe(STOP_JUMP_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.15f));
		}

		sprite->setAnimationSpeed(STOP_JUMP_RIGHT, 8);
		for (int i = 0; i < 6; i++){
			sprite->addKeyframe(STOP_JUMP_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.15f));
		}

		sprite->setAnimationSpeed(CLIMB_LEFT, 8);
		for (int i = 1; i <= 4; i++){
			sprite->addKeyframe(CLIMB_LEFT, glm::vec2(0.f + 1 - (i / 20.0f), 0.65f));
		}

		sprite->setAnimationSpeed(CLIMB_RIGHT, 8);
		for (int i = 0; i < 4; i++){
			sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.65f));
		}
		sprite->setAnimationSpeed(CLIMB_LEFT_UP, 8);
		for (int i = 5; i <= 10; i++){
			sprite->addKeyframe(CLIMB_LEFT_UP, glm::vec2(0.f + 1 - (i / 20.0f), 0.65f));
		}

		sprite->setAnimationSpeed(CLIMB_RIGHT_UP, 8);
		for (int i = 4; i < 10; i++){
			sprite->addKeyframe(CLIMB_RIGHT_UP, glm::vec2(0.f + (i / 20.0f), 0.65f));
		}

		sprite->setAnimationSpeed(DOWN_LEFT, 8);
		for (int i = 1; i <= 4; i++){
			sprite->addKeyframe(DOWN_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.25f));
		}

		sprite->setAnimationSpeed(DOWN_RIGHT, 8);
		for (int i = 0; i < 4; i++){
			sprite->addKeyframe(DOWN_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.25f));
		}
		sprite->setAnimationSpeed(STAND_DOWN_LEFT, 8);
		sprite->addKeyframe(STAND_DOWN_LEFT, glm::vec2(0.85f, 0.25f));

		sprite->setAnimationSpeed(STAND_DOWN_RIGHT, 8);
		sprite->addKeyframe(STAND_DOWN_RIGHT, glm::vec2(0.15f, 0.25f));

		sprite->setAnimationSpeed(MOVE_DOWN_LEFT, 8);
		for (int i = 4; i <= 5; i++){
			sprite->addKeyframe(MOVE_DOWN_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.25f));
		}
		sprite->setAnimationSpeed(MOVE_DOWN_RIGHT, 8);
		for (int i = 3; i < 5; i++){
			sprite->addKeyframe(MOVE_DOWN_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.25f));
		}
		sprite->setAnimationSpeed(UP_LEFT, 8);
		for (int i = 6; i <= 10; i++){
			sprite->addKeyframe(UP_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.25f));
		}

		sprite->setAnimationSpeed(UP_RIGHT, 8);
		for (int i = 5; i < 10; i++){
			sprite->addKeyframe(UP_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.25f));
		}

		sprite->setAnimationSpeed(WALK_LEFT, 8);
		for (int i = 1; i <= 8; i++){
			sprite->addKeyframe(WALK_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.75f));
		}

		sprite->setAnimationSpeed(WALK_RIGHT, 8);
		for (int i = 0; i < 8; i++){
			sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.75f));
		}

		sprite->setAnimationSpeed(FALL_LEFT, 8);
		for (int i = 1; i <= 3; i++){
			sprite->addKeyframe(FALL_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.2f));
		}

		sprite->setAnimationSpeed(FALL_RIGHT, 8);
		for (int i = 0; i < 3; i++){
			sprite->addKeyframe(FALL_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.2f));
		}

		sprite->setAnimationSpeed(LAND_LEFT, 8);
		for (int i = 1; i <= 6; i++){
			sprite->addKeyframe(LAND_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.15f));
		}

		sprite->setAnimationSpeed(LAND_RIGHT, 8);
		for (int i = 0; i < 6; i++){
			sprite->addKeyframe(LAND_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.15f));
		}
		
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !bJumping && !bFalling && !collLeft)
	{
		leftright = true;
		collRight = false;
		if (!bDown){
			if (!upPressed){
				if (sprite->animation() == STAND_LEFT){
					sprite->changeAnimation(START_WALK_LEFT);
				}
			}
			if (sprite->animation() == START_WALK_LEFT){
				if (sprite->timetoChange(START_WALK_LEFT)){
					sprite->changeAnimation(START_MOVE_LEFT);
				}
			}
			if (sprite->animation() == STAND_RIGHT){
				sprite->changeAnimation(SWITCH_TO_LEFT);
			}
			if (sprite->animation() == MOVE_RIGHT){
			
			sprite->changeAnimation(SWITCH_TO_LEFT_RUNNING);
			}
			if (sprite->animation() == START_MOVE_RIGHT){
				sprite->changeAnimation(SWITCH_TO_LEFT_RUNNING);
			}
			if (sprite->animation() == SWITCH_TO_RIGHT){
				sprite->changeAnimation(SWITCH_TO_LEFT);
			}
			if (sprite->animation() == SWITCH_TO_RIGHT_RUNNING){
				sprite->changeAnimation(SWITCH_TO_LEFT_RUNNING);
			}
			if (sprite->animation() == STOP_MOVE_RIGHT){
				sprite->changeAnimation(SWITCH_TO_LEFT_RUNNING);
			}
			if (sprite->animation() == SWITCH_TO_LEFT){
				if (sprite->timetoChange(SWITCH_TO_LEFT)){
					sprite->changeAnimation(START_MOVE_LEFT);
				}
			}
			if (sprite->animation() == SWITCH_TO_LEFT_RUNNING){
				if (sprite->timetoChange(SWITCH_TO_LEFT_RUNNING)){
					sprite->changeAnimation(MOVE_LEFT);
				}
			}
			if (sprite->animation() == START_MOVE_LEFT){
				if (sprite->timetoChange(START_MOVE_LEFT)){
					sprite->changeAnimation(MOVE_LEFT);
				}
			}
			if (sprite->animation() == STOP_MOVE_LEFT){
				if (sprite->timetoChange(STOP_MOVE_LEFT)){
					sprite->changeAnimation(START_MOVE_LEFT);
				}
			}
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !upPressed){
				upPressed = true;
			}
			if (!Game::instance().getSpecialKey(GLUT_KEY_UP) && upPressed){
				if (sprite->animation() == STAND_LEFT){
					bJumping = true;
					sprite->changeAnimation(START_JUMP_LEFT);
					upPressed = false;
				}
				else if (sprite->animation() == MOVE_LEFT){
					bJumping = true;
					sprite->changeAnimation(START_JUMP_LEFT_RUN);
					upPressed = false;
				}
			}
		}
		else{
			if (sprite->animation() != MOVE_DOWN_LEFT)
				sprite->changeAnimation(MOVE_DOWN_LEFT);
		}
		if (sprite->animation() == START_MOVE_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == START_JUMP_LEFT_RUN){
			posPlayer.x -= 1;
		}
		if (sprite->animation() == STOP_MOVE_LEFT || sprite->animation() == MOVE_DOWN_LEFT){
			posPlayer.x -= 1;
		}
		if (sprite->animation() == WALK_LEFT && sprite->getKeyframe(WALK_LEFT) == 5){
			posPlayer.x -= 2;
		}

		if (map->collisionMoveLeft(posPlayer, glm::ivec2(64, 64))){
			if (sprite->animation() == MOVE_LEFT){
				sprite->changeAnimation(STOP_MOVE_LEFT);
				posPlayer.x -= 1;
			}
			if (sprite->animation() == STOP_MOVE_LEFT){
				sprite->changeAnimation(STAND_LEFT);
				posPlayer.x -= 1;
			}
			if (sprite->animation() == JUMP_LEFT){
				sprite->changeAnimation(STOP_MOVE_LEFT);
				posPlayer.x -= 2;
			}
			if (sprite->animation() == JUMP_RIGHT_RUN){
				sprite->changeAnimation(STOP_MOVE_LEFT);
				posPlayer.x -= 2;
			}
			if (sprite->animation() == WALK_RIGHT){
				sprite->changeAnimation(STAND_LEFT);
				posPlayer.x -= 2;
			}
			collLeft = true;
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !bJumping && !bFalling && !collRight)
	{
		leftright = false;
		collLeft = false;
		if (!bDown){
			if (!upPressed){
				if (sprite->animation() == STAND_RIGHT){
					sprite->changeAnimation(START_WALK_RIGHT);
				}
			}
			if (sprite->animation() == START_WALK_RIGHT){
				if (sprite->timetoChange(START_WALK_RIGHT)){
					sprite->changeAnimation(START_MOVE_RIGHT);
				}
			}
			if (sprite->animation() == STAND_LEFT){
				sprite->changeAnimation(SWITCH_TO_RIGHT);
			}
			if (sprite->animation() == MOVE_LEFT){
				sprite->changeAnimation(SWITCH_TO_RIGHT_RUNNING);
			}
			if (sprite->animation() == START_MOVE_LEFT){
				sprite->changeAnimation(SWITCH_TO_RIGHT_RUNNING);
			}
			if (sprite->animation() == STOP_MOVE_LEFT){
				sprite->changeAnimation(SWITCH_TO_RIGHT_RUNNING);
			}
			if (sprite->animation() == SWITCH_TO_LEFT){
				sprite->changeAnimation(SWITCH_TO_RIGHT);
			}
			if (sprite->animation() == SWITCH_TO_LEFT_RUNNING){
				sprite->changeAnimation(SWITCH_TO_RIGHT_RUNNING);
			}
			if (sprite->animation() == SWITCH_TO_RIGHT){
				if (sprite->timetoChange(SWITCH_TO_RIGHT)){
					sprite->changeAnimation(START_MOVE_RIGHT);
				}
			}
			if (sprite->animation() == SWITCH_TO_RIGHT_RUNNING){
				if (sprite->timetoChange(SWITCH_TO_RIGHT_RUNNING)){
					sprite->changeAnimation(MOVE_RIGHT);
				}
			}
			if (sprite->animation() == START_MOVE_RIGHT){
				if (sprite->timetoChange(START_MOVE_RIGHT)){
					sprite->changeAnimation(MOVE_RIGHT);
				}
			}
			if (sprite->animation() == STOP_MOVE_RIGHT){
				if (sprite->timetoChange(STOP_MOVE_RIGHT)){
					sprite->changeAnimation(START_MOVE_RIGHT);
				}
			}
			if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !upPressed){
				upPressed = true;
			}
			if (!Game::instance().getSpecialKey(GLUT_KEY_UP) && upPressed){
				if (sprite->animation() == STAND_RIGHT){
					bJumping = true;
					sprite->changeAnimation(START_JUMP_RIGHT);
					upPressed = false;
				}
				else if (sprite->animation() == MOVE_RIGHT){
					bJumping = true;
					sprite->changeAnimation(START_JUMP_RIGHT_RUN);
					upPressed = false;
				}
			}
		}
		else{
			if (sprite->animation() != MOVE_DOWN_RIGHT)
				sprite->changeAnimation(MOVE_DOWN_RIGHT);
		}
		if (sprite->animation() == START_MOVE_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == START_JUMP_RIGHT_RUN){
			posPlayer.x += 1;
		}
		if (sprite->animation() == STOP_MOVE_RIGHT || sprite->animation() == MOVE_DOWN_RIGHT){
			posPlayer.x += 1;
		}
		if (sprite->animation() == WALK_RIGHT && sprite->getKeyframe(WALK_RIGHT) == 5){
			posPlayer.x += 2;
		}

		if (map->collisionMoveRight(posPlayer, glm::ivec2(64, 64))){
			if (sprite->animation() == MOVE_RIGHT){
				sprite->changeAnimation(STOP_MOVE_RIGHT);
				posPlayer.x -= 1;
			}
			if (sprite->animation() == STOP_MOVE_RIGHT){
				sprite->changeAnimation(STAND_RIGHT);
				posPlayer.x -= 1;
			}
			if (sprite->animation() == JUMP_RIGHT){
				sprite->changeAnimation(STAND_RIGHT);
				posPlayer.x -= 2;
			}
			if (sprite->animation() == JUMP_RIGHT_RUN){
				sprite->changeAnimation(STAND_RIGHT);
				posPlayer.x -= 2;
			}
			if (sprite->animation() == WALK_RIGHT){
				sprite->changeAnimation(STAND_RIGHT);
				posPlayer.x -= 2;
			}
			collRight = true;
		}
		else collRight = false;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !downPressed && !bFalling){
		downPressed = true;
	}
	else if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN) && downPressed && !bFalling){
		if (sprite->animation() == STAND_LEFT || sprite->animation() == START_MOVE_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == STOP_MOVE_LEFT || sprite->animation() == SWITCH_TO_LEFT){
			bDown = true;
			sprite->changeAnimation(DOWN_LEFT);
		}
		if (sprite->animation() == STAND_RIGHT || sprite->animation() == START_MOVE_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == STOP_MOVE_RIGHT || sprite->animation() == SWITCH_TO_RIGHT){
			bDown = true;
			sprite->changeAnimation(DOWN_RIGHT);
		}
		downPressed = false;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !upPressed && !bFalling){
		upPressed = true;
	}
	else if (!Game::instance().getSpecialKey(GLUT_KEY_UP) && upPressed && !bFalling){
		if (bDown){
			if (sprite->animation() == DOWN_LEFT || sprite->animation() == STAND_DOWN_LEFT || sprite->animation() == MOVE_DOWN_LEFT){
				sprite->changeAnimation(UP_LEFT);
			}
			if (sprite->animation() == DOWN_RIGHT || sprite->animation() == STAND_DOWN_RIGHT || sprite->animation() == MOVE_DOWN_RIGHT){
				sprite->changeAnimation(UP_RIGHT);
			}
		}
		else{
			if (sprite->animation() == STAND_LEFT){
				bJumping = true;
				sprite->changeAnimation(START_JUMP_LEFT_UP);
			}
			else if (sprite->animation() == STAND_RIGHT){
				bJumping = true;
				sprite->changeAnimation(START_JUMP_RIGHT_UP);
			}
			else {
				if (sprite->animation() == STAND_LEFT || sprite->animation() == START_WALK_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == STOP_MOVE_LEFT){
					bJumping = true;
					sprite->changeAnimation(START_JUMP_LEFT_RUN);
				}
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == START_WALK_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == STOP_MOVE_RIGHT){
					bJumping = true;
					sprite->changeAnimation(START_JUMP_RIGHT_RUN);
				}
			}
		}
		upPressed = false;
	}else {
		if (sprite->animation() == MOVE_LEFT){
			sprite->changeAnimation(STOP_MOVE_LEFT);
		}
		if (sprite->animation() == MOVE_RIGHT){
			sprite->changeAnimation(STOP_MOVE_RIGHT);
		}
		if (sprite->animation() == START_MOVE_LEFT){
			sprite->changeAnimation(STOP_MOVE_LEFT);
		}
		if (sprite->animation() == START_MOVE_RIGHT){
			sprite->changeAnimation(STOP_MOVE_RIGHT);
		}

		if (sprite->animation() == START_WALK_LEFT){
			sprite->changeAnimation(WALK_LEFT);
		}

		if (sprite->animation() == START_WALK_RIGHT){
			sprite->changeAnimation(WALK_RIGHT);
		}

		if(sprite->animation() == WALK_LEFT){
			if (!map->collisionMoveLeft(posPlayer, glm::ivec2(64, 64))){
				if (sprite->getKeyframe(WALK_RIGHT) == 5) posPlayer.x -= 2;
				else if (sprite->timetoChange(WALK_LEFT)){
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			else posPlayer.x += 2;
		}
		if (sprite->animation() == WALK_RIGHT){
			if (!map->collisionMoveRight(posPlayer, glm::ivec2(64, 64))){
				if (sprite->getKeyframe(WALK_RIGHT) == 5) posPlayer.x += 2;
				else if (sprite->timetoChange(WALK_RIGHT)){
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			else posPlayer.x -= 2;
		}
		
		if (sprite->animation() == STOP_MOVE_LEFT){
			if (sprite->timetoChange(STOP_MOVE_LEFT)){
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		if (sprite->animation() == STOP_MOVE_RIGHT){
			if (sprite->timetoChange(STOP_MOVE_RIGHT)){
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		if (sprite->animation() == SWITCH_TO_LEFT){
			if (sprite->timetoChange(SWITCH_TO_LEFT)){
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		if (sprite->animation() == SWITCH_TO_RIGHT){
			if (sprite->timetoChange(SWITCH_TO_RIGHT)){
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		if (sprite->animation() == SWITCH_TO_LEFT_RUNNING){
			if (sprite->timetoChange(SWITCH_TO_LEFT_RUNNING)){
				sprite->changeAnimation(STOP_MOVE_LEFT);
			}
		}
		if (sprite->animation() == SWITCH_TO_RIGHT_RUNNING){
			if (sprite->timetoChange(SWITCH_TO_RIGHT_RUNNING)){
				sprite->changeAnimation(STOP_MOVE_RIGHT);
			}
		}
		if (sprite->animation() == DOWN_LEFT){
			if (sprite->timetoChange(DOWN_LEFT)){
				sprite->changeAnimation(STAND_DOWN_LEFT);
			}
		}
		if (sprite->animation() == DOWN_RIGHT){
			if (sprite->timetoChange(DOWN_RIGHT)){
				sprite->changeAnimation(STAND_DOWN_RIGHT);
			}
		}
		if (sprite->animation() == UP_LEFT){
			if (sprite->timetoChange(UP_LEFT)){
				bDown = false;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		if (sprite->animation() == UP_RIGHT){
			if (sprite->timetoChange(UP_RIGHT)){
				bDown = false;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		if (sprite->animation() == MOVE_DOWN_LEFT){
			if (sprite->timetoChange(MOVE_DOWN_LEFT)){
				sprite->changeAnimation(STAND_DOWN_LEFT);
			}
		}
		if (sprite->animation() == MOVE_DOWN_RIGHT){
			if (sprite->timetoChange(MOVE_DOWN_RIGHT)){
				sprite->changeAnimation(STAND_DOWN_RIGHT);
			}
		}
		if (sprite->animation() == JUMP_LEFT_RUN){
			if (!map->collisionMoveLeft(posPlayer, glm::ivec2(64, 64))){
				posPlayer.x -= 2;
				jumpAngle += 4;
				if (jumpAngle == 180){
					posPlayer.y = startY;
					bJumping = false;
					sprite->changeAnimation(MOVE_LEFT);
				}
				else posPlayer.y = int(startY - 10 * sin(3.14159f * jumpAngle / 180.f));
			}
			else {
				sprite->changeAnimation(STAND_LEFT);
				posPlayer.x += 2;
				posPlayer.y = startY;
				bJumping = false;
			}
		}
		if (sprite->animation() == JUMP_RIGHT_RUN){
			if (!map->collisionMoveRight(posPlayer, glm::ivec2(64, 64))){
				posPlayer.x += 2;
				jumpAngle += 4;
				if (jumpAngle == 180){
					posPlayer.y = startY;
					bJumping = false;
					sprite->changeAnimation(MOVE_RIGHT);
				}
				else posPlayer.y = int(startY - 10 * sin(3.14159f * jumpAngle / 180.f));
			}
			else {
				sprite->changeAnimation(STAND_RIGHT);
				posPlayer.x -= 2;
				posPlayer.y = startY;
				bJumping = false;
			}
		}
		if (sprite->animation() == START_JUMP_LEFT_RUN){
			if (sprite->timetoChange(START_JUMP_LEFT_RUN)){
				jumpAngle = 0;
				startY = posPlayer.y;
				sprite->changeAnimation(JUMP_LEFT_RUN);
			}
			else posPlayer.x -= 1;
		}
		if (sprite->animation() == START_JUMP_RIGHT_RUN){
			if (sprite->timetoChange(START_JUMP_RIGHT_RUN)){
				jumpAngle = 0;
				startY = posPlayer.y;
				sprite->changeAnimation(JUMP_RIGHT_RUN);
			}
			else posPlayer.x += 1;
		}

		if (sprite->animation() == JUMP_LEFT){
			if (!map->collisionMoveLeft(posPlayer, glm::ivec2(64, 64))){
				posPlayer.x -= 1;
				jumpAngle += 4;
				if (jumpAngle == 180){
					posPlayer.y = startY;
					if (map->collisionMoveDown(posPlayer, glm::ivec2(64, 64))){
						sprite->changeAnimation(STOP_JUMP_LEFT);
					}
					else {
						sprite->changeAnimation(FALL_LEFT);
						bJumping = false;
					}
				}
				else posPlayer.y = int(startY - 10 * sin(3.14159f * jumpAngle / 180.f));
			}
			else {
				sprite->changeAnimation(STAND_LEFT);
				posPlayer.x += 1;
				posPlayer.y = startY;
				bJumping = false;
			}
		}
		if (sprite->animation() == JUMP_RIGHT){
			if (!map->collisionMoveRight(posPlayer, glm::ivec2(64, 64))){
				posPlayer.x += 1;
				jumpAngle += 4;
				if (jumpAngle == 180){
					posPlayer.y = startY;
					if (map->collisionMoveDown(posPlayer, glm::ivec2(64, 64))){
						sprite->changeAnimation(STOP_JUMP_RIGHT);
					}
					else {
						sprite->changeAnimation(FALL_RIGHT);
						bJumping = false;
					}
				}
				else posPlayer.y = int(startY - 10 * sin(3.14159f * jumpAngle / 180.f));
			}
			else{
				sprite->changeAnimation(STAND_RIGHT);
				posPlayer.x -= 1;
				posPlayer.y = startY;
				bJumping = false;
			}
		}

		if (sprite->animation() == START_JUMP_LEFT){
			if (sprite->timetoChange(START_JUMP_LEFT)){
				jumpAngle = 0;
				startY = posPlayer.y;
				sprite->changeAnimation(JUMP_LEFT);
			}
		}
		if (sprite->animation() == START_JUMP_RIGHT){
			if (sprite->timetoChange(START_JUMP_RIGHT)){
				jumpAngle = 0;
				startY = posPlayer.y;
				sprite->changeAnimation(JUMP_RIGHT);
			}
		}

		if (sprite->animation() == STOP_JUMP_LEFT){
			if (!map->collisionMoveLeft(posPlayer, glm::ivec2(64, 64))){
				if (sprite->timetoChange(STOP_JUMP_LEFT)){
					bJumping = false;
					sprite->changeAnimation(STAND_LEFT);
				}
				else if (sprite->getKeyframe(STOP_JUMP_LEFT) == 2){
					posPlayer.x -= 1;
				}
			}
			else{
				bJumping = false;
				posPlayer.x += 1;
			}
		}
		if (sprite->animation() == STOP_JUMP_RIGHT){
			if (!map->collisionMoveRight(posPlayer, glm::ivec2(64, 64))){
				if (sprite->timetoChange(STOP_JUMP_RIGHT)){
					bJumping = false;
					sprite->changeAnimation(STAND_RIGHT);
				}
				else if (sprite->getKeyframe(STOP_JUMP_RIGHT) == 2){
					posPlayer.x += 1;
				}
			}
			else {
				bJumping = false;
				posPlayer.x += 1;
			}
		}

		if (sprite->animation() == START_JUMP_LEFT_UP){
			if (sprite->timetoChange(START_JUMP_LEFT_UP)){
				jumpAngle = 0;
				startY = posPlayer.y;
				sprite->changeAnimation(JUMP_LEFT_UP);
			}
		}
		if (sprite->animation() == START_JUMP_RIGHT_UP){
			if (sprite->timetoChange(START_JUMP_RIGHT_UP)){
				jumpAngle = 0;
				startY = posPlayer.y;
				sprite->changeAnimation(JUMP_RIGHT_UP);
			}
		}

		if (sprite->animation() == JUMP_LEFT_UP){
				if (bJumping){
				jumpAngle += JUMP_ANGLE_STEP;
				if (jumpAngle == 180){
					posPlayer.y = startY;
					sprite->changeAnimation(STOP_JUMP_LEFT_UP);
				}
				else{
					posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
					if (map->collisionMoveUp(posPlayer,glm::ivec2(64,64),true) && jumpAngle == 90){
						sprite->changeAnimation(CLIMB_LEFT);
					}
				}
			}
		}
		if (sprite->animation() == JUMP_RIGHT_UP){
			if (bJumping){
				jumpAngle += JUMP_ANGLE_STEP;
				if (jumpAngle == 180){
					posPlayer.y = startY;
					sprite->changeAnimation(STOP_JUMP_RIGHT_UP);
				}
				else{
					posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
					if (map->collisionMoveUp(posPlayer,glm::ivec2(64, 64),false) && jumpAngle == 90){
						sprite->changeAnimation(CLIMB_RIGHT);
					}
				}
			}
		}
		if (sprite->animation() == STOP_JUMP_LEFT_UP){
			if (sprite->timetoChange(STOP_JUMP_LEFT_UP)){
				bJumping = false;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		if (sprite->animation() == STOP_JUMP_RIGHT_UP){
			if (sprite->timetoChange(STOP_JUMP_RIGHT_UP)){
				bJumping = false;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		if (sprite->animation() == CLIMB_LEFT){
			if (sprite->timetoChange(CLIMB_LEFT)){
				sprite->changeAnimation(CLIMB_LEFT_UP);
			} else posPlayer.y -= 2;
			
		}
		if (sprite->animation() == CLIMB_RIGHT){
			if (sprite->timetoChange(CLIMB_RIGHT)){
				sprite->changeAnimation(CLIMB_RIGHT_UP);
			}
			else posPlayer.y -= 2;
		}
		if (sprite->animation() == CLIMB_LEFT_UP){
			if (sprite->timetoChange(CLIMB_LEFT_UP)){
				bJumping = false;
				collLeft = false;
				sprite->changeAnimation(STAND_LEFT);
			}
			else if (sprite->getKeyframe(CLIMB_LEFT_UP) == 3 ){
				posPlayer.x -= 2;
			}
		}
		if (sprite->animation() == CLIMB_RIGHT_UP){
			if (sprite->timetoChange(CLIMB_RIGHT_UP)){
				bJumping = false;
				collRight = false;
				sprite->changeAnimation(STAND_RIGHT);
			}
			else if (sprite->getKeyframe(CLIMB_RIGHT_UP) == 3 ){
				posPlayer.x += 2;
			}
		}
		if (sprite->animation() == LAND_LEFT){
			if (sprite->timetoChange(LAND_LEFT)){
				bFalling = false;
				bDown = false;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		if (sprite->animation() == LAND_RIGHT){
			if (sprite->timetoChange(LAND_RIGHT)){
				bDown = false;
				bFalling = false;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
	}
	if (!bJumping){
		if (!map->collisionMoveDown(posPlayer, glm::ivec2(64, 64))){
			if (leftright){
				sprite->changeAnimation(FALL_LEFT);
			}
			else {
				sprite->changeAnimation(FALL_RIGHT);
			}
			posPlayer.y += FALL_STEP;
			bFalling = true;
		}
		else {
			if (sprite->animation() == FALL_LEFT) sprite->changeAnimation(LAND_LEFT);
			else if (sprite->animation() == FALL_RIGHT)sprite->changeAnimation(LAND_RIGHT);
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition()
{
	glm::vec2 p = glm::vec2(float(tileMapDispl.x+posPlayer.x), float(tileMapDispl.y+posPlayer.y));
	return p;
}







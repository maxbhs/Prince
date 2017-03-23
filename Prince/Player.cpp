#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, START_MOVE_LEFT, START_MOVE_RIGHT, STOP_MOVE_LEFT, STOP_MOVE_RIGHT,
	SWITCH_TO_LEFT, SWITCH_TO_RIGHT, SWITCH_TO_LEFT_RUNNING, SWITCH_TO_RIGHT_RUNNING, JUMP_LEFT_UP, JUMP_RIGHT_UP, JUMP_LEFT, JUMP_RIGHT, CLIMB_LEFT, CLIMB_RIGHT,
	DOWN_LEFT, DOWN_RIGHT, STAND_DOWN_LEFT, STAND_DOWN_RIGHT, MOVE_DOWN_LEFT, MOVE_DOWN_RIGHT, UP_LEFT, UP_RIGHT, FALL_LEFT, FALL_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	bDown = false;
	spritesheet.loadFromFile("sprites/sprites-prince.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64,64), glm::vec2(0.05f, 0.05f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(28);


		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.95f, 0.0f));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));

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
		for (int i = 1; i <= 10; i++){
			sprite->addKeyframe(JUMP_LEFT_UP, glm::vec2(0.f + 1 - (i / 20.0f), 0.6f));
		}

		sprite->setAnimationSpeed(JUMP_RIGHT_UP, 8);
		for (int i = 0; i < 10; i++){
			sprite->addKeyframe(JUMP_RIGHT_UP, glm::vec2(0.f + (i / 20.0f), 0.6f));
		}

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		for (int i = 1; i <= 10; i++){
			sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.f + (1- (i / 20.0f)), 0.9f));
		}

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		for (int i = 0; i < 10; i++){
			sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.9f));
		}

		sprite->setAnimationSpeed(CLIMB_LEFT, 8);
		for (int i = 1; i <= 10; i++){
			sprite->addKeyframe(CLIMB_LEFT, glm::vec2(0.f + 1- (i / 20.0f), 0.65f));
		}
		for (int i = 1; i <= 1; i++){
		sprite->addKeyframe(CLIMB_LEFT, glm::vec2(0.f + 1 - (i / 20.0f), 0.7f));
		}

		sprite->setAnimationSpeed(CLIMB_RIGHT, 8);
		for (int i = 0; i < 10; i++){
			sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.65f));
		}
		for (int i = 0; i < 1; i++){
			sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.7f));
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
		///////////
		sprite->setAnimationSpeed(MOVE_DOWN_LEFT, 8);
		for (int i = 4; i <= 5; i++){
			sprite->addKeyframe(MOVE_DOWN_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.25f));
		}
		sprite->setAnimationSpeed(MOVE_DOWN_RIGHT, 8);
		for (int i = 3; i < 5; i++){
			sprite->addKeyframe(MOVE_DOWN_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.25f));
		}
		///////////////
		sprite->setAnimationSpeed(UP_LEFT, 8);
		for (int i = 6; i <= 10; i++){
			sprite->addKeyframe(UP_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.25f));
		}

		sprite->setAnimationSpeed(UP_RIGHT, 8);
		for (int i = 5; i < 10; i++){
			sprite->addKeyframe(UP_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.25f));
		}
		
		/*sprite->setAnimationSpeed(FALL_LEFT, 8);
		for (int i = 1; i <= 4; i++){
			sprite->addKeyframe(FALL_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.2f));
		}

		sprite->setAnimationSpeed(FALL_RIGHT, 8);
		for (int i = 0; i < 4; i++){
			sprite->addKeyframe(FALL_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.2f));
		}*/
	
		
		

		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (!bDown){
			if (sprite->animation() == STAND_LEFT){
				sprite->changeAnimation(START_MOVE_LEFT);
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
		}
		else {
			if (sprite->animation() != MOVE_DOWN_LEFT)
				sprite->changeAnimation(MOVE_DOWN_LEFT);

		}
		/*if (Game::instance().getSpecialKey(GLUT_KEY_UP)){
			sprite->changeAnimation(JUMP_LEFT);
			}
			if (sprite->timetoChange(JUMP_LEFT)){
			sprite->changeAnimation(MOVE_LEFT);
			}*/

		if (sprite->animation() == START_MOVE_LEFT || sprite->animation() == MOVE_LEFT){
			posPlayer.x -= 2;
		}
		if (sprite->animation() == STOP_MOVE_LEFT || sprite->animation() == MOVE_DOWN_LEFT){
			posPlayer.x -= 1;
		}
		/*if (sprite->animation() == JUMP_LEFT){
			posPlayer.x -= 3;
			//posPlayer.y -= 1;
			}*/

		if (map->collisionMoveLeft(posPlayer, glm::ivec2(64, 64))){
			if (sprite->animation() == MOVE_LEFT){
				sprite->changeAnimation(STOP_MOVE_LEFT);
			}
			if (sprite->animation() == STOP_MOVE_LEFT){
				sprite->changeAnimation(STAND_LEFT);
			}
			posPlayer.x += 2;
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (!bDown){
			if (sprite->animation() == STAND_RIGHT){
				sprite->changeAnimation(START_MOVE_RIGHT);
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
		}
		else{
			if (sprite->animation() != MOVE_DOWN_RIGHT)
				sprite->changeAnimation(MOVE_DOWN_RIGHT);
		}
		/*if (Game::instance().getSpecialKey(GLUT_KEY_UP)){
			sprite->changeAnimation(JUMP_RIGHT);
			}
			if (sprite->timetoChange(JUMP_RIGHT)){
			sprite->changeAnimation(MOVE_RIGHT);
			}*/

		if (sprite->animation() == START_MOVE_RIGHT || sprite->animation() == MOVE_RIGHT){
			posPlayer.x += 2;
		}
		if (sprite->animation() == STOP_MOVE_RIGHT || sprite->animation() == MOVE_DOWN_RIGHT){
			posPlayer.x += 1;
		}
		/*if (sprite->animation() == JUMP_RIGHT){
			posPlayer.x += 3;
			//posPlayer.y -= 1;
			}*/

		if (map->collisionMoveRight(posPlayer, glm::ivec2(64, 64))){
			if (sprite->animation() == MOVE_RIGHT){
				sprite->changeAnimation(STOP_MOVE_RIGHT);
			}
			if (sprite->animation() == STOP_MOVE_RIGHT){
				sprite->changeAnimation(STAND_RIGHT);
			}
			posPlayer.x -= 2;

		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)){
		if (sprite->animation() == STAND_LEFT || sprite->animation() == START_MOVE_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == STOP_MOVE_LEFT || sprite->animation() == SWITCH_TO_LEFT){
			sprite->changeAnimation(DOWN_LEFT);
			bDown = true;
		}
		if (sprite->animation() == STAND_RIGHT || sprite->animation() == START_MOVE_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == STOP_MOVE_RIGHT || sprite->animation() == SWITCH_TO_RIGHT){
			sprite->changeAnimation(DOWN_RIGHT);
			bDown = true;
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)){
		if (bDown){
			if (sprite->animation() == DOWN_LEFT || sprite->animation() == STAND_DOWN_LEFT || sprite->animation() == MOVE_DOWN_LEFT){
				sprite->changeAnimation(UP_LEFT);
				bDown = false;
			}
			if (sprite->animation() == DOWN_RIGHT || sprite->animation() == STAND_DOWN_RIGHT || sprite->animation() == MOVE_DOWN_RIGHT){
				sprite->changeAnimation(UP_RIGHT);
				bDown = false;
			}
		}
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
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		if (sprite->animation() == UP_RIGHT){
			if (sprite->timetoChange(UP_RIGHT)){
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
		}/*
	if (sprite->animation() == JUMP_LEFT){
	if (sprite->timetoChange(JUMP_LEFT)){
	sprite->changeAnimation(STOP_MOVE_LEFT);
	}
	}
	if (sprite->animation() == JUMP_RIGHT){
	if (sprite->timetoChange(JUMP_RIGHT)){
	sprite->changeAnimation(STOP_MOVE_RIGHT);
	}
	}*/
	}
	/*
	else {
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	if (sprite->animation() != JUMP_FALL) last = sprite->animation();

	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
			sprite->changeAnimation(last);
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90){
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y);
				if (!bJumping) sprite->changeAnimation(last);
			}
		}
	}
	else 
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y))
		{
			if (sprite->animation() != last) sprite->changeAnimation(last);
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
				sprite->changeAnimation(JUMP_FALL);
			}
		}
		else{
			if (sprite->animation() != JUMP_FALL) sprite->changeAnimation(JUMP_FALL);
		}
	}*/
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





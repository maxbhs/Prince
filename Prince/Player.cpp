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
	START_WALK_RIGHT, START_FALL_LEFT, START_FALL_RIGHT, FALL_LEFT, FALL_RIGHT, LAND_LEFT, LAND_RIGHT, START_FIGHT_LEFT, START_FIGHT_RIGHT, STOP_FIGHT_LEFT, STOP_FIGHT_RIGHT, ATTACK_LEFT, ATTACK_RIGHT,
	PARRI_LEFT, PARRI_RIGHT, IDLE_FIGHT_LEFT, IDLE_FIGHT_RIGHT, DIE_LEFT, DIE_RIGHT, RIP_LEFT, RIP_RIGHT, HEAL_LEFT, HEAL_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	leftright = false; // true = left / false = right
	bJumping = false;
	bDown = false;
	bFalling = false;
	downPressed = false;
	upPressed = false;
	collLeft = false;
	collRight = false;
	bHealing = false;

	lastkey = 0;
	contkey = 0;
	diffposM = 0;
	lastposM = glm::ivec2(0,0);

	vida = 6;
	bFighting = false;
	efePressed = false;
	cPressed = false;

	spritesheet.loadFromFile("sprites/sprites-prince.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64,64), glm::vec2(0.05f, 0.05f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(66);


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

		sprite->setAnimationSpeed(START_FALL_LEFT, 8);
		for (int i = 1; i <= 2; i++){
			sprite->addKeyframe(START_FALL_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.2f));
		}

		sprite->setAnimationSpeed(START_FALL_RIGHT, 8);
		for (int i = 0; i < 2; i++){
			sprite->addKeyframe(START_FALL_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.2f));
		}

		sprite->setAnimationSpeed(FALL_LEFT, 8);
		sprite->addKeyframe(FALL_LEFT, glm::vec2(0.f + (1 - (3 / 20.0f)), 0.2f));

		sprite->setAnimationSpeed(FALL_RIGHT, 8);
		sprite->addKeyframe(FALL_RIGHT, glm::vec2(0.f + (2 / 20.0f), 0.2f));

		sprite->setAnimationSpeed(LAND_LEFT, 8);
		for (int i = 1; i <= 6; i++){
			sprite->addKeyframe(LAND_LEFT, glm::vec2(0.f + (1 - (i / 20.0f)), 0.15f));
		}

		sprite->setAnimationSpeed(LAND_RIGHT, 8);
		for (int i = 0; i < 6; i++){
			sprite->addKeyframe(LAND_RIGHT, glm::vec2(0.f + (i / 20.0f), 0.15f));
		}
		
		sprite->setAnimationSpeed(START_FIGHT_LEFT, 8);
		for (int i = 7; i >=3; i--){
			sprite->addKeyframe(START_FIGHT_LEFT, glm::vec2(1 - (i / 20.0f), 0.35f));
		}

		sprite->setAnimationSpeed(START_FIGHT_RIGHT, 8);
		for (int i = 8; i > 3; i--){
			sprite->addKeyframe(START_FIGHT_RIGHT, glm::vec2(i / 20.0f, 0.35f));
		}

		sprite->setAnimationSpeed(STOP_FIGHT_LEFT, 8);
		for (int i = 5; i <= 9; i++){
			sprite->addKeyframe(STOP_FIGHT_LEFT, glm::vec2(1 - (i / 20.0f), 0.35f));
		}

		sprite->setAnimationSpeed(STOP_FIGHT_RIGHT, 8);
		for (int i = 4; i < 9; i++){
			sprite->addKeyframe(STOP_FIGHT_RIGHT, glm::vec2(i / 20.0f, 0.35f));
		}

		sprite->setAnimationSpeed(IDLE_FIGHT_LEFT, 2);
		sprite->addKeyframe(IDLE_FIGHT_LEFT, glm::vec2(1-(5 / 20.0f), 0.4f));
		sprite->addKeyframe(IDLE_FIGHT_LEFT, glm::vec2(1-(1 / 20.0f), 0.45f));

		sprite->setAnimationSpeed(IDLE_FIGHT_RIGHT, 2);
	    sprite->addKeyframe(IDLE_FIGHT_RIGHT, glm::vec2(4 / 20.0f, 0.4f));
		sprite->addKeyframe(IDLE_FIGHT_RIGHT, glm::vec2(0 / 20.0f, 0.45f));

		sprite->setAnimationSpeed(ATTACK_LEFT, 8);
		for(int i = 3; i <= 8; i++){
			sprite->addKeyframe(ATTACK_LEFT, glm::vec2(1 - (i / 20.0f), 0.45f));
		}

		sprite->setAnimationSpeed(ATTACK_RIGHT, 8);
		for(int i = 2; i < 8; i++){
			sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(i / 20.0f, 0.45f));
		}

		sprite->setAnimationSpeed(PARRI_LEFT, 8);
		sprite->addKeyframe(PARRI_LEFT, glm::vec2(1 - (4/ 20.0f), 0.4f));

		sprite->setAnimationSpeed(PARRI_RIGHT, 8);
		sprite->addKeyframe(PARRI_RIGHT, glm::vec2(3 / 20.0f, 0.4f));

		sprite->setAnimationSpeed(DIE_LEFT, 8);
		for (int i = 1; i <= 4; i++){
			sprite->addKeyframe(DIE_LEFT, glm::vec2(1 - (i / 20.0f), 0.95f));
		}

		sprite->setAnimationSpeed(DIE_RIGHT, 8);
		for (int i = 0; i < 4; i++){
			sprite->addKeyframe(DIE_RIGHT, glm::vec2(i / 20.0f, 0.95f));
		}

		sprite->setAnimationSpeed(RIP_LEFT, 8);
		sprite->addKeyframe(RIP_LEFT, glm::vec2(1 - (5 / 20.0f), 0.95f));

		sprite->setAnimationSpeed(RIP_RIGHT, 8);
		sprite->addKeyframe(RIP_RIGHT, glm::vec2(4 / 20.0f, 0.95f));

		sprite->setAnimationSpeed(HEAL_LEFT, 8);
		for (int i = 1; i <= 9; i++){
			sprite->addKeyframe(HEAL_LEFT, glm::vec2(1 - (i / 20.0f), 0.8f));
		}

		sprite->setAnimationSpeed(HEAL_RIGHT, 8);
		for (int i = 0; i < 9; i++){
			sprite->addKeyframe(HEAL_RIGHT, glm::vec2(i / 20.0f, 0.8f));
		}
			

		
	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime, glm::ivec2 &posM)
{
	sprite->update(deltaTime);
	if (vida > 0){
		if (Game::instance().getKey(32) && !efePressed && !bJumping && !bFalling && !bDown && !bHealing){
			efePressed = true;
		}
		else if (!Game::instance().getKey(32) && efePressed && bFighting && !bHealing){
			efePressed = false;
			if (leftright) sprite->changeAnimation(STOP_FIGHT_LEFT);
			else sprite->changeAnimation(STOP_FIGHT_RIGHT);
		}
		else if (!Game::instance().getKey(32) && efePressed && !bFighting && !bHealing) {
			bFighting = true;
			efePressed = false;
			if (leftright){
				sprite->changeAnimation(START_FIGHT_LEFT);
			}
			else sprite->changeAnimation(START_FIGHT_RIGHT);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !bJumping && !bFalling && !collLeft && !bFighting && !bHealing)
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
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && bFighting && !bHealing){
			if (sprite->animation() == START_FIGHT_LEFT){
				if (sprite->timetoChange(START_FIGHT_LEFT)){
					sprite->changeAnimation(IDLE_FIGHT_LEFT);
				}
			}
			if (sprite->animation() == START_FIGHT_RIGHT){
				if (sprite->timetoChange(START_FIGHT_RIGHT)){
					sprite->changeAnimation(IDLE_FIGHT_RIGHT);
				}
			}
			if (sprite->animation() == STOP_FIGHT_LEFT){
				if (sprite->timetoChange(STOP_FIGHT_LEFT)){
					bFighting = false;
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			if (sprite->animation() == STOP_FIGHT_RIGHT){
				if (sprite->timetoChange(STOP_FIGHT_RIGHT)){
					bFighting = false;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			if (sprite->animation() == ATTACK_LEFT){
				if (sprite->timetoChange(ATTACK_LEFT)){
					sprite->changeAnimation(IDLE_FIGHT_LEFT);
				}
			}
			if (sprite->animation() == ATTACK_RIGHT){
				if (sprite->timetoChange(ATTACK_RIGHT)){
					sprite->changeAnimation(IDLE_FIGHT_RIGHT);
				}
			}
			if (sprite->animation() == PARRI_LEFT){
				if (sprite->timetoChange(PARRI_LEFT)){
					sprite->changeAnimation(IDLE_FIGHT_LEFT);
				}
			}
			if (sprite->animation() == PARRI_RIGHT){
				if (sprite->timetoChange(PARRI_RIGHT)){
					sprite->changeAnimation(IDLE_FIGHT_RIGHT);
				}
			}
			if (sprite->animation() == IDLE_FIGHT_LEFT || sprite->animation() == IDLE_FIGHT_RIGHT) posPlayer.x -= 1;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(64, 64))){
				posPlayer.x += 1;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !bJumping && !bFalling && !collRight && !bFighting && !bHealing)
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
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && bFighting && !bHealing){
			if (sprite->animation() == START_FIGHT_LEFT){
				if (sprite->timetoChange(START_FIGHT_LEFT)){
					sprite->changeAnimation(IDLE_FIGHT_LEFT);
				}
			}
			if (sprite->animation() == START_FIGHT_RIGHT){
				if (sprite->timetoChange(START_FIGHT_RIGHT)){
					sprite->changeAnimation(IDLE_FIGHT_RIGHT);
				}
			}
			if (sprite->animation() == STOP_FIGHT_LEFT){
				if (sprite->timetoChange(STOP_FIGHT_LEFT)){
					bFighting = false;
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			if (sprite->animation() == STOP_FIGHT_RIGHT){
				if (sprite->timetoChange(STOP_FIGHT_RIGHT)){
					bFighting = false;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			if (sprite->animation() == ATTACK_LEFT){
				if (sprite->timetoChange(ATTACK_LEFT)){
					sprite->changeAnimation(IDLE_FIGHT_LEFT);
				}
			}
			if (sprite->animation() == ATTACK_RIGHT){
				if (sprite->timetoChange(ATTACK_RIGHT)){
					sprite->changeAnimation(IDLE_FIGHT_RIGHT);
				}
			}
			if (sprite->animation() == PARRI_LEFT){
				if (sprite->timetoChange(PARRI_LEFT)){
					sprite->changeAnimation(IDLE_FIGHT_LEFT);
				}
			}
			if (sprite->animation() == PARRI_RIGHT){
				if (sprite->timetoChange(PARRI_RIGHT)){
					sprite->changeAnimation(IDLE_FIGHT_RIGHT);
				}
			}
			if (sprite->animation() == IDLE_FIGHT_LEFT || sprite->animation() == IDLE_FIGHT_RIGHT)posPlayer.x += 1;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(64, 64))){
				posPlayer.x -= 1;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !downPressed && !bFalling && !bHealing){
			downPressed = true;
		}
		else if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN) && downPressed && !bFalling && !bHealing){
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
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && bFighting && !bHealing){
			if (leftright){
				if (sprite->animation() != PARRI_LEFT){
					sprite->changeAnimation(PARRI_LEFT);
				}
			}
			else {
				if (sprite->animation() != PARRI_RIGHT){
					sprite->changeAnimation(PARRI_RIGHT);
				}
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !upPressed && !bFalling && !bHealing){
			upPressed = true;
		}
		else if (!Game::instance().getSpecialKey(GLUT_KEY_UP) && upPressed && !bFalling && !bHealing){
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
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && bFighting && !bHealing){
			if (leftright){
				if (sprite->animation() != ATTACK_LEFT){
					sprite->changeAnimation(ATTACK_LEFT);
				}
			}
			else {
				if (sprite->animation() != ATTACK_RIGHT){
					sprite->changeAnimation(ATTACK_RIGHT);
				}
			}
		}
		else if (bFighting){
			if (sprite->animation() == START_FIGHT_LEFT){
				if (sprite->timetoChange(START_FIGHT_LEFT)){
					sprite->changeAnimation(IDLE_FIGHT_LEFT);
				}
			}
			if (sprite->animation() == START_FIGHT_RIGHT){
				if (sprite->timetoChange(START_FIGHT_RIGHT)){
					sprite->changeAnimation(IDLE_FIGHT_RIGHT);
				}
			}
			if (sprite->animation() == STOP_FIGHT_LEFT){
				if (sprite->timetoChange(STOP_FIGHT_LEFT)){
					bFighting = false;
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			if (sprite->animation() == STOP_FIGHT_RIGHT){
				if (sprite->timetoChange(STOP_FIGHT_RIGHT)){
					bFighting = false;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			if (sprite->animation() == ATTACK_LEFT){
				if (sprite->timetoChange(ATTACK_LEFT)){
					sprite->changeAnimation(IDLE_FIGHT_LEFT);
				}
			}
			if (sprite->animation() == ATTACK_RIGHT){
				if (sprite->timetoChange(ATTACK_RIGHT)){
					sprite->changeAnimation(IDLE_FIGHT_RIGHT);
				}
			}
			if (sprite->animation() == PARRI_LEFT){
				if (sprite->timetoChange(PARRI_LEFT)){
					sprite->changeAnimation(IDLE_FIGHT_LEFT);
				}
			}
			if (sprite->animation() == PARRI_RIGHT){
				if (sprite->timetoChange(PARRI_RIGHT)){
					sprite->changeAnimation(IDLE_FIGHT_RIGHT);
				}
			}
		}
		else {
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

			if (sprite->animation() == WALK_LEFT){
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
						if (map->collisionMoveDown(posPlayer, glm::ivec2(64, 64), &posPlayer.y)){
							sprite->changeAnimation(MOVE_LEFT);
						}
						else {
							sprite->changeAnimation(FALL_LEFT);
							bJumping = false;
							bFighting = false;
						}
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
						if (map->collisionMoveDown(posPlayer, glm::ivec2(64, 64), &posPlayer.y)){
							sprite->changeAnimation(MOVE_RIGHT);
						}
						else {
							sprite->changeAnimation(FALL_RIGHT);
							bJumping = false;
							bFighting = false;
						}
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
						if (map->collisionMoveDown(posPlayer, glm::ivec2(64, 64), &posPlayer.y)){
							sprite->changeAnimation(STOP_JUMP_LEFT);
						}
						else {
							sprite->changeAnimation(FALL_LEFT);
							bJumping = false;
							bFighting = false;
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
						if (map->collisionMoveDown(posPlayer, glm::ivec2(64, 64), &posPlayer.y)){
							sprite->changeAnimation(STOP_JUMP_RIGHT);
						}
						else {
							sprite->changeAnimation(FALL_RIGHT);
							bJumping = false;
							bFighting = false;
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
						if (map->collisionMoveUp(posPlayer, glm::ivec2(64, 64), true) && jumpAngle == 90){
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
						if (map->collisionMoveUp(posPlayer, glm::ivec2(64, 64), false) && jumpAngle == 90){
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
				}
				else posPlayer.y -= 2;

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
				else if (sprite->getKeyframe(CLIMB_LEFT_UP) == 3){
					posPlayer.x -= 1;
				}
			}
			if (sprite->animation() == CLIMB_RIGHT_UP){
				if (sprite->timetoChange(CLIMB_RIGHT_UP)){
					bJumping = false;
					collRight = false;
					sprite->changeAnimation(STAND_RIGHT);
				}
				else if (sprite->getKeyframe(CLIMB_RIGHT_UP) == 1){
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
			if (sprite->animation() == HEAL_LEFT){
				if (sprite->timetoChange(HEAL_LEFT)){
					bHealing = false;
					if (vida < 6) vida += 1;
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			if (sprite->animation() == HEAL_RIGHT){
				if (sprite->timetoChange(HEAL_RIGHT)){
					bHealing = false;
					if (vida < 6) vida += 1;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			if (sprite->animation() == START_FALL_LEFT){
				if (sprite->timetoChange(START_FALL_LEFT)){
					sprite->changeAnimation(FALL_LEFT);
				}
				else if (sprite->getKeyframe(START_FALL_LEFT) == 0) posPlayer.x -= 1;
			}
			if (sprite->animation() == START_FALL_RIGHT){
				if (sprite->timetoChange(START_FALL_RIGHT)){
					sprite->changeAnimation(FALL_RIGHT);
				}
				else if (sprite->getKeyframe(START_FALL_RIGHT) == 0)posPlayer.x += 1;
			}
		}
		if (!bJumping){
			if (!map->collisionMoveDown(posPlayer, glm::ivec2(64, 64), &posPlayer.y)){
				if (leftright){
					if (sprite->animation() != START_FALL_LEFT && sprite->animation() != FALL_LEFT)sprite->changeAnimation(START_FALL_LEFT);
					bFighting = false;
				}
				else {
					if (sprite->animation() != START_FALL_RIGHT && sprite->animation() != FALL_RIGHT)sprite->changeAnimation(START_FALL_RIGHT);
					bFighting = false;
				}
				if (!bFalling) startY = posPlayer.y;
				posPlayer.y += FALL_STEP;
				bFalling = true;
			}
			else {

				if (sprite->animation() == FALL_LEFT) {
					if (posPlayer.y + (diffposM * 189) - startY > 5 * map->getTileSize().y - 1){
						vida = 0;
						sprite->changeAnimation(DIE_LEFT);
					}
					else if (posPlayer.y + (diffposM * 189) - startY > 2 * map->getTileSize().y - 1){
						vida -= 1;
						if (vida == 0) sprite->changeAnimation(DIE_LEFT);
						else sprite->changeAnimation(LAND_LEFT);
					}
					else sprite->changeAnimation(LAND_LEFT);
					diffposM = 0;
				}
				else if (sprite->animation() == FALL_RIGHT){
					if (posPlayer.y + (diffposM * 189) - startY > 5 * map->getTileSize().y - 1){
						vida = 0;
						sprite->changeAnimation(DIE_RIGHT);
					}
					else if (posPlayer.y + (diffposM * 189) - startY > 2 * map->getTileSize().y - 1){
						vida -= 1;
						if (vida == 0) sprite->changeAnimation(DIE_RIGHT);
						else sprite->changeAnimation(LAND_RIGHT);
					}
					else sprite->changeAnimation(LAND_RIGHT);
					diffposM = 0;
				}
			}
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
	else {
		if (sprite->animation() == DIE_LEFT){
			if (sprite->timetoChange(DIE_LEFT)){
				sprite->changeAnimation(RIP_LEFT);
				sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y+9)));
			}

		}
		else if (sprite->animation() == DIE_RIGHT){
			if (sprite->timetoChange(DIE_RIGHT)){
				sprite->changeAnimation(RIP_RIGHT);
				sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y+9)));
			}

		}
	}
}

void Player::update(int deltaTime, int v){
	if (v == 1){
		if (leftright){
			if (sprite->animation() != HEAL_LEFT){
				sprite->changeAnimation(HEAL_LEFT);
				bHealing = true;
			}
		}
		else {
			if (sprite->animation() != HEAL_RIGHT){
				sprite->changeAnimation(HEAL_RIGHT);
				bHealing = true;
			}
		}
	}
}

void Player::update(int deltaTime, glm::vec2 posTrap, int anim){
	if (vida > 0){
		if (anim == 0){
			if (int(posPlayer.x + 32 / 16) == int(posTrap.x + 32 / 16)){
				vida = 0;
				if (leftright){
					if (sprite->animation() != DIE_LEFT)
						sprite->changeAnimation(DIE_LEFT);
				}
				else {
					if (sprite->animation() != DIE_RIGHT)
						sprite->changeAnimation(DIE_RIGHT);
				}
			}
		}
	}
}
void Player::update(int deltaTime, glm::vec2 posEnemy, int anim, int key){
	if (lastkey == key) ++contkey;
	else {
		lastkey = key;
		contkey = 0;
	}
	if (vida > 0){
		if (!leftright){
			if (bFighting){
				if (Game::instance().getKey(int('c')) && !cPressed) {
					cPressed = true;
				}
				else if (!Game::instance().getKey(int('c')) && cPressed){
					if (sprite->animation() == STAND_RIGHT)
						sprite->changeAnimation(IDLE_FIGHT_LEFT);
					if (sprite->animation() == IDLE_FIGHT_RIGHT)
						sprite->changeAnimation(IDLE_FIGHT_LEFT);
					if (sprite->animation() == ATTACK_RIGHT)
						sprite->changeAnimation(IDLE_FIGHT_LEFT);

					leftright = true;
					cPressed = false;
				}
			}
			if (sqrt(pow(posEnemy.x - posPlayer.x, 2) + pow(posEnemy.y - posPlayer.y, 2)) < 3*32){
				if ((anim == 7 || anim == 6) && key == 2 && contkey == 0){
					//if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) posPlayer.x -= 1;
					if (sprite->animation() != PARRI_RIGHT){
						vida -= 1;
						if (vida == 0) sprite->changeAnimation(DIE_RIGHT);
					}
				}
			}
		}
		else {
			if (bFighting){
				if (Game::instance().getKey(int('c')) && !cPressed) {
					cPressed = true;
				}
				else if (!Game::instance().getKey(int('c')) && cPressed){
					if (sprite->animation() == STAND_LEFT)
						sprite->changeAnimation(IDLE_FIGHT_RIGHT);
					if (sprite->animation() == IDLE_FIGHT_LEFT)
						sprite->changeAnimation(IDLE_FIGHT_RIGHT);
					if (sprite->animation() == ATTACK_LEFT)
						sprite->changeAnimation(IDLE_FIGHT_RIGHT);
					leftright = false;
					cPressed = false;
				}
			}
			if (sqrt(pow(posEnemy.x - posPlayer.x, 2) + pow(posEnemy.y - posPlayer.y, 2)) < 3 * 32){
				if ((anim == 7 || anim == 6) && key == 2 && contkey == 0){
					//if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) posPlayer.x -= 1;
					if (sprite->animation() != PARRI_LEFT){
						vida -= 1;
						if (vida == 0) sprite->changeAnimation(DIE_LEFT);
					}
				}
			}
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
	else {
		 if (sprite->animation() == DIE_RIGHT){
			 if (sprite->timetoChange(DIE_RIGHT)){
				 sprite->changeAnimation(RIP_RIGHT);
				 sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y+9)));
			 }
		}
	}
}

void Player::ha_caido() {
	diffposM += 1;
}

bool Player::saltando() {
	return bJumping;
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
	glm::vec2 p = (glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	return p;
}

glm::ivec2 Player::getPositionTile()
{
	glm::ivec2 p = glm::ivec2((posPlayer.x + 16) / 32, (posPlayer.y + 32) / 63);
	return p;
}

int Player::getVida(){
	return vida;
}

int Player::getCurrentAnim(){
	return sprite->animation();
}

int Player::getCurrentKeyframe(int animId){
	return sprite->getKeyframe(animId);
}







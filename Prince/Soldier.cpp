#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Soldier.h"
#include "Game.h"
#include "Scene.h"

enum SoldierAnims{
	STAND_LEFT, STAND_RIGHT, IDLE_LEFT, IDLE_RIGHT, PARRI_LEFT, PARRI_RIGHT, ATTACK_LEFT, ATTACK_RIGHT,
	FALL_LEFT, FALL_RIGHT, DIE_LEFT, DIE_RIGHT, REST_DIE_LEFT, REST_DIE_RIGHT
};

void Soldier::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool b){

	leftright = b;
	vida = 4;
	cd = 0;
	lastkey = 0;
	contkey = 0;
    
	spritesheet.loadFromFile("sprites/sprites-soldier.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.0625f, 0.0625f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(14);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(1 - (1*0.0625), 0.0f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));


	sprite->setAnimationSpeed(IDLE_LEFT, 2);
	for (int i = 3; i <= 4; i++){
		sprite->addKeyframe(IDLE_LEFT, glm::vec2(1-(i*0.0625), 0.0f));
	}

	sprite->setAnimationSpeed(IDLE_RIGHT, 2);
	for (int i = 2; i < 4; i++){
		sprite->addKeyframe(IDLE_RIGHT, glm::vec2(i*0.0625, 0.0f));
	}

	sprite->setAnimationSpeed(PARRI_LEFT, 16);
	sprite->addKeyframe(PARRI_LEFT, glm::vec2(1 - (1*0.0625), 0.0625f));
	sprite->addKeyframe(PARRI_LEFT, glm::vec2(1 - (1 * 0.0625), 0.0625f));
	sprite->addKeyframe(PARRI_LEFT, glm::vec2(1 - (1 * 0.0625), 0.0625f));
	sprite->addKeyframe(PARRI_LEFT, glm::vec2(1 - (1 * 0.0625), 0.0625f));

	sprite->setAnimationSpeed(PARRI_RIGHT, 16);
	sprite->addKeyframe(PARRI_RIGHT, glm::vec2(0*0.0625, 0.0625f));
	sprite->addKeyframe(PARRI_RIGHT, glm::vec2(0 * 0.0625, 0.0625f));
	sprite->addKeyframe(PARRI_RIGHT, glm::vec2(0 * 0.0625, 0.0625f));
	sprite->addKeyframe(PARRI_RIGHT, glm::vec2(0 * 0.0625, 0.0625f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 8);
	for (int i = 2; i <= 5; i++){
		sprite->addKeyframe(ATTACK_LEFT, glm::vec2(1 - (i*0.0625), 0.0625f));
	}

	sprite->setAnimationSpeed(ATTACK_RIGHT, 8);
	for (int i = 1; i < 5; i++){
		sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(i*0.0625, 0.0625f));
	}

	sprite->setAnimationSpeed(FALL_LEFT, 8);
	for (int i = 2; i <= 2; i++){
		sprite->addKeyframe(FALL_LEFT, glm::vec2(1 - (i*0.0625), 3*0.0625f));
	}

	sprite->setAnimationSpeed(FALL_RIGHT, 8);
	for (int i = 1; i < 2; i++){
		sprite->addKeyframe(FALL_RIGHT, glm::vec2(i*0.0625, 3*0.0625f));
	}

	sprite->setAnimationSpeed(DIE_LEFT, 8);
	for (int i = 3; i <= 7; i++){
		sprite->addKeyframe(DIE_LEFT, glm::vec2(1 - (i*0.0625), 3*0.0625f));
	}

	sprite->setAnimationSpeed(DIE_RIGHT, 8);
	for (int i = 2; i < 7; i++){
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(i*0.0625, 3*0.0625f));
	}

	sprite->setAnimationSpeed(REST_DIE_LEFT, 8);
    sprite->addKeyframe(REST_DIE_LEFT, glm::vec2(1 - (7*0.0625), 3 * 0.0625f));
	

	sprite->setAnimationSpeed(REST_DIE_RIGHT, 8);
    sprite->addKeyframe(REST_DIE_RIGHT, glm::vec2(6*0.0625, 3 * 0.0625f));

	if (leftright)sprite->changeAnimation(0);
	else sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSoldier.x), float(tileMapDispl.y + posSoldier.y)));
}

void Soldier::update(int deltaTime, glm::vec2 posPlayer, int anim, int key, int vidaPlayer){
	sprite->update(deltaTime);
	if (lastkey == key) ++contkey;
	else {
		lastkey = key;
		contkey = 0;
	}
	if (vida > 0 && vidaPlayer > 0){
		if (leftright){
			if (posPlayer.x >= posSoldier.x && sqrt(pow(posPlayer.x - posSoldier.x, 2) + pow(posPlayer.y - posSoldier.y, 2)) < 3 * 32){
				if (sprite->animation() == STAND_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == IDLE_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == ATTACK_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				leftright = false;
			}
			else if (sqrt(pow(posPlayer.x - posSoldier.x, 2) + pow(posPlayer.y - posSoldier.y, 2)) < 3* 32){
				if (cd == 0){
					if (sprite->animation() != ATTACK_LEFT)sprite->changeAnimation(ATTACK_LEFT);
					cd = 100;
				}
				else if (anim == 55 && key == 4 && contkey == 0){
					int x = rand() % 4; 
					if (x == 0){ 
						sprite->changeAnimation(PARRI_LEFT);
					}
					else {
						vida -= 1;
						if (vida <= 0) sprite->changeAnimation(DIE_LEFT);
						posSoldier.x += 16;
					}
				}
				else {
					if (sprite->animation() == ATTACK_LEFT){
						if (sprite->timetoChange(ATTACK_LEFT))
							sprite->changeAnimation(IDLE_LEFT);
					}
					if (sprite->animation() == PARRI_LEFT){
						if (sprite->timetoChange(PARRI_LEFT))
							sprite->changeAnimation(IDLE_LEFT);
					}
					cd--;
				}
			}
			else if (posPlayer.x >= posSoldier.x && sqrt(pow(posPlayer.x - posSoldier.x, 2) + pow(posPlayer.y - posSoldier.y, 2)) < 5 * 32){
				if (sprite->animation() == STAND_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == IDLE_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == ATTACK_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				leftright = false;
			}
			else if (sqrt(pow(posPlayer.x - posSoldier.x, 2) + pow(posPlayer.y - posSoldier.y, 2)) < 5 * 32){
				if (sprite->animation() == STAND_LEFT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == ATTACK_LEFT)
					sprite->changeAnimation(IDLE_LEFT);
			}
			if (!map->collisionMoveDown(posSoldier, glm::ivec2(64, 64), &posSoldier.y)){
				if (sprite->animation() != FALL_RIGHT)
					sprite->changeAnimation(FALL_RIGHT);
				posSoldier.y += 4;
			}
			else {
				if (sprite->animation() == FALL_RIGHT){
					vida = 0;
					sprite->changeAnimation(DIE_RIGHT);
				}
			}
		}
		else {
			if (posPlayer.x <= posSoldier.x && sqrt(pow(posPlayer.x - posSoldier.x, 2) + pow(posPlayer.y - posSoldier.y, 2)) < 5 * 32){
				if (sprite->animation() == STAND_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == IDLE_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == ATTACK_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				leftright = true;
			}
			else if (sqrt(pow(posPlayer.x - posSoldier.x, 2) + pow(posPlayer.y - posSoldier.y, 2)) < 3 * 32){
				if (cd == 0){
					if (sprite->animation() != ATTACK_RIGHT)sprite->changeAnimation(ATTACK_RIGHT);
					cd = 100;
				}
				else if (anim == 54 && key == 4 && contkey == 0){
					int x = rand() % 4;
					if (x == 0){
						sprite->changeAnimation(PARRI_RIGHT);
					}
					else {
						vida -= 1;
						if (vida <= 0) sprite->changeAnimation(DIE_RIGHT);
						posSoldier.x -= 16;
					}
				}
				else {
					if (sprite->animation() == ATTACK_RIGHT){
						if (sprite->timetoChange(ATTACK_RIGHT))
							sprite->changeAnimation(IDLE_RIGHT);
					}
					if (sprite->animation() == PARRI_RIGHT){
						if (sprite->timetoChange(PARRI_RIGHT))
							sprite->changeAnimation(IDLE_RIGHT);
					}
					cd--;
				}
			}
			else if (posPlayer.x <= posSoldier.x && sqrt(pow(posPlayer.x - posSoldier.x, 2) + pow(posPlayer.y - posSoldier.y, 2)) < 5 * 32){
				if (sprite->animation() == STAND_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == IDLE_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == ATTACK_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				leftright = true;
			}
			else if (sqrt(pow(posPlayer.x - posSoldier.x, 2) + pow(posPlayer.y - posSoldier.y, 2)) < 5 * 32){
				if (sprite->animation() == STAND_RIGHT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == ATTACK_RIGHT)
					sprite->changeAnimation(IDLE_RIGHT);
			}
			if (!map->collisionMoveDown(posSoldier, glm::ivec2(64, 64), &posSoldier.y)){
				if (sprite->animation() != FALL_LEFT)
					sprite->changeAnimation(FALL_LEFT);
				posSoldier.y += 4;
			}
			else {
				if (sprite->animation() == FALL_LEFT){
					vida = 0;
					sprite->changeAnimation(DIE_LEFT);
				}
			}
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSoldier.x), float(tileMapDispl.y + posSoldier.y)));
	}
	else if (vidaPlayer <= 0){
		if (leftright){
			if (sprite->animation() != REST_DIE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
		}
		else{
			if (sprite->animation() != REST_DIE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}
	}
	if (sprite->animation() == DIE_LEFT){
		if (sprite->timetoChange(DIE_LEFT))
			sprite->changeAnimation(REST_DIE_LEFT);
	}
	else if (sprite->animation() == DIE_RIGHT){
		if (sprite->timetoChange(DIE_RIGHT))
			sprite->changeAnimation(REST_DIE_RIGHT);
	}
}

void Soldier::update(int deltaTime, glm::vec2 posTrap, int anim, int key){
	if (vida > 0){
		if (anim == 0 && (key == 3 || key == 4 || key == 5 || key == 6 || key == 7)){
			if (((posSoldier.x + 32 <= posTrap.x + 31 + 16) && (posSoldier.x + 32 >= posTrap.x + 31 + 10)) && (int((posSoldier.y + 32) / 64) == int((posTrap.y + 32) / 64 + 1))){
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

void Soldier::render(){
	sprite->render();
}

void Soldier::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}
void Soldier::setPosition(const glm::vec2 &pos)
{
	posSoldier = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSoldier.x), float(tileMapDispl.y + posSoldier.y)));
}

glm::vec2 Soldier::getPosition()
{
	glm::vec2 p = (glm::vec2(float(tileMapDispl.x + posSoldier.x), float(tileMapDispl.y + posSoldier.y)));
	return p;
}

glm::ivec2 Soldier::getPositionTile()
{
	glm::ivec2 p = glm::ivec2((posSoldier.x + 16) / 32, (posSoldier.y + 32) / 63);
	return p;
}

int Soldier::getVida(){
	return vida;
}

int Soldier::getCurrentAnim(){
	return sprite->animation();
}

int Soldier::getCurrentKeyframe(int animId){
	return sprite->getKeyframe(animId);
}

void Soldier::setLeftRight(bool b){
	leftright = b;
}

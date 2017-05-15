#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Boss.h"
#include "Game.h"
#include "Scene.h"

enum BossAnims{
	STAND_LEFT, STAND_RIGHT, IDLE_LEFT, IDLE_RIGHT, PARRI_LEFT, PARRI_RIGHT, ATTACK_LEFT, ATTACK_RIGHT,
	FALL_LEFT, FALL_RIGHT, DIE_LEFT, DIE_RIGHT, REST_DIE_LEFT, REST_DIE_RIGHT, ATTACK_LEFT_ULTI, ATTACK_RIGHT_ULTI
};

void Boss::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool b){

	leftright = b;
	vida = 5;
	cd = 0;
	cdulti = 450;
	lastkey = 0;
	contkey = 0;

	spritesheet.loadFromFile("sprites/sprites-boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.0625f, 0.0625f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(16);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(1 - (1 * 0.0625), 0.0f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));


	sprite->setAnimationSpeed(IDLE_LEFT, 2);
	for (int i = 3; i <= 4; i++){
		sprite->addKeyframe(IDLE_LEFT, glm::vec2(1 - (i*0.0625), 0.0f));
	}

	sprite->setAnimationSpeed(IDLE_RIGHT, 2);
	for (int i = 2; i < 4; i++){
		sprite->addKeyframe(IDLE_RIGHT, glm::vec2(i*0.0625, 0.0f));
	}

	sprite->setAnimationSpeed(PARRI_LEFT, 16);
	sprite->addKeyframe(PARRI_LEFT, glm::vec2(1 - (1 * 0.0625), 0.0625f));
	sprite->addKeyframe(PARRI_LEFT, glm::vec2(1 - (1 * 0.0625), 0.0625f));
	sprite->addKeyframe(PARRI_LEFT, glm::vec2(1 - (1 * 0.0625), 0.0625f));
	sprite->addKeyframe(PARRI_LEFT, glm::vec2(1 - (1 * 0.0625), 0.0625f));

	sprite->setAnimationSpeed(PARRI_RIGHT, 16);
	sprite->addKeyframe(PARRI_RIGHT, glm::vec2(0 * 0.0625, 0.0625f));
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

	sprite->setAnimationSpeed(ATTACK_LEFT_ULTI, 4);
	for (int i = 1; i <= 3; i++){
		sprite->addKeyframe(ATTACK_LEFT_ULTI, glm::vec2(1 - (i*0.0625), 2*0.0625f));
	}
	for (int i = 1; i <= 3; i++){
		sprite->addKeyframe(ATTACK_LEFT_ULTI, glm::vec2(1 - (i*0.0625), 2 * 0.0625f));
	}

	sprite->setAnimationSpeed(ATTACK_RIGHT_ULTI, 4);
	for (int i = 0; i < 3; i++){
		sprite->addKeyframe(ATTACK_RIGHT_ULTI, glm::vec2(i*0.0625, 2*0.0625f));
	}
	for (int i = 0; i < 3; i++){
		sprite->addKeyframe(ATTACK_RIGHT_ULTI, glm::vec2(i*0.0625, 2 * 0.0625f));
	}



	sprite->setAnimationSpeed(FALL_LEFT, 8);
	for (int i = 2; i <= 2; i++){
		sprite->addKeyframe(FALL_LEFT, glm::vec2(1 - (i*0.0625), 3 * 0.0625f));
	}

	sprite->setAnimationSpeed(FALL_RIGHT, 8);
	for (int i = 1; i < 2; i++){
		sprite->addKeyframe(FALL_RIGHT, glm::vec2(i*0.0625, 3 * 0.0625f));
	}

	sprite->setAnimationSpeed(DIE_LEFT, 8);
	for (int i = 3; i <= 7; i++){
		sprite->addKeyframe(DIE_LEFT, glm::vec2(1 - (i*0.0625), 3 * 0.0625f));
	}

	sprite->setAnimationSpeed(DIE_RIGHT, 8);
	for (int i = 2; i < 7; i++){
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(i*0.0625, 3 * 0.0625f));
	}

	sprite->setAnimationSpeed(REST_DIE_LEFT, 8);
	sprite->addKeyframe(REST_DIE_LEFT, glm::vec2(1 - (7 * 0.0625), 3 * 0.0625f));


	sprite->setAnimationSpeed(REST_DIE_RIGHT, 8);
	sprite->addKeyframe(REST_DIE_RIGHT, glm::vec2(6 * 0.0625, 3 * 0.0625f));

	if (leftright)sprite->changeAnimation(0);
	else sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

void Boss::update(int deltaTime, glm::vec2 posPlayer, int anim, int key, int vidaPlayer){
	sprite->update(deltaTime);
	if (lastkey == key) ++contkey;
	else {
		lastkey = key;
		contkey = 0;
	}
	if (vida > 0 && vidaPlayer > 0){
		if (leftright){
			if (posPlayer.x >= posBoss.x && sqrt(pow(posPlayer.x - posBoss.x, 2) + pow(posPlayer.y - posBoss.y, 2)) < 3 * 32){
				if (sprite->animation() == STAND_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == IDLE_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == ATTACK_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == ATTACK_LEFT_ULTI)
					sprite->changeAnimation(IDLE_RIGHT);
				leftright = false;
			}
			else if (sqrt(pow(posPlayer.x - posBoss.x, 2) + pow(posPlayer.y - posBoss.y, 2)) < 3 * 32){
				if (cdulti == 0){
					if (sprite->animation() != ATTACK_LEFT_ULTI){
						sprite->changeAnimation(ATTACK_LEFT_ULTI);
					}
					cdulti = 450;
				}
				else if (cd == 0){
					if (sprite->animation() != ATTACK_LEFT_ULTI && sprite->animation() != ATTACK_LEFT)sprite->changeAnimation(ATTACK_LEFT);
					cd = 100;
				}
				else if (anim == 55 && key == 4 && contkey == 0){
					int x = rand() % 2;
					if (x == 0){
						if (sprite->animation() != ATTACK_LEFT_ULTI)
							sprite->changeAnimation(PARRI_LEFT);
					}
					else {
						vida -= 1;
						if (vida <= 0) sprite->changeAnimation(DIE_LEFT);
						posBoss.x += 8;
					}
				}
				else {
					if (sprite->animation() == ATTACK_LEFT){
						if (sprite->timetoChange(ATTACK_LEFT))
							sprite->changeAnimation(IDLE_LEFT);
					}
					if (sprite->animation() == ATTACK_LEFT_ULTI){
						if (sprite->timetoChange(ATTACK_LEFT_ULTI))
							sprite->changeAnimation(IDLE_LEFT);
					}
					if (sprite->animation() == PARRI_LEFT){
						if (sprite->timetoChange(PARRI_LEFT))
							sprite->changeAnimation(IDLE_LEFT);
					}
					cd--;
					cdulti--;
				}
			}
			else if (posPlayer.x >= posBoss.x && sqrt(pow(posPlayer.x - posBoss.x, 2) + pow(posPlayer.y - posBoss.y, 2)) < 5 * 32){
				if (sprite->animation() == STAND_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == IDLE_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == ATTACK_LEFT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == ATTACK_LEFT_ULTI)
					sprite->changeAnimation(IDLE_RIGHT);
				leftright = false;
			}
			else if (sqrt(pow(posPlayer.x - posBoss.x, 2) + pow(posPlayer.y - posBoss.y, 2)) < 5 * 32){
				if (sprite->animation() == STAND_LEFT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == ATTACK_LEFT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == ATTACK_LEFT_ULTI)
					sprite->changeAnimation(IDLE_LEFT);
			}
			if (!map->collisionMoveDown(posBoss, glm::ivec2(64, 64), &posBoss.y)){
				if (sprite->animation() != FALL_RIGHT)
					sprite->changeAnimation(FALL_RIGHT);
				posBoss.y += 4;
			}
			else {
				if (sprite->animation() == FALL_RIGHT){
					vida = 0;
					sprite->changeAnimation(DIE_RIGHT);
				}
			}
		}
		else {
			if (posPlayer.x <= posBoss.x && sqrt(pow(posPlayer.x - posBoss.x, 2) + pow(posPlayer.y - posBoss.y, 2)) < 5 * 32){
				if (sprite->animation() == STAND_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == IDLE_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == ATTACK_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == ATTACK_RIGHT_ULTI)
					sprite->changeAnimation(IDLE_LEFT);
				leftright = true;
			}
			else if (sqrt(pow(posPlayer.x - posBoss.x, 2) + pow(posPlayer.y - posBoss.y, 2)) < 3 * 32){
				if (cdulti == 0){
					if (sprite->animation() != ATTACK_RIGHT_ULTI){
						sprite->changeAnimation(ATTACK_RIGHT_ULTI);
					}
					cdulti = 450;
				}
				else if (cd == 0){
					if (sprite->animation() != ATTACK_RIGHT_ULTI && sprite->animation() != ATTACK_RIGHT)sprite->changeAnimation(ATTACK_RIGHT);
					cd = 100;
				}
				else if (anim == 54 && key == 4 && contkey == 0){
					int x = rand() % 2;
					if (x == 0){
						if (sprite->animation() != ATTACK_RIGHT_ULTI)
							sprite->changeAnimation(PARRI_RIGHT);
					}
					else {
						vida -= 1;
						if (vida <= 0) sprite->changeAnimation(DIE_RIGHT);
						posBoss.x -= 8;
					}
				}
				else {
					if (sprite->animation() == ATTACK_RIGHT){
						if (sprite->timetoChange(ATTACK_RIGHT))
							sprite->changeAnimation(IDLE_RIGHT);
					}
					if (sprite->animation() == ATTACK_RIGHT_ULTI){
						if (sprite->timetoChange(ATTACK_RIGHT_ULTI))
							sprite->changeAnimation(IDLE_RIGHT);
					}
					if (sprite->animation() == PARRI_RIGHT){
						if (sprite->timetoChange(PARRI_RIGHT))
							sprite->changeAnimation(IDLE_RIGHT);
					}
					cd--;
					cdulti--;
				}
			}
			else if (posPlayer.x <= posBoss.x && sqrt(pow(posPlayer.x - posBoss.x, 2) + pow(posPlayer.y - posBoss.y, 2)) < 5 * 32){
				if (sprite->animation() == STAND_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == IDLE_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == ATTACK_RIGHT)
					sprite->changeAnimation(IDLE_LEFT);
				if (sprite->animation() == ATTACK_RIGHT_ULTI)
					sprite->changeAnimation(IDLE_LEFT);
				leftright = true;
			}
			else if (sqrt(pow(posPlayer.x - posBoss.x, 2) + pow(posPlayer.y - posBoss.y, 2)) < 5 * 32){
				if (sprite->animation() == STAND_RIGHT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == ATTACK_RIGHT)
					sprite->changeAnimation(IDLE_RIGHT);
				if (sprite->animation() == ATTACK_RIGHT_ULTI)
					sprite->changeAnimation(IDLE_RIGHT);
			}
			if (!map->collisionMoveDown(posBoss, glm::ivec2(64, 64), &posBoss.y)){
				if (sprite->animation() != FALL_LEFT)
					sprite->changeAnimation(FALL_LEFT);
				posBoss.y += 4;
			}
			else {
				if (sprite->animation() == FALL_LEFT){
					vida = 0;
					sprite->changeAnimation(DIE_LEFT);
				}
			}
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
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

void Boss::render(){
	sprite->render();
}

void Boss::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}
void Boss::setPosition(const glm::vec2 &pos)
{
	posBoss = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

glm::vec2 Boss::getPosition()
{
	glm::vec2 p = (glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
	return p;
}

glm::ivec2 Boss::getPositionTile()
{
	glm::ivec2 p = glm::ivec2((posBoss.x + 16) / 32, (posBoss.y + 32) / 63);
	return p;
}

int Boss::getVida(){
	return vida;
}

int Boss::getCurrentAnim(){
	return sprite->animation();
}

int Boss::getCurrentKeyframe(int animId){
	return sprite->getKeyframe(animId);
}

void Boss::setLeftRight(bool b){
	leftright = b;
}

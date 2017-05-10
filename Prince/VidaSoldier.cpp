#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "VidaSoldier.h"
#include "Game.h"

enum VSAnims
{
	FULL_VIDA,THREE, TWO, ONE, DEAD
};


void VidaSoldier::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("sprites/sprites-vida-enemy.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 8), glm::vec2(0.625f, 0.015625f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(FULL_VIDA, 8);
	sprite->addKeyframe(FULL_VIDA, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(THREE, 8);
	sprite->addKeyframe(THREE, glm::vec2(0.0f, 4 * 0.015625f));

	sprite->setAnimationSpeed(TWO, 8);
	sprite->addKeyframe(TWO, glm::vec2(0.0f, 8 * 0.015625f));

	sprite->setAnimationSpeed(ONE, 8);
	sprite->addKeyframe(ONE, glm::vec2(0.0f, 12 * 0.015625f));

	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(DEAD, glm::vec2(0.0f, 16 * 0.015625f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVS.x), float(tileMapDispl.y + posVS.y)));
}

void VidaSoldier::update(int deltaTime, int vida){
	sprite->update(deltaTime);
	sprite->changeAnimation(4 - vida);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVS.x), float(tileMapDispl.y + posVS.y)));
}

void VidaSoldier::render(){
	sprite->render();
}

void VidaSoldier::setPosition(const glm::vec2 &pos)
{
	posVS = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVS.x), float(tileMapDispl.y + posVS.y)));
}


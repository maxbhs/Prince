#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "VidaBoss.h"
#include "Game.h"

enum VBAnims
{
	FULL_VIDA, FOUR, THREE, TWO, ONE, DEAD
};


void VidaBoss::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("sprites/sprites-vida-boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 8), glm::vec2(0.625f, 0.015625f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(FULL_VIDA, 8);
	sprite->addKeyframe(FULL_VIDA, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(FOUR, 8);
	sprite->addKeyframe(FOUR, glm::vec2(0.0f, 4 * 0.015625f));

	sprite->setAnimationSpeed(THREE, 8);
	sprite->addKeyframe(THREE, glm::vec2(0.0f, 8 * 0.015625f));

	sprite->setAnimationSpeed(TWO, 8);
	sprite->addKeyframe(TWO, glm::vec2(0.0f, 12 * 0.015625f));

	sprite->setAnimationSpeed(ONE, 8);
	sprite->addKeyframe(ONE, glm::vec2(0.0f, 16 * 0.015625f));

	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(DEAD, glm::vec2(0.0f, 20 * 0.015625f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVB.x), float(tileMapDispl.y + posVB.y)));
}

void VidaBoss::update(int deltaTime, int vida){
	sprite->update(deltaTime);
	sprite->changeAnimation(5 - vida);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVB.x), float(tileMapDispl.y + posVB.y)));
}

void VidaBoss::render(){
	sprite->render();
}

void VidaBoss::setPosition(const glm::vec2 &pos){
	posVB = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVB.x), float(tileMapDispl.y + posVB.y)));
}



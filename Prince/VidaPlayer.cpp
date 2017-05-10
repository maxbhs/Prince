#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "VidaPlayer.h"
#include "Game.h"

enum VPAnims
{
	FULL_VIDA, FIVE, FOUR, THREE, TWO, ONE, DEAD
};


void VidaPlayer::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("sprites/sprites-vida.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 8), glm::vec2(0.625f, 0.015625f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(7);

	sprite->setAnimationSpeed(FULL_VIDA, 8);
	sprite->addKeyframe(FULL_VIDA, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(FIVE, 8);
	sprite->addKeyframe(FIVE, glm::vec2(0.0f, 4*0.015625f));

	sprite->setAnimationSpeed(FOUR, 8);
	sprite->addKeyframe(FOUR, glm::vec2(0.0f, 8 * 0.015625f));

	sprite->setAnimationSpeed(THREE, 8);
	sprite->addKeyframe(THREE, glm::vec2(0.0f, 12 * 0.015625f));

	sprite->setAnimationSpeed(TWO, 8);
	sprite->addKeyframe(TWO, glm::vec2(0.0f, 16 * 0.015625f));

	sprite->setAnimationSpeed(ONE, 8);
	sprite->addKeyframe(ONE, glm::vec2(0.0f, 20 * 0.015625f));

	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(DEAD, glm::vec2(0.0f, 24 * 0.015625f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVP.x), float(tileMapDispl.y + posVP.y)));
}

void VidaPlayer::update(int deltaTime, int vida){
	sprite->update(deltaTime);
	sprite->changeAnimation(6-vida);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVP.x), float(tileMapDispl.y + posVP.y)));
}

void VidaPlayer::render(){
	sprite->render();
}

void VidaPlayer::setPosition(const glm::vec2 &pos)
{
	posVP = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVP.x), float(tileMapDispl.y + posVP.y)));
}

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "levelcompleted.h"
#include "Game.h"

enum LevelCompletedAnims
{
	LEVEL_COMPLETED,
};


void levelcompleted::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/level-completed.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 203), glm::vec2(1.0, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(LEVEL_COMPLETED, 8);
	sprite->addKeyframe(LEVEL_COMPLETED, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLC.x), float(tileMapDispl.y + posLC.y)));
}

void levelcompleted::update(int deltaTime, int lvl){

	sprite->update(deltaTime);
	if (Game::instance().getKey(13)){
		Game::instance().init(lvl, true);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLC.x), float(tileMapDispl.y + posLC.y)));
}

void levelcompleted::render(){
	sprite->render();
}

void levelcompleted::setPosition(const glm::vec2 &pos)
{
	posLC = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLC.x), float(tileMapDispl.y + posLC.y)));
}

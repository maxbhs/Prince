#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "instruccions.h"
#include "Game.h"

enum InstrucAnims
{
	INSTRUCCIONS,
};


void instruccions::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/instruccions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 203), glm::vec2(1.0, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(INSTRUCCIONS, 8);
	sprite->addKeyframe(INSTRUCCIONS, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posInst.x), float(tileMapDispl.y + posInst.y)));
}

void instruccions::update(int deltaTime){
	sprite->update(deltaTime);
	if (Game::instance().getKey(13)){
		Game::instance().init(false, true);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posInst.x), float(tileMapDispl.y + posInst.y)));
}

void instruccions::render(){
	sprite->render();
}

void instruccions::setPosition(const glm::vec2 &pos)
{
	posInst = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posInst.x), float(tileMapDispl.y + posInst.y)));
}



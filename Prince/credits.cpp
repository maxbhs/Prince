#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "credits.h"
#include "Game.h"

enum CreditsAnims
{
	CREDITS,
};


void credits::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 203), glm::vec2(1.0, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(CREDITS, 8);
	sprite->addKeyframe(CREDITS, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posInst.x), float(tileMapDispl.y + posInst.y)));
}

void credits::update(int deltaTime){
	sprite->update(deltaTime);
	if (Game::instance().getKey(13)){
		Game::instance().init(false,true);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posInst.x), float(tileMapDispl.y + posInst.y)));
}

void credits::render(){
	sprite->render();
}

void credits::setPosition(const glm::vec2 &pos)
{
	posInst = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posInst.x), float(tileMapDispl.y + posInst.y)));
}
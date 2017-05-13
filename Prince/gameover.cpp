#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "gameover.h"
#include "Game.h"

enum GAMEOVERAnims
{
	GAME_OVER,
};


void gameover::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/game-over.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 203), glm::vec2(1.0, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(GAME_OVER, 8);
	sprite->addKeyframe(GAME_OVER, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGO.x), float(tileMapDispl.y + posGO.y)));
}

void gameover::update(int deltaTime, int lvl){
	
	sprite->update(deltaTime);
	if (Game::instance().getKey(13)){
		Game::instance().init(lvl,true);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGO.x), float(tileMapDispl.y + posGO.y)));
}

void gameover::render(){
	sprite->render();
}

void gameover::setPosition(const glm::vec2 &pos)
{
	posGO = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGO.x), float(tileMapDispl.y + posGO.y)));
}


#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Trap.h"
#include "Game.h"

enum TrapAnims
{
	TRAP, OPEN
};


void Trap::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("sprites/sprites-cuchilla.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25, .25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(TRAP, 8);
	sprite->addKeyframe(TRAP, glm::vec2(0.0f, 0.f));
	sprite->addKeyframe(TRAP, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(TRAP, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(TRAP, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(TRAP, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(TRAP, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(TRAP, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(TRAP, glm::vec2(0.75f, 0.25f));
	sprite->addKeyframe(TRAP, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(TRAP, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(OPEN, 8);
	sprite->addKeyframe(OPEN, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(OPEN, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(OPEN, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(OPEN, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(OPEN, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(OPEN, glm::vec2(0.25f, 0.5f));






	sprite->changeAnimation(TRAP);
	int x = rand() % 3;
	if (x == 0)
		sprite->setKeyframe(TRAP, 0);
	else if (x == 1)
		sprite->setKeyframe(TRAP, 4);
	else 
		sprite->setKeyframe(TRAP, 9);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTrap.x), float(tileMapDispl.y + posTrap.y)));
}

void Trap::update(int deltaTime){
	sprite->update(deltaTime);
	if (sprite->animation() == TRAP){
		if (sprite->timetoChange(TRAP)){
			sprite->changeAnimation(OPEN);
		}
	}
	if (sprite->animation() == OPEN){
		if (sprite->timetoChange(OPEN)){
			sprite->changeAnimation(TRAP);
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTrap.x), float(tileMapDispl.y + posTrap.y)));
}

void Trap::render(){
	sprite->render();
}

void Trap::setPosition(const glm::vec2 &pos)
{
	posTrap = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTrap.x), float(tileMapDispl.y + posTrap.y)));
}

glm::ivec2 Trap::getPositionTile()
{
	glm::ivec2 p = glm::ivec2((posTrap.x) / 32, (posTrap.y + 32) / 63);
	return p;
}

glm::vec2 Trap::getPosition()
{
	glm::vec2 p = (glm::vec2(float(tileMapDispl.x + posTrap.x + 31), float(tileMapDispl.y + posTrap.y)));
	return p;
}

int Trap::getCurrentAnim(){
	return sprite->animation();
}

int Trap::getCurrentKeyframe(int animId){
	return sprite->getKeyframe(animId);
}


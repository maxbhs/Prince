#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Door.h"
#include "Game.h"

enum DoorAnims
{
	CLOSE, DOOR, OPEN
}; 


void Door::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("sprites/sprites-door.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25, .25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(CLOSE, 8);
	sprite->addKeyframe(CLOSE, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(DOOR, 8);
	sprite->addKeyframe(DOOR, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(DOOR, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(DOOR, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(DOOR, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(DOOR, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(DOOR, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(DOOR, glm::vec2(0.75f, 0.25f));

	sprite->setAnimationSpeed(OPEN, 8);
	sprite->addKeyframe(OPEN, glm::vec2(0.0f, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));
}

void Door::update(int deltaTime, bool open){
	sprite->update(deltaTime);
	if (sprite->animation() == CLOSE && open){
		sprite->changeAnimation(DOOR);
	}
	else if (sprite->animation() == DOOR){
		if (sprite->timetoChange(DOOR))
			sprite->changeAnimation(OPEN);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));
}

void Door::render(){
	sprite->render();
}

void Door::setPosition(const glm::vec2 &pos)
{
	posDoor = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDoor.x), float(tileMapDispl.y + posDoor.y)));
}

glm::ivec2 Door::getPositionTile()
{
	glm::ivec2 p = glm::ivec2((posDoor.x) / 32, (posDoor.y + 32) / 63);
	return p;
}


#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Land.h"
#include "Game.h"

enum LandAnims
{
	LAND, NONE
};


void Land::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	cont = -1;
	stepOn = false;
	firstColl = true;
	spritesheet.loadFromFile("sprites/sprites-piedra.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25, .25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(LAND, 2);
	sprite->addKeyframe(LAND, glm::vec2(0.f, 0.f));
	sprite->setAnimationSpeed(NONE, 2);
	sprite->addKeyframe(NONE, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLand.x), float(tileMapDispl.y + posLand.y)));
}

void Land::update(int deltaTime, glm::ivec2 pt, glm::ivec2 ps, bool bJ){
	sprite->update(deltaTime);
	if (((pt.x == int(posLand.x / 32) && pt.y == int(posLand.y / 63)) || (ps.x == int(posLand.x / 32) && ps.y == int(posLand.y / 63))) && !stepOn && !bJ){
		stepOn = true;
		cont = 35;
	}
	if (stepOn && cont > 0) cont -= 1;
	if (cont == 0) {
		posLand.y += 4;
		if (map->collisionMoveDown(posLand, glm::ivec2(64,64), &posLand.y)){
			posLand.y -= 4;
			if (firstColl) firstColl = false;
			else sprite->changeAnimation(NONE);
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLand.x), float(tileMapDispl.y + posLand.y)));
}

void Land::render(){
	sprite->render();
}

void Land::setPosition(const glm::vec2 &pos)
{
	posLand = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLand.x), float(tileMapDispl.y + posLand.y)));
}

glm::vec2 Land::getPosition()
{
	glm::vec2 p = (glm::vec2(float(tileMapDispl.x + posLand.x), float(tileMapDispl.y + posLand.y)));
	return p;
}

void Land::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::ivec2 Land::getPositionTile()
{
	glm::ivec2 p = glm::ivec2((posLand.x + 16) / 32, (posLand.y + 32) / 63);
	return p;
}

bool Land::getStepOn(){
	return stepOn;
}

int Land::getCurrentAnim(){
	return sprite->animation();
}

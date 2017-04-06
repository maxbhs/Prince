#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Land.h"
#include "Game.h"

enum LandAnims
{
	LAND
};


void Land::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	cont = -1;
	stepOn = false;
	spritesheet.loadFromFile("sprites/sprites-piedra.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25, .25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(LAND, 2);
	sprite->addKeyframe(LAND, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLand.x), float(tileMapDispl.y + posLand.y)));
}

void Land::update(int deltaTime, glm::ivec2 pt){
	sprite->update(deltaTime);
	if (pt.x == int(posLand.x / 32) && pt.y == int(posLand.y / 63) && !stepOn){
		stepOn = true;
		cont = 25;
	}
	if (stepOn && cont > 0) cont -= 1;
	if (cont == 0) posLand.y += 4;
	if (map->collisionMoveDown(posLand, glm::ivec2(64, 64), &posLand.y)){
		posLand.y -= 4;
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

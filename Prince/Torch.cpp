#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Torch.h"
#include "Game.h"

enum TorchAnims
{
	TORCH,
};


void Torch::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("sprites/sprites-torch.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.25, .25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(TORCH, 8);
	sprite->addKeyframe(TORCH, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(TORCH, glm::vec2(0.25f, 0.0f));
	sprite->addKeyframe(TORCH, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(TORCH, glm::vec2(0.75f, 0.0f));
	sprite->addKeyframe(TORCH, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(TORCH, glm::vec2(0.25f, 0.25f));

	sprite->changeAnimation(0);
	int x = rand() % 6;
	sprite->setKeyframe(0,x);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTor.x), float(tileMapDispl.y + posTor.y)));
}

void Torch::update(int deltaTime){
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTor.x), float(tileMapDispl.y + posTor.y)));
}

void Torch::render(){
	sprite->render();
}

void Torch::setPosition(const glm::vec2 &pos)
{
	posTor = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTor.x), float(tileMapDispl.y + posTor.y)));
}


#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "menu.h"
#include "Game.h"

enum MenuAnims
{
	MENU,
};


void menu::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	ins = false;
	cred = false;

	spritesheet.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(320, 203), glm::vec2(1.0, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(MENU, 8);
	sprite->addKeyframe(MENU, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	texProgram = shaderProgram;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMenu.x), float(tileMapDispl.y + posMenu.y)));
}

void menu::update(int deltaTime){
	sprite->update(deltaTime);
	if (Game::instance().getKey(int('q')) && !ins && !cred){
		Game::instance().init(1, false);
		ins = false;
		cred = false;
	}
	else if (Game::instance().getKey(int('w')) && !ins && !cred){
		Game::instance().init(2, false);
		ins = false;
		cred = false;
	}
	else if (Game::instance().getKey(int('e')) && !ins && !cred){
		Game::instance().init(3, false);
		ins = false;
		cred = false;
	}
	else if (Game::instance().getKey(int('r')) && !cred){
		instruc = new instruccions();
		instruc->init(glm::ivec2(0, 0), texProgram);
		instruc->setPosition(glm::vec2(0, 0));
		ins = true;
		cred = false;

	}
	else if (Game::instance().getKey(int('t')) && !ins){
		credit = new credits();
		credit->init(glm::ivec2(0, 0), texProgram);
		credit->setPosition(glm::vec2(0, 0));
		ins = false;
		cred = true;
	}

	if (ins) instruc->update(deltaTime);
	if (cred) credit->update(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMenu.x), float(tileMapDispl.y + posMenu.y)));
}

void menu::render(){
	sprite->render();
	if (ins) instruc->render();
	if (cred) credit->render();
}

void menu::setPosition(const glm::vec2 &pos)
{
	posMenu = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMenu.x), float(tileMapDispl.y + posMenu.y)));
}
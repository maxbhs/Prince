#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init(bool lvl, bool menu)
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	sc = false;
	if (menu){
		menuprincipal.init();
	}
	else {
		scene.init(lvl);
		sc = true;
	}
}

bool Game::update(int deltaTime)
{
	if (!sc){
		menuprincipal.update(deltaTime);
	}
	else {
		scene.update(deltaTime);
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!sc){
		menuprincipal.render();
	}
	else {
		scene.render();
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}







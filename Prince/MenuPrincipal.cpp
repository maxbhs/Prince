#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "MenuPrincipal.h"
#include "Game.h"


#define SCREEN_X -30
#define SCREEN_Y -60

MenuPrincipal::MenuPrincipal(){
	menuprin = NULL;
}

MenuPrincipal::~MenuPrincipal()
{
	if (menuprin != NULL)
		delete menuprin;
}

void MenuPrincipal::init(){

	initShaders();
	menuprin = new menu();
	menuprin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	menuprin->setPosition(glm::vec2(30, 60));
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);


}

void MenuPrincipal::update(int deltaTime){
	currentTime += deltaTime;
	menuprin->update(deltaTime);
}

void MenuPrincipal::render(){
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	menuprin->render();
}

void MenuPrincipal::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();

	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
}
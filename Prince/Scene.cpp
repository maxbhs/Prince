#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X -30
#define SCREEN_Y -60

#define INIT_PLAYER_X_TILES 9
#define INIT_PLAYER_Y_TILES 2


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	posM.x = 0;
	posM.y = 0;
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, posM);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize().x, (INIT_PLAYER_Y_TILES * map->getTileSize().y)+2));
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	
}

void Scene::init2(int mov)  //mov=1 dreta, mov=2 esquerra, mov=3 abaix, mov=4 adalt
{
	glm::vec2 p = player->getPositionTile();
	int x = p.x;
	int y = p.y;

	if (mov == 1){
		posM.x++;
		x = 0;
	}
	else if (mov == 2){
		posM.x--;
		x = 10;
	}
	else if (mov == 3){
		posM.y++;
		y = 1;
		x += 3;
	}
	else if (mov == 4){
		posM.y--;
		y = 3;
		x += 7;
	}
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, posM);
	player->setPosition(glm::vec2(x * map->getTileSize().x, (y * map->getTileSize().y) + 2)); //falta editar
	player->setTileMap(map);
}

void Scene::update(int deltaTime)
{
	
	currentTime += deltaTime;
	player->update(deltaTime);
	glm::ivec2 p = player->getPosition();
	if (p.x > 294) init2(1);           //esto esta hecho a ojo -.-
	else if (p.x < -30) init2(2);
	else if (p.y > 160) init2(3);
	else if (p.y < -52) init2(4);
	
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render_back(texProgram);
	player->render();
	map->render_front(texProgram);
	

	
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();

	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
}




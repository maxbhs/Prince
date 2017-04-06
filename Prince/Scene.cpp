#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X -30
#define SCREEN_Y -60

#define INIT_PLAYER_X_TILES 1
#define INIT_PLAYER_Y_TILES 1


Scene::Scene()
{
	map = NULL;
	player = NULL;
	inter = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (inter != NULL)
		delete inter;
}


void Scene::init()
{
	posM.x = 0;
	posM.y = 0;
	ini_teleport();
	puls = false;
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, posM);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize().x, (INIT_PLAYER_Y_TILES * map->getTileSize().y)+2));
	player->setTileMap(map);
	vidaPlayer = player->getVida();
	inter = new Interface();
	inter->init(glm::ivec2(SCREEN_X,SCREEN_Y), texProgram);
	inter->setPosition(glm::vec2(30,252));
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	torchs = false;
	doors = false;
	traps = false;
	lands = false;
	drawEnemy();
	drawTorchs();
	drawDoors();
	drawTraps();
	drawLands();
}

void Scene::init2(int mov, int tp)  //mov=1 dreta, mov=2 esquerra, mov=3 abaix, mov=4 adalt
{
	glm::vec2 p = player->getPosition();
	float x = p.x;
	float y = p.y;
	//x += 32;
	if (mov == 1){
		posM.x++;
		x = 5;
		y +=60;
	}
	else if (mov == 2){
		posM.x--;
		x = 315;
		y += 60;
	}
	else if (mov == 3){
		posM.y++;
		y = 65;
		x += 30;
		player->ha_caido();
	}
	else if (mov == 4){
		posM.y--;
		y = 195;
		x += 30;
	}
	else if (mov == 0) {
		x = (float) rptp[tp].x * map->getTileSize().x;
		y = (float) rptp[tp].y * map->getTileSize().y+2;
	}
	//map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, posM);
	map->prepareMap(texProgram, posM);
	player->setPosition(glm::vec2(x, y)); //falta editar
	player->setTileMap(map);
	torchsvec.clear();
	doorsvec.clear();
	trapsvec.clear();
	landsvec.clear();
	drawTorchs();
	drawDoors();
	drawTraps();
	drawLands();
}

void Scene::update(int deltaTime)
{

	currentTime += deltaTime;
	player->update(deltaTime, posM);
	glm::vec2 p = player->getPosition();
	glm::ivec2 pt = player->getPositionTile();
	int a = player->getCurrentAnim();
	int k = player->getCurrentKeyframe(a);
	int v = player->getVida();
	if (soldiers[posM.y * map->getnXnY().x + posM.x]){
		soldiersvec[posM.y * map->getnXnY().x + posM.x].update(deltaTime, p, a, k,v);
		glm::vec2 pp = soldiersvec[posM.y * map->getnXnY().x + posM.x].getPosition();
		int aa = soldiersvec[posM.y * map->getnXnY().x + posM.x].getCurrentAnim();
		int kk = soldiersvec[posM.y * map->getnXnY().x + posM.x].getCurrentKeyframe(aa);
		player->update(deltaTime, pp, aa, kk);
	}
	if (torchs){
		for (int i = 0; i < torchsvec.size(); i++)
			torchsvec[i].update(deltaTime);
	}
	if (map->getTile(pt) == 13) {
		map->prepareArrayTile(texProgram, pt, 36);
		puls = true;
	}
	else puls = false;


	if (map->getTile(pt) == 12 && !player->saltando()) {
		map->preparePotion(texProgram, pt, posM);
		player->update(deltaTime, 1);
	}
	if (map->getTile(pt) == 37 && !player->saltando()) {
		teleport(pt);
	}

	if (doors){

		if (map->getTile(pt) == 13){
			for (int i = 0; i < doorsvec.size(); i++){
				doorsvec[i].update(deltaTime, true);
				glm::ivec2 posT = doorsvec[i].getPositionTile();
				map->setTile(posT, 2); //cambiado
			}
		}
		else {
			for (int i = 0; i < doorsvec.size(); i++)
				doorsvec[i].update(deltaTime, false);
		}
	}
	if (traps){
		for (int i = 0; i < trapsvec.size(); i++){
			trapsvec[i].update(deltaTime);
			glm::vec2 pp = trapsvec[i].getPosition();
			int aa = trapsvec[i].getCurrentAnim();
			player->update(deltaTime, pp, aa);
		}
	}
	if (lands){
		for (int i = 0; i < landsvec.size(); i++){
			landsvec[i].update(deltaTime, pt);
			glm::ivec2 pl = landsvec[i].getPositionTile();
			if (landsvec[i].getStepOn() && cont[i] == -1) cont[i] = 25;
			if (cont[i] > 0) cont[i] -= 1;
			if (cont[i] == 0) {
				map->setTile(pl, 27);
			}
		}
	}
	inter->update(deltaTime, player->getVida());
	
	if (p.x > 294) init2(1, 0);           //esto esta hecho a ojo -.-
	else if (p.x < -30) init2(2, 0);
	else if (p.y > 160) init2(3, 0);
	else if (p.y < -52) init2(4, 0);
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
	if (puls) map->render_puls(texProgram);
	inter->render();
	if (torchs){
		for (int i = 0; i < torchsvec.size(); i++)
			torchsvec[i].render();
	}
	if (doors){
		for (int i = 0; i < doorsvec.size(); i++)
			doorsvec[i].render();
	}
	if (traps){
		for (int i = 0; i < trapsvec.size(); i++)
			trapsvec[i].render();
	}
	if (lands){
		for (int i = 0; i < landsvec.size(); i++)
			landsvec[i].render();
	}
	
	if (soldiers[posM.y * map->getnXnY().x + posM.x]) soldiersvec[posM.y * map->getnXnY().x + posM.x].render();
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

void Scene::ini_teleport() {
	aptp = new glm::ivec2[5];
	aptp[0].x = 7; aptp[0].y = 3;
	aptp[1].x = 5; aptp[1].y = 1;
	aptp[2].x = 5; aptp[2].y = 1;
	aptp[3].x = 3; aptp[3].y = 1;
	aptp[4].x = 6; aptp[4].y = 3;

	
	amtp = new glm::ivec2[5];
	amtp[0].x = 1; amtp[0].y = 0;
	amtp[1].x = 0; amtp[1].y = 1;
	amtp[2].x = 3; amtp[2].y = 1;
	amtp[3].x = 3; amtp[3].y = 0;
	amtp[4].x = 1; amtp[4].y = 1;


	rptp = new glm::ivec2[5];
	rptp[0].x = 1; rptp[0].y = 1;
	rptp[1].x = 3; rptp[1].y = 3;
	rptp[2].x = 5; rptp[2].y = 1;
	rptp[3].x = 7; rptp[3].y = 1;
	rptp[4].x = 7; rptp[4].y = 1;


	rmtp = new glm::ivec2[5];
	rmtp[0].x = 0; rmtp[0].y = 1;
	rmtp[1].x = 1; rmtp[1].y = 0;
	rmtp[2].x = 3; rmtp[2].y = 0;
	rmtp[3].x = 3; rmtp[3].y = 1;
	rmtp[4].x = 0; rmtp[4].y = 0;



}

void Scene::teleport(glm::ivec2 posT) {
	int tp = 0;
	for (int i = 0; i < 5; i++) {
		if (aptp[i].x == posT.x && aptp[i].y == posT.y && amtp[i].x == posM.x && amtp[i].y == posM.y) {
			tp = i;
		}
	}
	posM.x = rmtp[tp].x;
	posM.y = rmtp[tp].y;
	init2(0, tp);
}


void Scene::drawEnemy(){ 
	posEnemy = new glm::vec2 [map->getnXnY().x * map->getnXnY().y];
	dirEnemy = new bool[map->getnXnY().x * map->getnXnY().y];
	soldiers = new bool[map->getnXnY().x * map->getnXnY().y];
	for (int i = 0; i < map->getnXnY().x * map->getnXnY().y; i++){
		posEnemy[i].x = -1;
		posEnemy[i].y = -1;
		soldiers[i] = false;
	}
	/*posEnemy[0].x = 4; //A MANO DECIDIR DONDE VA  EL
	posEnemy[0].y = 2;// ENEMY
	dirEnemy[0] = false;

	posEnemy[2].x = 1; //A MANO DECIDIR DONDE VA  EL
	posEnemy[2].y = 1;// ENEMY
	dirEnemy[2] = false;

	posEnemy[10].x = 9; //A MANO DECIDIR DONDE VA  EL
	posEnemy[10].y = 1;// ENEMY 
	dirEnemy[10] = true;*/

	for (int i = 0; i < map->getnXnY().x * map->getnXnY().y; i++){
		if (posEnemy[i].x != -1){
			Soldier *soldier = new Soldier();
			soldier->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram,dirEnemy[i]);
			soldier->setPosition(glm::vec2(posEnemy[i].x*map->getTileSize().x, posEnemy[i].y*map->getTileSize().y + 2));
			soldier->setTileMap(map);
			soldiersvec.push_back(*soldier);
			soldiers[i] = true;
		}
		 else {
			 Soldier *soldier = new Soldier();
			soldiersvec.push_back(*soldier);
			soldiers[i] = false;
		}
	}
}

void Scene::drawTorchs(){
	vector<glm::ivec2> torchspos = map->getTorchsPos();
	for (int i = 0; i < torchspos.size(); i ++){
		Torch *torch = new Torch();
		torch->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		torch->setPosition(glm::vec2(torchspos[i].x * map->getTileSize().x, torchspos[i].y * map->getTileSize().y));
		torchsvec.push_back(*torch);
	}
	if (torchsvec.size() > 0) torchs = true;
	else torchs = false;
}

void Scene::drawDoors(){
	vector<glm::ivec2> doorspos = map->getDoorsPos();
	for (int i = 0; i < doorspos.size(); i++){
		Door *door = new Door();
		door->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		door->setPosition(glm::vec2(doorspos[i].x * map->getTileSize().x, doorspos[i].y * map->getTileSize().y-1));
		doorsvec.push_back(*door);
	}
	if (doorsvec.size() > 0) doors = true;
	else doors = false;
}

void Scene::drawTraps(){
	vector<glm::ivec2> trapspos = map->getTrapsPos();
	for (int i = 0; i < trapspos.size(); i++){
		Trap *trap = new Trap();
		trap->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		trap->setPosition(glm::vec2(trapspos[i].x * map->getTileSize().x, trapspos[i].y * map->getTileSize().y - 1));
		trapsvec.push_back(*trap);
	}
	if (trapsvec.size() > 0) traps = true;
	else traps = false;
}

void Scene::drawLands(){
	vector<glm::ivec2> landspos = map->getLandsPos();
	cont = new int[landspos.size()];
	for (int i = 0; i < landspos.size(); i++){
		Land *land = new Land();
		land->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		land->setPosition(glm::vec2(landspos[i].x * map->getTileSize().x, landspos[i].y * map->getTileSize().y + 46));
		land->setTileMap(map);
		landsvec.push_back(*land);
		cont[i] = -1;
	}
	if (landsvec.size() > 0) lands = true;
	else lands = false;
}


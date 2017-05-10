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
	vidap = NULL;
	vidas = NULL;
	vidab = NULL;
	boss = NULL;
	go = NULL;
	aptp = NULL;
	amtp = NULL;
	rptp = NULL;
	rmtp = NULL;
	posSoldier = NULL;
	dirSoldier = NULL;
	soldiers = NULL;
	lands2 = NULL;
	cont = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (vidap != NULL)
		delete vidap;
	if (vidas != NULL)
		delete vidas;
	if(vidab != NULL)
		delete vidab;
	if (boss != NULL)
		delete boss;
	if (go != NULL)
		delete go;
	if (aptp != NULL)
		delete aptp;
	if (amtp != NULL)
		delete amtp;
	if (rptp != NULL)
		delete rptp;
	if (rmtp != NULL)
		delete rmtp;
	if (posSoldier != NULL)
		delete posSoldier;
	if (dirSoldier != NULL)
		delete dirSoldier;
	if (soldiers != NULL)
		delete soldiers;
	if (lands2 != NULL)
		delete lands2;
	if (cont != NULL)
		delete cont;
	
}


void Scene::init(bool lvl)
{
	posM.x = 0;
	posM.y = 0;
	nivell = lvl; // lvl true nivell 1 // lvl false nivell 2
	ini_teleport();
	puls = false;
	initShaders();
	if (nivell){
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, posM);
	}
	else{
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, posM); //CAMBIAR MAPA
	}
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize().x, (INIT_PLAYER_Y_TILES * map->getTileSize().y) + 2));
	player->setTileMap(map);
	vidap = new VidaPlayer();
	vidap->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	vidap->setPosition(glm::vec2(30, 252));
	vidas = new VidaSoldier();
	vidas->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	vidas->setPosition(glm::vec2(30, 252));
	vidab = new VidaBoss();
	vidab->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	vidab->setPosition(glm::vec2(30, 252));
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	bossdead = false;
	torchs = false;
	doors = false;
	traps = false;
	lands = false;
	
	soldiersvec.clear();
	torchsvec.clear();
	doorsvec.clear();
	trapsvec.clear();
	landsvec.clear();

	drawSoldiers();
	drawBoss();
	drawTorchs();
	drawDoors();
	drawTraps();
	drawLands();
	contGAMEOVER = 100;
	LEVELCOMPLETED = false;
}

void Scene::init2(int mov, int tp)  //mov=1 dreta, mov=2 esquerra, mov=3 abaix, mov=4 adalt
{
	glm::vec2 p = player->getPosition();
	float x = p.x;
	float y = p.y;
	
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
	
	map->prepareMap(texProgram, posM);
	player->setPosition(glm::vec2(x, y)); 
	player->setTileMap(map);
	torchsvec.clear();
	doorsvec.clear();
	trapsvec.clear();
	landsvec.clear();
	drawBoss();
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
	glm::ivec2 ps = glm::ivec2(0, 0);
	int a = player->getCurrentAnim();
	int k = player->getCurrentKeyframe(a);
	int v = player->getVida();
	if (soldiers[posM.y * map->getnXnY().x + posM.x]){
		soldiersvec[posM.y * map->getnXnY().x + posM.x]->update(deltaTime, p, a, k,v);
		glm::vec2 pp = soldiersvec[posM.y * map->getnXnY().x + posM.x]->getPosition();
		ps = soldiersvec[posM.y * map->getnXnY().x + posM.x]->getPositionTile();
		int aa = soldiersvec[posM.y * map->getnXnY().x + posM.x]->getCurrentAnim();
		int kk = soldiersvec[posM.y * map->getnXnY().x + posM.x]->getCurrentKeyframe(aa);
		player->update(deltaTime, pp, aa, kk,false);
	}
	if (bboss){
		boss->update(deltaTime, p, a, k, v);
		glm::vec2 pp = boss->getPosition();
		ps = boss->getPositionTile();
		int aa = boss->getCurrentAnim();
		int kk = boss->getCurrentKeyframe(aa);
		player->update(deltaTime, pp, aa, kk,true);
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
		map->changeTile(texProgram, pt, posM,4);
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
				map->setTile(posT, 2); 
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
			int kk = trapsvec[i].getCurrentKeyframe(aa);
			player->update(deltaTime, pp, aa, kk);
			if (soldiers[posM.y * map->getnXnY().x + posM.x]) soldiersvec[posM.y * map->getnXnY().x + posM.x]->update(deltaTime,pp,aa,kk);
		}
	}
	if (lands){
		for (int i = 0; i < landsvec.size(); i++){
			if (lands2[i]){
				glm::ivec2 pl = landsvec[i].getPositionTile();
				landsvec[i].update(deltaTime, pt, ps, player->saltando());
				if (landsvec[i].getStepOn() && cont[i] == -1) cont[i] = 35;
				if (cont[i] > 0) cont[i] -= 1;
				if (cont[i] == 0){
					map->changeTile(texProgram, pl, posM, 27);
					cont[i] = -2;
				}
				if (landsvec[i].getCurrentAnim() == 1){
					glm::ivec2 pl2 = landsvec[i].getPositionTile();
					map->changeTile(texProgram, pl2, posM, 16);
					lands2[i] = false;
				}
			}
		}
	}
	vidaPlay = player->getVida();
	if (vidaPlay <= 0){
		vidaPlay = 0;
		if (contGAMEOVER >= 0) --contGAMEOVER;
		
	}
	vidap->update(deltaTime, vidaPlay);
	if (contGAMEOVER == 0) {
		go = new gameover();
		go->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		go->setPosition(glm::vec2(30, 60));
	}
	if (contGAMEOVER == -1){
		go->update(deltaTime, nivell);
	}

	if (soldiers[posM.y * map->getnXnY().x + posM.x]){
		vidaSold = soldiersvec[posM.y * map->getnXnY().x + posM.x]->getVida();
		if (vidaSold < 0) vidaSold = 0;
		vidas->update(deltaTime, vidaSold);
	}

	if (bboss){
		vidaBoss = boss->getVida();
		if (vidaBoss <= 0) {
			vidaBoss = 0;
		}
		vidab->update(deltaTime, boss->getVida());
		if (boss->getVida() == 0){
			bossdead = true;
			if (nivell) map->changeTile(texProgram, glm::ivec2(8, 3), posM, 42); //CUANDO MATAS AL BOSS APARECE EL TP VERDE
			else map->changeTile(texProgram, glm::ivec2(8, 3), posM, 42);
		}
	}

	if (nivell){
		if (posM.x == 4 && posM.y == 2){
			if (player->getPositionTile().x == 8 && player->getPositionTile().y == 3){ //CUANDO PISAS EL TP VERDE LEVEL-COMPLETED!!! i MENU PRINCI^PAL AGAIN
				if (bossdead){
					lc = new levelcompleted();
					lc->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					lc->setPosition(glm::vec2(30, 60));
					LEVELCOMPLETED = true;
				}
			}
		}
	}
	else {
		if (posM.x == 3 && posM.y == 3){
			if (player->getPositionTile().x == 8 && player->getPositionTile().y == 3){
				if (bossdead){
					lc = new levelcompleted();
					lc->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					lc->setPosition(glm::vec2(30, 60));
					LEVELCOMPLETED = true;
				}
			}
		}
	}

	if (LEVELCOMPLETED) lc->update(deltaTime, nivell);

	if (p.x > 294) init2(1, 0);           
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
	
	if (torchs){
		for (int i = 0; i < torchsvec.size(); i++)
			torchsvec[i].render();
	}
	if (doors){
		for (int i = 0; i < doorsvec.size(); i++)
			doorsvec[i].render();
	}

	if (lands){
		for (int i = 0; i < landsvec.size(); i++)
			landsvec[i].render();
	}

	if (soldiers[posM.y * map->getnXnY().x + posM.x]) soldiersvec[posM.y * map->getnXnY().x + posM.x]->render();
	if (bboss) boss->render();

	if (traps){
		for (int i = 0; i < trapsvec.size(); i++){
			if (player->getPosition().x + 32 >= trapsvec[i].getPosition().x + 13) 
				trapsvec[i].render();
		}
	}
	player->render();

	if (traps){
		for (int i = 0; i < trapsvec.size(); i++){
			if (player->getPosition().x + 32 < trapsvec[i].getPosition().x + 13) 
				trapsvec[i].render();
		}
	}
	vidap->render();
	if (soldiers[posM.y * map->getnXnY().x + posM.x] && soldiersvec[posM.y * map->getnXnY().x + posM.x]->getVida() > 0) vidas->render();
	if (boss && boss->getVida() > 0) vidab->render();
	
	map->render_front(texProgram);

	if (contGAMEOVER == -1) {
		go->render();
	}

	if (LEVELCOMPLETED) lc->render();
	
	

	
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
	if (nivell){						//DECIDIR TP LVL 1
		aptp = new glm::ivec2[8];
		aptp[0].x = 7; aptp[0].y = 3;
		aptp[1].x = 5; aptp[1].y = 1;
		aptp[2].x = 8; aptp[2].y = 1;
		aptp[3].x = 4; aptp[3].y = 1;
		aptp[4].x = 7; aptp[4].y = 1;
		aptp[5].x = 9; aptp[5].y = 2;
		aptp[6].x = 4; aptp[6].y = 3;
		aptp[7].x = 9; aptp[7].y = 3;


		amtp = new glm::ivec2[8];
		amtp[0].x = 1; amtp[0].y = 0;
		amtp[1].x = 0; amtp[1].y = 1;
		amtp[2].x = 1; amtp[2].y = 2;
		amtp[3].x = 3; amtp[3].y = 3;
		amtp[4].x = 0; amtp[4].y = 5;
		amtp[5].x = 1; amtp[5].y = 1;
		amtp[6].x = 3; amtp[6].y = 5;
		amtp[7].x = 4; amtp[7].y = 1;


		rptp = new glm::ivec2[8];
		rptp[0].x = 1; rptp[0].y = 1;
		rptp[1].x = 3; rptp[1].y = 3;
		rptp[2].x = 1; rptp[2].y = 3;
		rptp[3].x = 6; rptp[3].y = 3;
		rptp[4].x = 1; rptp[4].y = 2;
		rptp[5].x = 7; rptp[5].y = 3;
		rptp[6].x = 9; rptp[6].y = 1;
		rptp[7].x = 1; rptp[7].y = 3;


		rmtp = new glm::ivec2[8];
		rmtp[0].x = 0; rmtp[0].y = 1;
		rmtp[1].x = 1; rmtp[1].y = 0;
		rmtp[2].x = 0; rmtp[2].y = 3;
		rmtp[3].x = 0; rmtp[3].y = 5;
		rmtp[4].x = 1; rmtp[4].y = 1;
		rmtp[5].x = 2; rmtp[5].y = 5;
		rmtp[6].x = 4; rmtp[6].y = 0;
		rmtp[7].x = 4; rmtp[7].y = 2;
	}
	else {								//DECIDIR TP LVL2
		aptp = new glm::ivec2[6];
		aptp[0].x = 7; aptp[0].y = 1;
		aptp[1].x = 8; aptp[1].y = 3;
		aptp[2].x = 6; aptp[2].y = 1;
		aptp[3].x = 9; aptp[3].y = 1;
		aptp[4].x = 7; aptp[4].y = 1; 
		aptp[5].x = 2; aptp[5].y = 3;


		amtp = new glm::ivec2[6];
		amtp[0].x = 2; amtp[0].y = 0;
		amtp[1].x = 0; amtp[1].y = 3;
		amtp[2].x = 2; amtp[2].y = 1;
		amtp[3].x = 4; amtp[3].y = 0;
		amtp[4].x = 4; amtp[4].y = 0;
		amtp[5].x = 3; amtp[5].y = 2;


		rptp = new glm::ivec2[6];
		rptp[0].x = 1; rptp[0].y = 1;
		rptp[1].x = 2; rptp[1].y = 1;
		rptp[2].x = 2; rptp[2].y = 1;
		rptp[3].x = 2; rptp[3].y = 2;
		rptp[4].x = 2; rptp[4].y = 1;
		rptp[5].x = 2; rptp[5].y = 1;


		rmtp = new glm::ivec2[6];
		rmtp[0].x = 0; rmtp[0].y = 2;
		rmtp[1].x = 3; rmtp[1].y = 0;
		rmtp[2].x = 3; rmtp[2].y = 1;
		rmtp[3].x = 3; rmtp[3].y = 1;
		rmtp[4].x = 1; rmtp[4].y = 2;
		rmtp[5].x = 3; rmtp[5].y = 1;
	}
}

void Scene::teleport(glm::ivec2 posT) {
	int tp = 0;
	if (nivell){
		for (int i = 0; i < 8; i++) {
			if (aptp[i].x == posT.x && aptp[i].y == posT.y && amtp[i].x == posM.x && amtp[i].y == posM.y) { //TP LVL1
				tp = i;
			}
		}
	}
	else {
		for (int i = 0; i < 6; i++) {
			if (aptp[i].x == posT.x && aptp[i].y == posT.y && amtp[i].x == posM.x && amtp[i].y == posM.y) { //TP LVL2
				tp = i;
			}
		}
	}
	posM.x = rmtp[tp].x;
	posM.y = rmtp[tp].y;
	init2(0, tp);
}


void Scene::drawSoldiers(){
	posSoldier = new glm::vec2[map->getnXnY().x * map->getnXnY().y];
	dirSoldier = new bool[map->getnXnY().x * map->getnXnY().y];
	soldiers = new bool[map->getnXnY().x * map->getnXnY().y];
	for (int i = 0; i < map->getnXnY().x * map->getnXnY().y; i++){
		posSoldier[i].x = -1;
		posSoldier[i].y = -1;
		soldiers[i] = false;
	}
	if (nivell){				//ENEMYS LEVEL 1
		posSoldier[4].x = 6;
		posSoldier[4].y = 1;
		dirSoldier[4] = true;

		posSoldier[6].x = 6;
		posSoldier[6].y = 2;
		dirSoldier[6] = false;

		posSoldier[16].x = 6;
		posSoldier[16].y = 3;
		dirSoldier[16] = true;

		posSoldier[12].x = 2;
		posSoldier[12].y = 1;
		dirSoldier[12] = false;

		posSoldier[22].x = 5;
		posSoldier[22].y = 3;
		dirSoldier[22] = false;

		posSoldier[26].x = 4;
		posSoldier[26].y = 3;
		dirSoldier[26] = false;

		posSoldier[27].x = 5;
		posSoldier[27].y = 2;
		dirSoldier[27] = false;

		posSoldier[9].x = 6;
		posSoldier[9].y = 3;
		dirSoldier[9] = false;

	}
	else{
		posSoldier[6].x = 5;
		posSoldier[6].y = 3;
		dirSoldier[6] = true;

		posSoldier[5].x = 6;
		posSoldier[5].y = 3;
		dirSoldier[5] = false;

		posSoldier[15].x = 5;
		posSoldier[15].y = 3;
		dirSoldier[15] = true;

		posSoldier[4].x = 3;
		posSoldier[4].y = 1;
		dirSoldier[4] = true;

		posSoldier[12].x = 4;
		posSoldier[12].y = 2;
		dirSoldier[12] = true;

		posSoldier[16].x = 5;
		posSoldier[16].y = 1;
		dirSoldier[16] = true;

		posSoldier[17].x = 3;
		posSoldier[17].y = 1;
		dirSoldier[17] = true;
								//ENEMYS LEVEL2
	}

	for (int i = 0; i < map->getnXnY().x * map->getnXnY().y; i++){
		if (posSoldier[i].x != -1){
			Soldier *soldier = new Soldier();
			soldier->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram,dirSoldier[i]);
			soldier->setPosition(glm::vec2(posSoldier[i].x*map->getTileSize().x, posSoldier[i].y*map->getTileSize().y + 2));
			soldier->setTileMap(map);
			soldiersvec.push_back(soldier);
			soldiers[i] = true;
		}
		 else {
			 Soldier *soldier = new Soldier();
			soldiersvec.push_back(soldier);
			soldiers[i] = false;
		}
	}
}

void Scene::drawBoss(){

	posMapBoss.x = -1;
	posMapBoss.y = -1;

	if (nivell){
		posMapBoss.x = 4;  // A MANO AQUI DECIDIR
		posMapBoss.y = 2; // DONDE VA EL BOSS LVL1
		posTileBoss.x = 4; //TIENE QUE HAVER 1 POR MAPA  I CUANDO MUERE APARECE EL TP VERDE Q TE LLEVA AL MENU PRINCIPAL
		posTileBoss.y = 3;
		dirBoss = false;
	}
	else {
		posMapBoss.x = 3;  // A MANO AQUI DECIDIR
		posMapBoss.y = 3; // DONDE VA EL BOSS LV1
		posTileBoss.x = 4;
		posTileBoss.y = 3;
		dirBoss = false;
	}

	if ((posM.x == posMapBoss.x && posM.y == posMapBoss.y) && !bossdead){
		boss = new Boss();

		boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, dirBoss);
		boss->setPosition(glm::vec2(posTileBoss.x * map->getTileSize().x, posTileBoss.y * map->getTileSize().y + 2));
		boss->setTileMap(map);

		bboss = true;
	}
	else bboss = false;
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
	lands2 = new bool[landspos.size()];
	cont = new int[landspos.size()];
	for (int i = 0; i < landspos.size(); i++){
		Land *land = new Land();
		land->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		land->setPosition(glm::vec2(landspos[i].x * map->getTileSize().x, landspos[i].y * map->getTileSize().y));
		land->setTileMap(map);
		landsvec.push_back(*land);
		cont[i] = -1;
		lands2[i] = true;
	}
	if (landsvec.size() > 0) lands = true;
	else lands = false;
}


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
	posSoldierScreen = NULL;
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
	if (posSoldierScreen != NULL)
		delete posSoldierScreen;
	if (dirSoldier != NULL)
		delete dirSoldier;
	if (soldiers != NULL)
		delete soldiers;
	if (lands2 != NULL)
		delete lands2;
	if (cont != NULL)
		delete cont;
	
}

void Scene::init(int lvl)
{
	posM.x = 0;
	posM.y = 0;
	level = lvl; 
	init_teleport();
	initShaders();

	if (level == 1){
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, posM);
	}
	else if (level == 2){
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, posM);
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

	puls = false;
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

	init_soldiers();
	init_boss();
	init_torchs();
	init_doors();
	init_traps();
	init_lands();

	contGAMEOVER = 100;
	LEVELCOMPLETED = false;
}

void Scene::init_screen(int mov, int tp)  //mov=1 right, mov=2 left, mov=3 down, mov=4 up
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
	
	map->prepareScreen(texProgram, posM);

	player->setPosition(glm::vec2(x, y)); 
	player->setTileMap(map);

	torchsvec.clear();
	doorsvec.clear();
	trapsvec.clear();
	landsvec.clear();

	init_boss();
	init_torchs();
	init_doors();
	init_traps();
	init_lands();
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

	//if soldier in the screen, here we do the interaction
	if (soldiers[posM.y * map->getnXnY().x + posM.x] != -1){
		soldiersvec[soldiers[posM.y * map->getnXnY().x + posM.x]]->update(deltaTime, p, a, k, v);
		glm::vec2 pp = soldiersvec[soldiers[posM.y * map->getnXnY().x + posM.x]]->getPosition();
		ps = soldiersvec[soldiers[posM.y * map->getnXnY().x + posM.x]]->getPositionTile();
		int aa = soldiersvec[soldiers[posM.y * map->getnXnY().x + posM.x]]->getCurrentAnim();
		int kk = soldiersvec[soldiers[posM.y * map->getnXnY().x + posM.x]]->getCurrentKeyframe(aa);
		player->update(deltaTime, pp, aa, kk,false);
	}

	//if boss in the screen, interaction with player
	if (bboss){
		boss->update(deltaTime, p, a, k, v);
		glm::vec2 pp = boss->getPosition();
		ps = boss->getPositionTile();
		int aa = boss->getCurrentAnim();
		int kk = boss->getCurrentKeyframe(aa);
		player->update(deltaTime, pp, aa, kk,true);
	}
	
	//If torch in the screen, do the animation
	if (torchs){
		for (int i = 0; i < torchsvec.size(); i++)
			torchsvec[i].update(deltaTime);
	}

	//If player stay on heal -- heal interaction
	if (map->getTile(pt) == 12 && !player->saltando()) {
		map->changeTile(texProgram, pt, posM,4);
		player->update(deltaTime, 1);
	}

	if (map->getTile(pt) == 37 && !player->saltando()) {
		teleport(pt);
	}

	//Open the door if player stay on the button
	if (map->getTile(pt) == 13) {
		map->prepareArrayTile(texProgram, pt, 36);
		puls = true;
	}
	else puls = false;

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

	//if traps in the screen, traps animation and interaction with player and soldier
	if (traps){
		for (int i = 0; i < trapsvec.size(); i++){
			trapsvec[i].update(deltaTime);
			glm::vec2 pp = trapsvec[i].getPosition();
			int aa = trapsvec[i].getCurrentAnim();
			int kk = trapsvec[i].getCurrentKeyframe(aa);
			player->update(deltaTime, pp, aa, kk);
			if (soldiers[posM.y * map->getnXnY().x + posM.x] != -1) soldiersvec[soldiers[posM.y * map->getnXnY().x + posM.x]]->update(deltaTime, pp, aa, kk);
		}
	}

	//if lands in the screen, lands animation and interaction with player
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

	//Vida player, if it is 0 --- GAME OVER
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
		go->update(deltaTime, level);
	}

	//Update vida soldier
	if (soldiers[posM.y * map->getnXnY().x + posM.x] != -1){
		vidaSold = soldiersvec[soldiers[posM.y * map->getnXnY().x + posM.x]]->getVida();
		if (vidaSold < 0) vidaSold = 0;
		vidas->update(deltaTime, vidaSold);
	}

	//Update vida boss
	if (bboss){
		vidaBoss = boss->getVida();
		if (vidaBoss <= 0) {
			vidaBoss = 0;
		}
		vidab->update(deltaTime, boss->getVida());
		if (boss->getVida() == 0){
			bossdead = true;
			if (level == 1) map->changeTile(texProgram, glm::ivec2(8, 3), posM, 42); //When you kill boss, new green tp appear.
			else if (level == 2) map->changeTile(texProgram, glm::ivec2(8, 3), posM, 42);
		}
	}

	//How to end the level
	if (level == 1){
		if (posM.x == 4 && posM.y == 2){
			if (player->getPositionTile().x == 8 && player->getPositionTile().y == 3){ // If you use green tp you go back main menu.
				if (bossdead){
					lc = new levelcompleted();
					lc->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					lc->setPosition(glm::vec2(30, 60));
					LEVELCOMPLETED = true;
				}
			}
		}
	}
	else if (level == 2) {
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

	if (LEVELCOMPLETED) lc->update(deltaTime, level);

	//When player goes out limits of the screen, we put him another one
	if (p.x > 294) init_screen(1, 0);           
	else if (p.x < -30) init_screen(2, 0);
	else if (p.y > 160) init_screen(3, 0);
	else if (p.y < -52) init_screen(4, 0);
}

void Scene::render()
{
	glm::mat4 modelview;

	//Render every object if it has to.

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

	if (soldiers[posM.y * map->getnXnY().x + posM.x] != -1) soldiersvec[soldiers[posM.y * map->getnXnY().x + posM.x]]->render();
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
	if (soldiers[posM.y * map->getnXnY().x + posM.x] != -1 && soldiersvec[soldiers[posM.y * map->getnXnY().x + posM.x]]->getVida() > 0) vidas->render();
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

void Scene::init_teleport() {
	if (level == 1){						//CHOOSE TP LVL 1

		nTP = 8;
		amtp = new glm::ivec2[nTP];
		amtp[0].x = 1; amtp[0].y = 0;
		amtp[1].x = 0; amtp[1].y = 1;
		amtp[2].x = 1; amtp[2].y = 2;
		amtp[3].x = 3; amtp[3].y = 3;
		amtp[4].x = 0; amtp[4].y = 5;
		amtp[5].x = 1; amtp[5].y = 1;
		amtp[6].x = 3; amtp[6].y = 5;
		amtp[7].x = 4; amtp[7].y = 1;


		aptp = new glm::ivec2[nTP];		//size of aptp = amtp = rptp = rmtp = Number of teleports
		aptp[0].x = 7; aptp[0].y = 3;   //aptp -- choose tp blue position inside screen.
		aptp[1].x = 5; aptp[1].y = 1;   //amtp -- choose which screen
		aptp[2].x = 8; aptp[2].y = 1;   //rptp -- choose tp red position inside map screen.
		aptp[3].x = 4; aptp[3].y = 1;   //rmtp -- choose which screen  
		aptp[4].x = 7; aptp[4].y = 1;   //So in lvl 1 we have 8 teleport (8 blue that teleport you to 8 red) 
		aptp[5].x = 9; aptp[5].y = 2;   //Example: for tp 0
		aptp[6].x = 4; aptp[6].y = 3;   //tp 0 blue is in screen[1,0] position[7,3]
		aptp[7].x = 9; aptp[7].y = 3;   //tp 0 red is in screen[0,1] position[1,1]


		rmtp = new glm::ivec2[nTP];
		rmtp[0].x = 0; rmtp[0].y = 1;
		rmtp[1].x = 1; rmtp[1].y = 0;
		rmtp[2].x = 0; rmtp[2].y = 3;
		rmtp[3].x = 0; rmtp[3].y = 5;
		rmtp[4].x = 1; rmtp[4].y = 1;
		rmtp[5].x = 2; rmtp[5].y = 5;
		rmtp[6].x = 4; rmtp[6].y = 0;
		rmtp[7].x = 4; rmtp[7].y = 2;

		rptp = new glm::ivec2[nTP];
		rptp[0].x = 1; rptp[0].y = 1;
		rptp[1].x = 3; rptp[1].y = 3;
		rptp[2].x = 1; rptp[2].y = 3;
		rptp[3].x = 6; rptp[3].y = 3;
		rptp[4].x = 1; rptp[4].y = 2;
		rptp[5].x = 7; rptp[5].y = 3;
		rptp[6].x = 9; rptp[6].y = 1;
		rptp[7].x = 1; rptp[7].y = 3;


	}
	else if (level == 2) {								//CHOOSE TP LVL2
		
		nTP = 6;
		amtp = new glm::ivec2[nTP];
		amtp[0].x = 2; amtp[0].y = 0;
		amtp[1].x = 0; amtp[1].y = 3;
		amtp[2].x = 2; amtp[2].y = 1;
		amtp[3].x = 4; amtp[3].y = 0;
		amtp[4].x = 4; amtp[4].y = 0;
		amtp[5].x = 3; amtp[5].y = 2;

		aptp = new glm::ivec2[nTP];
		aptp[0].x = 7; aptp[0].y = 1;
		aptp[1].x = 8; aptp[1].y = 3;
		aptp[2].x = 6; aptp[2].y = 1;
		aptp[3].x = 9; aptp[3].y = 1;
		aptp[4].x = 7; aptp[4].y = 1; 
		aptp[5].x = 2; aptp[5].y = 3;

		rmtp = new glm::ivec2[nTP];
		rmtp[0].x = 0; rmtp[0].y = 2;
		rmtp[1].x = 3; rmtp[1].y = 0;
		rmtp[2].x = 3; rmtp[2].y = 1;
		rmtp[3].x = 3; rmtp[3].y = 1;
		rmtp[4].x = 1; rmtp[4].y = 2;
		rmtp[5].x = 3; rmtp[5].y = 1;

		rptp = new glm::ivec2[nTP];
		rptp[0].x = 1; rptp[0].y = 1;
		rptp[1].x = 2; rptp[1].y = 1;
		rptp[2].x = 2; rptp[2].y = 1;
		rptp[3].x = 2; rptp[3].y = 2;
		rptp[4].x = 2; rptp[4].y = 1;
		rptp[5].x = 2; rptp[5].y = 1;

	}
}

void Scene::teleport(glm::ivec2 posT) {
	int tp = 0;
	for (int i = 0; i < nTP; i++) {
		if (aptp[i].x == posT.x && aptp[i].y == posT.y && amtp[i].x == posM.x && amtp[i].y == posM.y) { //TP LVL1
			tp = i;
		}
	}
	posM.x = rmtp[tp].x;
	posM.y = rmtp[tp].y;
	init_screen(0, tp);
}


void Scene::init_soldiers(){
	soldiers = new int[map->getnXnY().x * map->getnXnY().y];
	for (int i = 0; i < map->getnXnY().x * map->getnXnY().y; i++){
		soldiers[i] = -1;
	}
	
	if (level == 1){
		posSoldierScreen = new glm::ivec2[8];
		dirSoldier = new bool[8];
		
		//Select screen like tp. But here we do j * mapsize.x + i. 
		//Soldier 0 is in screen (4,0) at position (6,1) looking left;
		posSoldierScreen[0].x = 6;
		posSoldierScreen[0].y = 1;
		dirSoldier[0] = true;
		soldiers[4] = 0; 
				
		posSoldierScreen[1].x = 6;
		posSoldierScreen[1].y = 2;
		dirSoldier[1] = false;
		soldiers[6] = 1;

		posSoldierScreen[2].x = 6;
		posSoldierScreen[2].y = 3;
		dirSoldier[2] = true;
		soldiers[16] = 2;

		posSoldierScreen[3].x = 2;
		posSoldierScreen[3].y = 1;
		dirSoldier[3] = false;
		soldiers[12] = 3;

		posSoldierScreen[4].x = 4;
		posSoldierScreen[4].y = 3;
		dirSoldier[4] = false;
		soldiers[26] = 4;

		posSoldierScreen[5].x = 5;
		posSoldierScreen[5].y = 3;
		dirSoldier[5] = false;
		soldiers[22] = 5;

		posSoldierScreen[6].x = 5;
		posSoldierScreen[6].y = 2;
		dirSoldier[6] = true;
		soldiers[27] = 6;

		posSoldierScreen[7].x = 6;
		posSoldierScreen[7].y = 3;
		dirSoldier[9] = true;
		soldiers[9] = 7;

		
		for (int i = 0; i < 8; i++){
			Soldier *soldier = new Soldier();
			soldier->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, dirSoldier[i]);
			soldier->setPosition(glm::vec2(posSoldierScreen[i].x * map->getTileSize().x , posSoldierScreen[i].y*map->getTileSize().y + 2));
			soldier->setTileMap(map);
			soldiersvec.push_back(soldier);
		}
	}
	else if (level == 2){

		posSoldierScreen = new glm::ivec2[7];
		dirSoldier = new bool[7];

		posSoldierScreen[0].x = 5;
		posSoldierScreen[0].y = 3;
		dirSoldier[0] = true;
		soldiers[6] = 0;

		posSoldierScreen[1].x = 6;
		posSoldierScreen[1].y = 3;
		dirSoldier[1] = false;
		soldiers[5] = 1;

		posSoldierScreen[2].x = 5;
		posSoldierScreen[2].y = 3;
		dirSoldier[2] = true;
		soldiers[15] = 2;

		posSoldierScreen[3].x = 3;
		posSoldierScreen[3].y = 1;
		dirSoldier[3] = true;
		soldiers[4] = 3;

		posSoldierScreen[4].x = 4;
		posSoldierScreen[4].y = 2;
		dirSoldier[4] = true;
		soldiers[12] = 4;

		posSoldierScreen[5].x = 5;
		posSoldierScreen[5].y = 1;
		dirSoldier[5] = true;
		soldiers[16] = 5;

		posSoldierScreen[6].x = 3;
		posSoldierScreen[6].y = 1;
		dirSoldier[6] = true;
		soldiers[17] = 6;

		for (int i = 0; i < 7; i++){
			Soldier *soldier = new Soldier();
			soldier->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, dirSoldier[i]);
			soldier->setPosition(glm::vec2(posSoldierScreen[i].x * map->getTileSize().x , posSoldierScreen[i].y*map->getTileSize().y + 2));
			soldier->setTileMap(map);
			soldiersvec.push_back(soldier);
		}

	}
}

void Scene::init_boss(){

	posMapBoss.x = -1;
	posMapBoss.y = -1;

	if (level == 1){
		posMapBoss.x = 4;  // A MANO AQUI DECIDIR
		posMapBoss.y = 2; // DONDE VA EL BOSS LVL1
		posTileBoss.x = 4; //TIENE QUE HAVER 1 POR MAPA  I CUANDO MUERE APARECE EL TP VERDE Q TE LLEVA AL MENU PRINCIPAL
		posTileBoss.y = 3;
		dirBoss = false;
	}
	else if (level == 2) {
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

//Objects init values are read from the lvl.txt file

void Scene::init_torchs(){
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

void Scene::init_doors(){
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

void Scene::init_traps(){
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

void Scene::init_lands(){
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


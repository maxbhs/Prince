#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Soldier.h"
#include "Boss.h"
#include "VidaPlayer.h"
#include "VidaSoldier.h"
#include "VidaBoss.h"
#include "Torch.h"
#include "Door.h"
#include "Trap.h"
#include "Land.h"
#include "gameover.h"
#include "levelcompleted.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(bool lvl);
	void init2(int mov, int tp);
	void update(int deltaTime);
	void render();

	void ini_teleport();
	void teleport(glm::ivec2 posT);

	void drawSoldiers();
	void drawBoss();
	void drawTorchs();
	void drawDoors();
	void drawTraps();
	void drawLands();


private:
	void initShaders();

private:
	TileMap *map;
	VidaPlayer *vidap;
	VidaSoldier *vidas;
	VidaBoss *vidab;
	vector<Torch> torchsvec;
	vector<Door> doorsvec;
	vector<Trap> trapsvec;
	vector<Land> landsvec;
	vector<Soldier*> soldiersvec;
	glm::ivec2 *aptp;
	glm::ivec2 *amtp;
	glm::ivec2 *rptp;
	glm::ivec2 *rmtp;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::ivec2 posM, posMaux;
	glm::vec2 *posSoldier;
	glm::ivec2 posMapBoss;
	glm::vec2 posTileBoss;
	bool dirBoss;
	bool *dirSoldier;
	bool torchs, doors, traps, lands, bboss;
	bool *soldiers;
	bool nivell;
	bool bossdead;
	int vidaPlay, vidaSold, vidaBoss;
	bool *lands2;
	Player *player;
	Boss *boss;
	gameover *go;
	levelcompleted *lc;
	bool puls;
	int *cont;
	int contGAMEOVER;
	bool LEVELCOMPLETED;
};


#endif // _SCENE_INCLUDE


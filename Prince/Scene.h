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

	
	void init(int lvl);  //init is called only one time per level
	void init_screen(int mov, int tp);//when you pass throught different screens in the same lvl init2 is called

	void update(int deltaTime);
	void render();

	void init_teleport(); //init position of tp
	void teleport(glm::ivec2 posT);//TeleportPlayer


	//Init objects traps and enemys, called every screen.
	void init_soldiers();
	void init_boss();
	void init_torchs();
	void init_doors();
	void init_traps();
	void init_lands();


private:
	void initShaders();

private:
	TileMap *map;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::ivec2 posM;
	int level;
	int nTP;

	bool torchs, doors, traps, lands, puls;
	bool *lands2;
	int *cont;
	vector<Torch> torchsvec;
	vector<Door> doorsvec;
	vector<Trap> trapsvec;
	vector<Land> landsvec;
	vector<Soldier*> soldiersvec;

	glm::ivec2 *aptp;
	glm::ivec2 *amtp;
	glm::ivec2 *rptp;
	glm::ivec2 *rmtp;

	Player *player;
	Boss *boss;

	int *soldiers;
	glm::ivec2 *posSoldierScreen;
	bool *dirSoldier;

	glm::ivec2 posMapBoss;
	glm::vec2 posTileBoss;
	bool dirBoss, bossdead, bboss;

	int vidaPlay, vidaSold, vidaBoss;
	VidaPlayer *vidap;
	VidaSoldier *vidas;
	VidaBoss *vidab;

	gameover *go;
	levelcompleted *lc;
	int contGAMEOVER;
	bool LEVELCOMPLETED;
};


#endif // _SCENE_INCLUDE


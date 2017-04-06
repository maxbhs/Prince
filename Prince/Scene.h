#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Soldier.h"
#include "Interface.h"
#include "Torch.h"
#include "Door.h"
#include "Trap.h"
#include "Land.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void init2(int mov, int tp);
	void update(int deltaTime);
	void render();

	void ini_teleport();
	void teleport(glm::ivec2 posT);

	void drawEnemy();
	void drawTorchs();
	void drawDoors();
	void drawTraps();
	void drawLands();


private:
	void initShaders();

private:
	TileMap *map;
	Interface *inter;
	vector<Torch> torchsvec;
	vector<Door> doorsvec;
	vector<Trap> trapsvec;
	vector<Land> landsvec;
	vector<Soldier> soldiersvec;
	glm::ivec2 *aptp;
	glm::ivec2 *amtp;
	glm::ivec2 *rptp;
	glm::ivec2 *rmtp;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::ivec2 posM, posMaux;
	glm::vec2 *posEnemy;
	bool *dirEnemy;
	bool torchs, doors, traps, lands;
	bool *soldiers;
	Player *player;
	//Soldier *soldier;
	int vidaPlayer, vidaEnemy;
	bool puls;
	int *cont;
};


#endif // _SCENE_INCLUDE


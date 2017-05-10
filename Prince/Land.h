#ifndef _LAND_INCLUDE
#define _LAND_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Land
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, glm::ivec2 posPlayerTile, glm::ivec2 posSoldierTile, bool bJ);
	void render();

	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition();
	void setTileMap(TileMap *tileMap);
	glm::ivec2 getPositionTile();
	bool getStepOn();
	int getCurrentAnim();

private:
	glm::ivec2 tileMapDispl, posLand;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool stepOn, firstColl;
	int cont;
};


#endif // _Land_INCLUDE
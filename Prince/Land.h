#ifndef _LAND_INCLUDE
#define _LAND_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Land
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, glm::ivec2 posPlayerTile);
	void render();

	void setPosition(const glm::vec2 &pos);
	void setTileMap(TileMap *tileMap);
	glm::ivec2 getPositionTile();
	bool getStepOn();

private:
	glm::ivec2 tileMapDispl, posLand;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool stepOn;
	int cont;
};


#endif // _Land_INCLUDE
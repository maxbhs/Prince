#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Door
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, bool open);
	void render();

	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPositionTile();

private:
	glm::ivec2 tileMapDispl, posDoor;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _DOOR_INCLUDE


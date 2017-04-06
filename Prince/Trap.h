#ifndef _TRAP_INCLUDE
#define _TRAP_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Trap
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPositionTile();
	glm::vec2 getPosition();
	int getCurrentAnim();

private:
	glm::ivec2 tileMapDispl, posTrap;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _TRAP_INCLUDE
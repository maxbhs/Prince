#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class levelcompleted
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, bool lvl);
	void render();

	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posLC;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _LEVEL_INCLUDE


#ifndef _INTER_INCLUDE
#define _INTER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Interface
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, int vida);
	void render();

	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posInter;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _INTER_INCLUDE


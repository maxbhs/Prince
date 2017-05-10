#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class credits
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posInst;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _CREDITS_INCLUDE
#ifndef _INSTRUC_INCLUDE
#define _INSTRUC_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class instruccions
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


#endif // _INSTRUC_INCLUDE

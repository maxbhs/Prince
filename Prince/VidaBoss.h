#ifndef _VB_INCLUDE
#define _VB_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class VidaBoss
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, int vida);
	void render();

	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posVB;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _VB_INCLUDE

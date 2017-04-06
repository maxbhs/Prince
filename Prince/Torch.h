#ifndef _TORCH_INCLUDE
#define _TORCH_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class Torch
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posTor;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _TORCH_INCLUDE

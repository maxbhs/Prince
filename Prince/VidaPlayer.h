#ifndef _VP_INCLUDE
#define _VP_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class VidaPlayer
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, int vida);
	void render();

	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posVP;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _VP_INCLUDE


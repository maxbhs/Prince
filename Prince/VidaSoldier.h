#ifndef _VS_INCLUDE
#define _VS_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class VidaSoldier
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, int vida);
	void render();

	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posVS;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _VS_INCLUDE
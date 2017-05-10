#ifndef _GAMEOVER_INCLUDE
#define _GAMEOVER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class gameover
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, bool lvl);
	void render();

	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posGO;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _GAMEOVER_INCLUDE
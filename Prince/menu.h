#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include "Sprite.h"
#include "instruccions.h"
#include "credits.h"
#include "TileMap.h"




class menu
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);

private:
	glm::ivec2 tileMapDispl, posMenu;
	Texture spritesheet;
	Sprite *sprite;
	instruccions *instruc;
	credits *credit;
	ShaderProgram texProgram;
	bool ins, cred;

};


#endif // _MENU_INCLUDE
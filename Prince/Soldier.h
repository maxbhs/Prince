#ifndef _SOLDIER_INCLUDE
#define _SOLDIER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Soldier is basically a Sprite that represents an enemy. As such it has
// all properties it needs to track its movement, action and collisions.


class Soldier
{

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool leftright);
	void update(int deltaTime, glm::vec2 posPlayer, int animPlayer, int keyFrameAnim, int vidaPlayer); //update normal
	void update(int deltaTime, glm::vec2 posEnemy, int anim, int key); //update trap
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
	glm::vec2 getPosition();
	glm::ivec2 getPositionTile();

	int getVida();
	int getCurrentAnim();
	int getCurrentKeyframe(int animId);
	void setLeftRight(bool b);

private:

	bool leftright;
	glm::ivec2 tileMapDispl, posSoldier;
	int startY;
	int vida, cd, lastkey, contkey;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};


#endif // _SOLDIER_INCLUDE


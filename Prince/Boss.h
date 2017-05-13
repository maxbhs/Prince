#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Boss is basically a Sprite that represents an enemy. As such it has
// all properties it needs to track its movement and actions.


class Boss
{

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool leftright);
	void update(int deltaTime, glm::vec2 posPlayer, int animPlayer, int keyFrameAnim, int vidaPlayer);
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
	glm::ivec2 tileMapDispl, posBoss;
	int startY;
	int vida, cd, cdulti, lastkey, contkey;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};


#endif // _BOSS_INCLUDE




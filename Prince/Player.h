#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, glm::ivec2 &posM); //update normal
	void update(int deltaTime, int vida); //update heal
	void update(int deltaTime, glm::vec2 posTrap, int anim, int key); //update trap
	void update(int deltaTime, glm::vec2 posEnemy, int anim, int key, bool boss); //update enemy
	void render();
	
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition();
    glm::ivec2 getPositionTile();
	int getVida();
	int getCurrentAnim();
	int getCurrentKeyframe(int animId);
	bool saltando();
	void ha_caido();

private:

	bool bJumping, bDown, bFalling, bFighting, bHealing;
	bool downPressed, upPressed, efePressed, cPressed;
	bool collLeft, collRight, leftright;
	glm::ivec2 posPlayer;
	glm::ivec2 tileMapDispl, lastposM;
	int jumpAngle, startY;
	int vida, cd, lastkey, contkey;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int diffposM;
};


#endif // _PLAYER_INCLUDE



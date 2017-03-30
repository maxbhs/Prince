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
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition();
	
private:

	bool canIClimb();
	bool canIFall();

	bool bJumping, bDown, bFalling, canClimb, downPressed, upPressed, left;
	bool collLeft, collRight;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	char last;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};


#endif // _PLAYER_INCLUDE



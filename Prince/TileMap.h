#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include <vector>
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, const glm::vec2 &posM);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, const glm::vec2 &posM);
	~TileMap();

	void render_back(ShaderProgram &program);
	void render_front(ShaderProgram &program);
	void render_puls(ShaderProgram &program);
	void free();

	void prepareArrayTile(ShaderProgram &program, glm::ivec2 posT, int tile);
	void preparePotion(ShaderProgram &program, const glm::ivec2 posT, const glm::ivec2 posM);
	void prepareMap(ShaderProgram &program, const glm::vec2 &posM);
	
	glm::ivec2 getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, const bool &leftright) const;

	glm::ivec2 getnXnY();
	vector<glm::ivec2> getTorchsPos();
	vector<glm::ivec2> getDoorsPos();
	vector<glm::ivec2> getTrapsPos();
	vector<glm::ivec2> getLandsPos();

	void setTile(glm::ivec2,int tile);
	int getTile(glm::ivec2 pos);

private:
	bool loadLevel(const string &levelFile, const glm::ivec2 &posM);
	void cargamapa(const glm::ivec2 &posM);
	void prepareArrayBack(ShaderProgram &program);
	void prepareArrayFront(ShaderProgram &program);
	

private:
	GLuint vao, vao2, vao3;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	glm::ivec2 tileSize, blockSize;
	Texture tilesheet;
	ShaderProgram shaderProgram;
	glm::vec2 tileTexSize, minCoord;
	int *map, *mapA;
	int ntilesFront;
	int nX, nY, mapAX, mapAY;

};


#endif // _TILE_MAP_INCLUDE



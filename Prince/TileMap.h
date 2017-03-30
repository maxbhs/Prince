#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render_back(ShaderProgram &program);
	void render_front(ShaderProgram &program);
	void free();
	
	glm::ivec2 getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrayBack(ShaderProgram &program);
	void prepareArrayFront(ShaderProgram &program);

private:
	GLuint vao, vao2;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	glm::ivec2 tileSize, blockSize;
	Texture tilesheet;
	ShaderProgram shaderProgram;
	glm::vec2 tileTexSize, minCoord;
	int *map;
	int ntilesFront;

};


#endif // _TILE_MAP_INCLUDE



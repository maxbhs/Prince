#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include <glm/gtc/matrix_transform.hpp>

//nou
using namespace std;

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	shaderProgram = program;
	minCoord = minCoords;
	prepareArrays(program);

}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render_back(ShaderProgram &texProgram)
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::render_front(glm::ivec2 posPlayer, ShaderProgram &texProgram){

	prepareTile(texProgram,posPlayer);
	
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao2);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize.x >> tileSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> blockSize.x >> blockSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if (tile >= 'A' && tile < 'a') map[j*mapSize.x + i] = (tile - int('A')) + 10;
			else if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else
				map[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2];
	vector<float> vertices;
	
	for (int j = mapSize.y-1; j>=0; j--)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoord.x + i * tileSize.x, (minCoord.y + j * tileSize.y)-1);
				texCoordTile[0] = glm::vec2(float((tile-1)%8) / tilesheetSize.x, float((tile-1)/8) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y + blockSize.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y + blockSize.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

void TileMap::prepareTile(ShaderProgram &program, glm::ivec2 posPlayer){


	glm::vec2 posTile, texCoordTile[2];
	vector<float> vertices;
	int x = (posPlayer.x+tileSize.x/2) / tileSize.x;
	int y = (posPlayer.y+tileSize.y/2) / tileSize.y;
	int tile = map[(y)*tileSize.x + x];
	tile = 28;

	//vector<int> tiles = { 3, 17, 25, 29, 30, 31, 33 };

	//for (int i = 0; i < tiles.size(); i++){
		//if (tile == tiles[i]){
			//if (tile == 3 ||tile == 17 || tile == 25) tile = 28;
			posTile = glm::vec2(minCoord.x + (2)* tileSize.x, (minCoord.y + 2* tileSize.y)-1);
			texCoordTile[0] = glm::vec2(float((tile - 1) % 8) / tilesheetSize.x, float((tile - 1) / 8) / tilesheetSize.y);
			texCoordTile[1] = texCoordTile[0] + tileTexSize;

			// First triangle
			vertices.push_back(posTile.x); vertices.push_back(posTile.y);
			vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
			vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y);
			vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
			vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y + blockSize.y);
			vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
			// Second triangle
			vertices.push_back(posTile.x); vertices.push_back(posTile.y);
			vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
			vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y + blockSize.y);
			vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
			vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize.y);
			vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			glGenVertexArrays(1, &vao2);
			glBindVertexArray(vao2);
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
			posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
			texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
		//}
	//}
}



// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	int px = pos.x;
	int py = pos.y;

	
	x = px / tileSize.x;
	y0 = py / tileSize.y;
	y1 = (py + size.y - 1) / tileSize.y;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y*mapSize.x + x] == 11) return true;
		else {
			for (int i = 29; i < 34; i++){
				if (map[y*mapSize.x + x] == i) return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	int px = pos.x;
	int py = pos.y;

	x = (px + size.x - 1) / tileSize.x;
	y0 = py / tileSize.y;
	y1 = (py + size.y - 1) / tileSize.y;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y*mapSize.x + x] == 11) return true;
		else {
			for (int i = 29; i < 34; i++){
				if (map[y*mapSize.x + x] == i) return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x0, x1, y;

	int py = pos.y;

	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = (pos.y + size.y - 1) / tileSize.y;
	for (int x = x0; x <= x1; x++){
		if (map[(y)*mapSize.x + x] != 27){
			if (py - tileSize.y * y + size.y <= 4){
				return true;
			}
		}
	}

	return false;
}































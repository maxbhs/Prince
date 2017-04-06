#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include <glm/gtc/matrix_transform.hpp>

//nou
using namespace std;

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, const glm::vec2 &posM)
{
	TileMap *map = new TileMap(levelFile, minCoords, program, posM);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, const glm::vec2 &posM)
{
	loadLevel(levelFile, posM);//cambiar con la posicion del mapa a pintar!
	cargamapa(posM);
	shaderProgram = program;
	minCoord = minCoords;
	prepareArrayBack(program);
	prepareArrayFront(program); //cambiado

}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}

void TileMap::prepareMap(ShaderProgram &program, const glm::vec2 &posM) {
	cargamapa(posM);
	shaderProgram = program;
	prepareArrayBack(program);
	prepareArrayFront(program);
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

void TileMap::render_front(ShaderProgram &texProgram){

	
	
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao2);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * ntilesFront); //cambiado
	glDisable(GL_TEXTURE_2D);
}

void TileMap::render_puls(ShaderProgram &texProgram){

	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao3);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6); //cambiado
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile, const glm::ivec2 &posM)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> nX >> nY;
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
	mapAX = (mapSize.x + (10 * (nX - 1))); //21
	mapAY = (mapSize.y + (3 * (nY - 1))); //7
	mapA = new int[mapAX * mapAY]; // 21 * 7
	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j<mapAY; j++)
	{
		for (int i = 0; i < mapAX; i++)
		{
			fin.get(tile);
			if (tile >= 'A' && tile < 'a') mapA[j*mapAX + i] = (tile - int('A')) + 10;
			else if (tile == 'a') mapA[j*mapAX + i] = 37;
			else if (tile == 'r') mapA[j*mapAX + i] = 38;
			else
				mapA[j*mapAX + i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::cargamapa(const glm::ivec2 &posM)
{
	    //A PARTIR DE AQUI LO METERMOS EN OTRA FUNCION PARA QUE NO 
	for (int j = 0; j < mapSize.y; j++)		 //LEA EL BLOC DE NOTAS MIL VECES!!!! TENDREMOS QUE DECLARAR mapA EN EL .h !!!
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			map[j*mapSize.x + i] = mapA[((j + (3 * (posM.y)))*mapAX) + (i + (10 * (posM.x)))];
			//map[j*mapSize.x + i] = mapA[((j+3)*mapAX) + i];
		}
	}

}

void TileMap::prepareArrayBack(ShaderProgram &program)
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

void TileMap::prepareArrayFront(ShaderProgram &program){


	glm::vec2 posTile, texCoordTile[2];
	vector<float> vertices;
	
	int ntiles = 0; //poner a 0 una vez programado el bucle!!
	int tile;
	bool pintar = false;

	for (int j = mapSize.y - 1; j >= 0; j--) {
		for (int i = 0; i < mapSize.x; i++) {
			tile = map[j * mapSize.x + i];
			if (tile == 3 || tile == 17 || tile == 25) { //tile columna
				tile = 28;
				pintar = true;
			}
			else if (tile == 11 || tile == 18 || tile == 23) { //tile porta
				tile = 34;
				pintar = true;
			}
			else if (tile >= 32 && tile <= 33) {
				//if (tile != 33) tile = 32; //hablarlo con el max i cambiar el tile de la columna SOLA
				pintar = true; //arreglar el problema
			}
			if (pintar) {
				pintar = false;
				ntiles++;

				posTile = glm::vec2(minCoord.x + (i)* tileSize.x, (minCoord.y + j * tileSize.y) - 1);
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
				
			}
		}
	}
		glGenVertexArrays(1, &vao2);
		glBindVertexArray(vao2);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 24 * ntiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
		texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
		ntilesFront = ntiles; //cambiado
}


void TileMap::prepareArrayTile(ShaderProgram &program, glm::ivec2 posT, int t){


	glm::vec2 posTile, texCoordTile[2];
	vector<float> vertices;

	int tile = t;

	posTile = glm::vec2(minCoord.x + (posT.x)* tileSize.x, (minCoord.y + posT.y * tileSize.y) - 1);
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

	glGenVertexArrays(1, &vao3);
	glBindVertexArray(vao3);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * 1 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

void TileMap::preparePotion(ShaderProgram &program, const glm::ivec2 posT, const glm::ivec2 posM) {
	mapA[((posT.y + (3 * (posM.y)))*mapAX) + (posT.x + (10 * (posM.x)))] = 4;
	map[posT.y*mapSize.x + posT.x] = 4;
	prepareArrayBack(program);
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x = (pos.x + size.x/4 -5) / tileSize.x;
	int y = (pos.y + 10)/ tileSize.y;

	if (map[y*mapSize.x + x] == 11 || map[y*mapSize.x + x] == 18 || map[y*mapSize.x + x] == 23) return true;
	else {
		for (int i = 29; i < 34; i++){
			if (map[y*mapSize.x + x] == i) return true;
		}
	}
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	
	int x = (pos.x + size.x/4 +10)/ tileSize.x;
	int y = (pos.y + 10)/ tileSize.y;

	if (map[y*mapSize.x + x] == 11 || map[y*mapSize.x + x] == 18 || map[y*mapSize.x + x] == 23) return true;
	else {
		for (int i = 29; i < 34; i++){
			if (map[y*mapSize.x + x] == i) return true;
		}
	}
	return false;
}


bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const {

	int x = (pos.x + size.x/4)/ tileSize.x;
	int y = (pos.y)/ tileSize.y;

	if (map[y * mapSize.x + x] != 27){
		if (pos.y >= tileSize.y * y){
			*posY = y*tileSize.y + 2;
			return true;
		}
	}
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, const bool &leftright) const {
	int x0 = (pos.x + size.x/4 +10) / tileSize.x;
	int x1 = (pos.x + size.x/4 -10) / tileSize.x;
	int y = (pos.y + size.y / 2) / tileSize.y;
	
		if (leftright){
		if (map[(y - 1)*mapSize.x + x0] == 27){
			for (int i = 27; i < 34; i++){
				if (map[(y - 1)*mapSize.x + x0 -1] == i) return false;
				
			}
			if (float(pos.x + size.x/4 + 10) - float((x0)*tileSize.x) <= 15) return true;
		}
	}
	else {
		if (map[(y - 1)*mapSize.x + x1] == 27){
			for (int i = 27; i < 34; i++){
				if (map[(y - 1) * mapSize.x + x1 +1] == i) return false;
			}
			if (float((x1+1)*tileSize.x) - float(pos.x+ size.x/4) -10 <= 5) return true;
		}
	}
	return false;
}

glm::ivec2 TileMap::getnXnY(){
	glm::ivec2 n = glm::ivec2(nX,nY);
	return n;

}

vector<glm::ivec2> TileMap::getTorchsPos(){
	vector<glm::ivec2> pos;
	for (int i = 0; i < mapSize.x; i++){
		for (int j = 0; j < mapSize.y; j++)
			if (map[j*mapSize.x+i] == 1 || map[j*mapSize.x + i] == 15){
				glm::ivec2 p;
				p.x = i;
				p.y = j;
				pos.push_back(p);
		}
	}
	return pos;
}

vector<glm::ivec2> TileMap::getDoorsPos(){
	vector<glm::ivec2> pos;
	for (int i = 0; i < mapSize.x; i++){
		for (int j = 0; j < mapSize.y; j++)
			if (map[j*mapSize.x + i] == 11 || map[j*mapSize.x + i] == 18 || map[j*mapSize.x + i] == 23){
				glm::ivec2 p;
				p.x = i;
				p.y = j;
				pos.push_back(p);
			}
	}
	return pos;
}

vector<glm::ivec2> TileMap::getTrapsPos(){
	vector<glm::ivec2> pos;
	for (int i = 0; i < mapSize.x; i++){
		for (int j = 0; j < mapSize.y; j++)
			if (map[j*mapSize.x + i] == 6){
				glm::ivec2 p;
				p.x = i;
				p.y = j;
				pos.push_back(p);
			}
	}
	return pos;
}

vector<glm::ivec2> TileMap::getLandsPos(){
	vector<glm::ivec2> pos;
	for (int i = 0; i < mapSize.x; i++){
		for (int j = 0; j < mapSize.y; j++)
			if (map[j*mapSize.x + i] == 9){
				glm::ivec2 p;
				p.x = i;
				p.y = j;
				pos.push_back(p);
			}
	}
	return pos;
}

void TileMap::setTile(glm::ivec2 pos, int tile){
	map[pos.y*mapSize.x + pos.x] = tile;
}

int TileMap::getTile(glm::ivec2 pos) {
	return map[pos.y*mapSize.x + pos.x];
}

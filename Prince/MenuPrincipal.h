#ifndef _MP_INCLUDE
#define _MP_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "menu.h"
#include "Scene.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class MenuPrincipal
{

public:
	MenuPrincipal();
	~MenuPrincipal();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	menu *menuprin;

};


#endif // _MP_INCLUDE

#pragma once

#include "IUpdateDraw.h"
#include "../Engine.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "CubeBatch.h"
#include "InputManager.h"
#include "../Objects/MapObject.h"
#include "Environment.h"
#include <array>

class TestSence  : public IUpdateDraw
{
public:
	TestSence();
	~TestSence();
	void Update() override ;
	void Draw() override;

	std::array<glm::vec3, 10> cubePositions;

	Texture* p_Texture;
	int tw, th;

	glm::mat4 view;
	glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);;

	glm::vec3 pos = glm::vec3(0, 0, 3);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);


	MapObject* pMapObject;
};
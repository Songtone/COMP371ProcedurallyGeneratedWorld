#include "building.h"
#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

// Textures http://thumbs.modthesims2.com/img/1/7/8/2/8/2/MTS_plasticbox-901242-cwindow00_overview.jpg

building::building()
{
	srand(time(NULL));
	seed = rand() % 1000000000;
}


building::~building()
{
}

std::vector<glm::vec3> building::generate(glm::vec3 position) {
	// creates vertices by averaging random value of given position
	int height = getHeight(position);
	int widht = getWidht(position);

	std::vector<int> leftValues;
	int index = 1;
	while (true) {
		int left = getHeight(position + glm::vec3(-MAX_WIDHT*index, 0, 0));
		if (left == height){
			leftValues.push_back(left);
			index++;
		}
		else {
			break;
		}
	}

	std::vector<int> rigthValues;
	index = 1;
	while (true) {
		int rigth = getHeight(position + glm::vec3(MAX_WIDHT*index, 0, 0));
		if (rigth == height) {
			rigthValues.push_back(rigth);
			index++;
		}
		else {
			break;
		}
	}

	srand(leftValues.size() + rigthValues.size() + height + seed);
	int textureValue = rand() % TOTAL_TEXTURES + 3;

	std::vector<glm::vec3> vertices;

	// Center face
	// TRIANGLE 1
	// bottom left
	vertices.push_back(position);
	vertices.push_back(glm::vec3(textureValue, 0, 0));

	// top right
	vertices.push_back(position + glm::vec3(0, height, widht));
	vertices.push_back(glm::vec3(textureValue, widht, height));

	// bottom right
	vertices.push_back(position + glm::vec3(0, 0, widht));
	vertices.push_back(glm::vec3(textureValue, widht, 0));

	// TRIANGLE 2
	// bottom left
	vertices.push_back(position);
	vertices.push_back(glm::vec3(textureValue, 0, 0));

	// top left
	vertices.push_back(position + glm::vec3(0, height, 0));
	vertices.push_back(glm::vec3(textureValue, 0, height));

	// top right
	vertices.push_back(position + glm::vec3(0, height, widht));
	vertices.push_back(glm::vec3(textureValue, widht, height));

	// Left face
	// TRIANGLE 1
	// top left
	vertices.push_back(position + glm::vec3(0, height, 0));
	vertices.push_back(glm::vec3(textureValue, widht, height));

	// far bottom left
	vertices.push_back(position + glm::vec3(-widht, 0, 0));
	vertices.push_back(glm::vec3(textureValue, 0, 0));

	// bottom left
	vertices.push_back(position);
	vertices.push_back(glm::vec3(textureValue, widht, 0));

	// TRIANGLE 2
	// far bottom left
	vertices.push_back(position + glm::vec3(-widht, 0, 0));
	vertices.push_back(glm::vec3(textureValue, 0, 0));

	// far top left
	vertices.push_back(position + glm::vec3(-widht, height, 0));
	vertices.push_back(glm::vec3(textureValue, 0, height));

	// top left
	vertices.push_back(position + glm::vec3(0, height, 0));
	vertices.push_back(glm::vec3(textureValue, widht, height));

	// right face
	// TRIANGLE 1
	// far bottom right
	vertices.push_back(position + glm::vec3(-widht, 0, widht));
	vertices.push_back(glm::vec3(textureValue, widht, 0));

	// top right
	vertices.push_back(position + glm::vec3(0, height, widht));
	vertices.push_back(glm::vec3(textureValue, 0, height));

	// bottom right
	vertices.push_back(position + glm::vec3(0, 0, widht));
	vertices.push_back(glm::vec3(textureValue, 0, 0));

	// TRIANGLE 2
	// far bottom right
	vertices.push_back(position + glm::vec3(-widht, 0, widht));
	vertices.push_back(glm::vec3(textureValue, widht, 0));

	// far top left
	vertices.push_back(position + glm::vec3(-widht, height, widht));
	vertices.push_back(glm::vec3(textureValue, widht, height));

	// top right
	vertices.push_back(position + glm::vec3(0, height, widht));
	vertices.push_back(glm::vec3(textureValue, 0, height));

	return vertices;
}

std::vector<glm::vec3> building::generateOp(glm::vec3 position) {
	// creates vertices by averaging random value of given position
	int height = getHeight(position);
	int widht = getWidht(position);

	std::vector<int> leftValues;
	int index = 1;
	while (true) {
		int left = getHeight(position + glm::vec3(-MAX_WIDHT*index, 0, 0));
		if (left == height){
			leftValues.push_back(left);
			index++;
		}
		else {
			break;
		}
	}

	std::vector<int> rigthValues;
	index = 1;
	while (true) {
		int rigth = getHeight(position + glm::vec3(MAX_WIDHT*index, 0, 0));
		if (rigth == height) {
			rigthValues.push_back(rigth);
			index++;
		}
		else {
			break;
		}
	}

	srand(leftValues.size() + rigthValues.size() + height + seed);
	int textureValue = rand() % TOTAL_TEXTURES + 3;

	std::vector<glm::vec3> vertices;

	// Center face
	// TRIANGLE 1
	// bottom left
	vertices.push_back(position);
	vertices.push_back(glm::vec3(textureValue, 0, 0));

	// bottom right
	vertices.push_back(position + glm::vec3(0, 0, widht));
	vertices.push_back(glm::vec3(textureValue, widht, 0));

	// top right
	vertices.push_back(position + glm::vec3(0, height, widht));
	vertices.push_back(glm::vec3(textureValue, widht, height));

	// TRIANGLE 2
	// bottom left
	vertices.push_back(position);
	vertices.push_back(glm::vec3(textureValue, 0, 0));

	// top right
	vertices.push_back(position + glm::vec3(0, height, widht));
	vertices.push_back(glm::vec3(textureValue, widht, height));

	// top left
	vertices.push_back(position + glm::vec3(0, height, 0));
	vertices.push_back(glm::vec3(textureValue, 0, height));

	// Left face
	// TRIANGLE 1
	// top left
	vertices.push_back(position + glm::vec3(0, height, 0));
	vertices.push_back(glm::vec3(textureValue, widht, height));

	// bottom left
	vertices.push_back(position);
	vertices.push_back(glm::vec3(textureValue, widht, 0));

	// far bottom left
	vertices.push_back(position + glm::vec3(widht, 0, 0));
	vertices.push_back(glm::vec3(textureValue, 0, 0));

	// TRIANGLE 2
	// far bottom left
	vertices.push_back(position + glm::vec3(widht, 0, 0));
	vertices.push_back(glm::vec3(textureValue, 0, 0));

	// top left
	vertices.push_back(position + glm::vec3(0, height, 0));
	vertices.push_back(glm::vec3(textureValue, widht, height));

	// far top left
	vertices.push_back(position + glm::vec3(widht, height, 0));
	vertices.push_back(glm::vec3(textureValue, 0, height));

	// right face
	// TRIANGLE 1
	// far bottom right
	vertices.push_back(position + glm::vec3(widht, 0, widht));
	vertices.push_back(glm::vec3(textureValue, widht, 0));

	// bottom right
	vertices.push_back(position + glm::vec3(0, 0, widht));
	vertices.push_back(glm::vec3(textureValue, 0, 0));

	// top right
	vertices.push_back(position + glm::vec3(0, height, widht));
	vertices.push_back(glm::vec3(textureValue, 0, height));

	// TRIANGLE 2
	// far bottom right
	vertices.push_back(position + glm::vec3(widht, 0, widht));
	vertices.push_back(glm::vec3(textureValue, widht, 0));

	// top right
	vertices.push_back(position + glm::vec3(0, height, widht));
	vertices.push_back(glm::vec3(textureValue, 0, height));

	// far top left
	vertices.push_back(position + glm::vec3(widht, height, widht));
	vertices.push_back(glm::vec3(textureValue, widht, height));

	return vertices;
}

int building::getHeight(glm::vec3 position) {
	// get procedural height
	int left = getNoiseH(position - glm::vec3(MAX_WIDHT, 0, 0));
	int right = getNoiseH(position + glm::vec3(MAX_WIDHT, 0, 0));
	int middle = getNoiseH(position);

	int height = (left + middle + right) / 3;
	height = height / STEP_SIZE;

	return height * STEP_SIZE;
}

int building::getWidht(glm::vec3 position) {
	// get procedural widht
	int widht = getNoiseW(position);

	widht = widht / STEP_SIZE;

	return widht * STEP_SIZE;
}

// value noise
int building::getNoiseH(glm::vec3 position) {
	// get procdeural value
	srand(position.x + position.y + position.z + seed);
	int height = (rand() % MAX_HEIGHT + 1);
	return height;
}

int building::getNoiseW(glm::vec3 position) {
	// get procdeural value
	srand(position.x + position.y + position.z + seed);
	int height = (rand() % MAX_WIDHT + 1);
	return height;
}

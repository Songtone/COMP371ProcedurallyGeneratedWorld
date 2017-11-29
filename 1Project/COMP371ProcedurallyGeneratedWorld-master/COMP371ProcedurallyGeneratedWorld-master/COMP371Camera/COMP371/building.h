#pragma once
#include "glm.hpp"
#include <time.h>
#include <vector>

class building
{
private:
	int seed;
	int const MAX_WIDHT = 50;
	int const MAX_HEIGHT = 40;
	int const TOTAL_TEXTURES = 6;
	int const STEP_SIZE = 10;
public:
	building();
	~building();
	std::vector<glm::vec3> generate(glm::vec3 position);
	std::vector<glm::vec3> generateOp(glm::vec3 position);
	int getHeight(glm::vec3 position);
	int getWidht(glm::vec3 position);
	int getNoiseH(glm::vec3 position);
	int getNoiseW(glm::vec3 position);
};


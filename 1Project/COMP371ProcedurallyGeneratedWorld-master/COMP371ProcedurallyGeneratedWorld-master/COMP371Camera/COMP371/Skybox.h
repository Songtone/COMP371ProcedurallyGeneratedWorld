#ifndef SKYBOX_H
#define SKYBOX_H

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <string>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "..\glm\glm.hpp"
#include <vector>

using namespace std;

class Skybox
{

	void loadSkybox(GLuint skyTextures[], string filename, int ID){
		glGenTextures(1, &skyTextures[ID]);
		glBindTexture(GL_TEXTURE_2D, skyTextures[ID]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		unsigned char *image8 = stbi_load("window6.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	}

};

#endif
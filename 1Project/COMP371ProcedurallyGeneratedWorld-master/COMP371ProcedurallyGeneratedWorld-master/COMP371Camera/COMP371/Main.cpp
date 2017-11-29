//example skeleton code
//modified from http://learnopengl.com/

//TEAMPROJECT CAMERA

#include "stdafx.h"
#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include "camera.h"
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "objloader.hpp"  //include the object loader
#include "building.h"
#include "building.cpp"
#include "stb_image.h"


using namespace std;

//variables
bool wantCursor = false;

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 800;

glm::vec3 camera_position;
glm::vec3 triangle_scale;
glm::mat4 projection_matrix;

//camera
Camera camera(glm::vec3(0.0f, 1.0f, 10.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// Constant vectors
//const glm::vec3 center(0.0f, 0.0f, 0.0f);
//const glm::vec3 up(0.0f, 1.0f, 0.0f);
//const glm::vec3 eye(0.0f, 0.0f, 10.0f);


//functions used
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);


// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Load Procedure", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// tell GLFW to not capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);

	// Build and compile our shader program
	// Vertex shader

	// Read the Vertex Shader code from the file
	string vertex_shader_path = "vertex.shader";
	string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

	if (VertexShaderStream.is_open()) {
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	string fragment_shader_path = "fragment.shader";
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
		getchar();
		exit(-1);
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader); //free up memory
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);
	
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width_image, height_image, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image = stbi_load("road2.jpg", &width_image, &height_image, &nrChannels, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image, height_image, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width_image2, height_image2, nrChannels2;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image2 = stbi_load("grass.jpg", &width_image2, &height_image2, &nrChannels2, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image2, height_image2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	stbi_image_free(image2);

	GLuint texture3;
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width_image3, height_image3, nrChannels3;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image3 = stbi_load("window1.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image3);
	stbi_image_free(image3);

	GLuint texture4;
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image4 = stbi_load("window2.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image4);
	stbi_image_free(image4);

	GLuint texture5;
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image5 = stbi_load("window3.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image5);
	stbi_image_free(image5);

	GLuint texture6;
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image6 = stbi_load("window4.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image6);
	stbi_image_free(image6);

	GLuint texture7;
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image7 = stbi_load("window5.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image7);
	stbi_image_free(image7);
	
	GLuint texture8;
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image8 = stbi_load("window6.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image8);
	stbi_image_free(image8);
	
	
	GLuint texture9;
	glGenTextures(1, &texture9);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image9 = stbi_load("skybox_front.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image9);
	stbi_image_free(image9);
	
	GLuint texture10;
	glGenTextures(1, &texture10);
	glBindTexture(GL_TEXTURE_2D, texture10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image10 = stbi_load("skybox_right.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image10);
	stbi_image_free(image10);
	
	GLuint texture11;
	glGenTextures(1, &texture11);
	glBindTexture(GL_TEXTURE_2D, texture11);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image11 = stbi_load("skybox_back.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image11);
	stbi_image_free(image11);

	GLuint texture12;
	glGenTextures(1, &texture12);
	glBindTexture(GL_TEXTURE_2D, texture12);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image12 = stbi_load("skybox_left.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image12);
	stbi_image_free(image12);

	GLuint texture13;
	glGenTextures(1, &texture13);
	glBindTexture(GL_TEXTURE_2D, texture13);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image13 = stbi_load("skybox_up.jpg", &width_image3, &height_image3, &nrChannels3, STBI_rgb);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_image3, height_image3, 0, GL_RGB, GL_UNSIGNED_BYTE, image13);
	stbi_image_free(image13);
	
	//Creating a plane --- 3 separate ones, which will be alterated between each other to segment the streets.
	// FORMAT for every point is
	// POINT vec3(x, y, z)
	//
	// TEXTURE INFO vec3(texture# , x, y)
	// x,y are UVs / coordinate to texture
	glm::vec3 ULB = glm::vec3(-1000, 1000, -1000);
	glm::vec3 ULF = glm::vec3(-1000, 1000, 1000);
	glm::vec3 URB = glm::vec3(1000, 1000, -1000);
	glm::vec3 URF = glm::vec3(1000, 1000, 1000);
	glm::vec3 DLB = glm::vec3(-1000, -1000, -1000);
	glm::vec3 DLF = glm::vec3(-1000, -1000, 1000);
	glm::vec3 DRB = glm::vec3(1000, -1000, -1000);
	glm::vec3 DRF = glm::vec3(1000, -1000, 1000);

	vector<glm::vec3> skyup;
	skyup.push_back(ULB);
	skyup.push_back(glm::vec3(13.0f, 0.0f, 1.0f));
	skyup.push_back(URB);
	skyup.push_back(glm::vec3(13.0f, 0.0f, 0.0f));
	skyup.push_back(URF);
	skyup.push_back(glm::vec3(13.0f, 1.0f, 0.0f));

	skyup.push_back(ULB);
	skyup.push_back(glm::vec3(13.0f, 0.0f, 1.0f));
	skyup.push_back(URF);
	skyup.push_back(glm::vec3(13.0f, 1.0f, 0.0f));
	skyup.push_back(ULF);
	skyup.push_back(glm::vec3(13.0f, 1.0f, 1.0f));

	vector<glm::vec3> skyfront;
	skyfront.push_back(DLF);
	skyfront.push_back(glm::vec3(9.0f, 1.0f, 0.0f));
	skyfront.push_back(DRF);
	skyfront.push_back(glm::vec3(9.0f, 0.0f, 0.0f));
	skyfront.push_back(URF);
	skyfront.push_back(glm::vec3(9.0f, 0.0f, 1.0f));

	skyfront.push_back(DLF);
	skyfront.push_back(glm::vec3(9.0f, 1.0f, 0.0f));
	skyfront.push_back(URF);
	skyfront.push_back(glm::vec3(9.0f, 0.0f, 1.0f));
	skyfront.push_back(ULF);
	skyfront.push_back(glm::vec3(9.0f, 1.0f, 1.0f));

	vector<glm::vec3> skyleft;
	skyleft.push_back(DLB);
	skyleft.push_back(glm::vec3(10.0f, 1.0f, 0.0f));
	skyleft.push_back(DLF);
	skyleft.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
	skyleft.push_back(ULF);
	skyleft.push_back(glm::vec3(10.0f, 0.0f, 1.0f));

	skyleft.push_back(DLB);
	skyleft.push_back(glm::vec3(10.0f, 1.0f, 0.0f));
	skyleft.push_back(ULF);
	skyleft.push_back(glm::vec3(10.0f, 0.0f, 1.0f));
	skyleft.push_back(ULB);
	skyleft.push_back(glm::vec3(10.0f, 1.0f, 1.0f));

	vector<glm::vec3> skyright;
	skyright.push_back(DRF);
	skyright.push_back(glm::vec3(12.0f, 1.0f, 0.0f));
	skyright.push_back(DRB);
	skyright.push_back(glm::vec3(12.0f, 0.0f, 0.0f));
	skyright.push_back(URB);
	skyright.push_back(glm::vec3(12.0f, 0.0f, 1.0f));

	skyright.push_back(DRF);
	skyright.push_back(glm::vec3(12.0f, 1.0f, 0.0f));
	skyright.push_back(URB);
	skyright.push_back(glm::vec3(12.0f, 0.0f, 1.0f));
	skyright.push_back(URF);
	skyright.push_back(glm::vec3(12.0f, 1.0f, 1.0f));

	vector<glm::vec3> skyback;
	skyback.push_back(DLB);
	skyback.push_back(glm::vec3(11.0f, 0.0f, 0.0f));
	skyback.push_back(DRB);
	skyback.push_back(glm::vec3(11.0f, 1.0f, 0.0f));
	skyback.push_back(URB);
	skyback.push_back(glm::vec3(11.0f, 1.0f, 1.0f));

	skyback.push_back(DLB);
	skyback.push_back(glm::vec3(11.0f, 0.0f, 0.0f));
	skyback.push_back(URB);
	skyback.push_back(glm::vec3(11.0f, 1.0f, 1.0f));
	skyback.push_back(ULB);
	skyback.push_back(glm::vec3(11.0f, 0.0f, 1.0f));


	vector<glm::vec3> road;
	road.push_back(glm::vec3(-5.0f, 0.0f, -20.0f));
	road.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	road.push_back(glm::vec3(-5.0f, 0.0f, 20.0f));
	road.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
	road.push_back(glm::vec3(5.0f, 0.0f, -20.0f));
	road.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	road.push_back(glm::vec3(-5.0f, 0.0f, 20.0f));
	road.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
	road.push_back(glm::vec3(5.0f, 0.0f, 20.0f));
	road.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	road.push_back(glm::vec3(5.0f, 0.0f, -20.0f));
	road.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	vector<glm::vec3> road_next;
	road_next.push_back(glm::vec3(-5.0f, 0.0f, -60.0f));
	road_next.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	road_next.push_back(glm::vec3(-5.0f, 0.0f, -20.0f));
	road_next.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
	road_next.push_back(glm::vec3(5.0f, 0.0f, -60.0f));
	road_next.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	road_next.push_back(glm::vec3(-5.0f, 0.0f, -20.0f));
	road_next.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
	road_next.push_back(glm::vec3(5.0f, 0.0f, -20.0f));
	road_next.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	road_next.push_back(glm::vec3(5.0f, 0.0f, -60.0f));
	road_next.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	vector<glm::vec3> road_prev;
	road_prev.push_back(glm::vec3(-5.0f, 0.0f, 20.0f));
	road_prev.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	road_prev.push_back(glm::vec3(-5.0f, 0.0f, 60.0f));
	road_prev.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
	road_prev.push_back(glm::vec3(5.0f, 0.0f, 20.0f));
	road_prev.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	road_prev.push_back(glm::vec3(-5.0f, 0.0f, 60.0f));
	road_prev.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
	road_prev.push_back(glm::vec3(5.0f, 0.0f, 60.0f));
	road_prev.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	road_prev.push_back(glm::vec3(5.0f, 0.0f, 20.0f));
	road_prev.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	vector<glm::vec3> grass;
	grass.push_back(glm::vec3(-100.0f, -0.1f, -20.0f));
	grass.push_back(glm::vec3(2.0f, 0.0f, 0.0f));
	grass.push_back(glm::vec3(-100.0f, -0.1f, 20.0f));
	grass.push_back(glm::vec3(2.0f, 0.0f, 1.0f));
	grass.push_back(glm::vec3(100.0f, -0.1f, -20.0f));
	grass.push_back(glm::vec3(2.0f, 1.0f, 0.0f));

	grass.push_back(glm::vec3(-100.0f, -0.1f, 20.0f));
	grass.push_back(glm::vec3(2.0f, 0.0f, 1.0f));
	grass.push_back(glm::vec3(100.0f, -0.1f, 20.0f));
	grass.push_back(glm::vec3(2.0f, 1.0f, 1.0f));
	grass.push_back(glm::vec3(100.0f, -0.1f, -20.0f));
	grass.push_back(glm::vec3(2.0f, 1.0f, 0.0f));

	GLuint VAO_road;
	glGenVertexArrays(1, &VAO_road);
	GLuint VBO_road;
	glGenBuffers(1, &VBO_road);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_road);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_road);
	glBufferData(GL_ARRAY_BUFFER, road.size() * sizeof(glm::vec3), &road.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint VAO_road_next;
	glGenVertexArrays(1, &VAO_road_next);
	GLuint VBO_road_next;
	glGenBuffers(1, &VBO_road_next);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_road_next);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_road_next);
	glBufferData(GL_ARRAY_BUFFER, road_next.size() * sizeof(glm::vec3), &road_next.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint VAO_road_prev;
	glGenVertexArrays(1, &VAO_road_prev);
	GLuint VBO_road_prev;
	glGenBuffers(1, &VBO_road_prev);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_road_prev);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_road_prev);
	glBufferData(GL_ARRAY_BUFFER, road_prev.size() * sizeof(glm::vec3), &road_prev.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint VAO_grass;
	glGenVertexArrays(1, &VAO_grass);
	GLuint VBO_grass;
	glGenBuffers(1, &VBO_grass);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_grass);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_grass);
	glBufferData(GL_ARRAY_BUFFER, grass.size() * sizeof(glm::vec3), &grass.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	
	GLuint VAO_skyboxfront;
	glGenVertexArrays(1, &VAO_skyboxfront);
	GLuint VBO_skyboxfront, VBO_skyboxleft, VBO_skyboxback, VBO_skyboxright, VBO_skyboxup;
	
	glGenBuffers(1, &VBO_skyboxfront);
	glBindVertexArray(VBO_skyboxfront);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skyboxfront);
	glBufferData(GL_ARRAY_BUFFER, skyfront.size() * sizeof(glm::vec3), &skyfront.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint VAO_skyboxleft;
	glGenVertexArrays(1, &VAO_skyboxleft);
	glGenBuffers(1, &VBO_skyboxleft);
	glBindVertexArray(VBO_skyboxleft);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skyboxleft);
	glBufferData(GL_ARRAY_BUFFER, skyleft.size() * sizeof(glm::vec3), &skyleft.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint VAO_skyboxback;
	glGenVertexArrays(1, &VAO_skyboxback);
	glGenBuffers(1, &VBO_skyboxback);
	glBindVertexArray(VBO_skyboxback);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skyboxback);
	glBufferData(GL_ARRAY_BUFFER, skyback.size() * sizeof(glm::vec3), &skyback.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint VAO_skyboxup;
	glGenVertexArrays(1, &VAO_skyboxup);
	glGenBuffers(1, &VBO_skyboxup);
	glBindVertexArray(VBO_skyboxup);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skyboxup);
	glBufferData(GL_ARRAY_BUFFER, skyup.size() * sizeof(glm::vec3), &skyup.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	GLuint VAO_skyboxright;
	glGenVertexArrays(1, &VAO_skyboxright);

	glGenBuffers(1, &VBO_skyboxright);
	glBindVertexArray(VBO_skyboxright);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_skyboxright);
	glBufferData(GL_ARRAY_BUFFER, skyright.size() * sizeof(glm::vec3), &skyright.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	building build;
	std::vector<glm::vec3> temp;
	std::vector<glm::vec3> builder = builder = build.generate(road_prev[0] + glm::vec3(-0.5f, 0.0f, 0.0f));
	temp = build.generateOp(road_prev[4] + glm::vec3(0.5f, 0.0f, 0.0f));
	builder.insert(builder.end(), temp.begin(), temp.end());
	temp = build.generate(road[0] + glm::vec3(-0.5f, 0.0f, 0.0f));
	builder.insert(builder.end(), temp.begin(), temp.end());
	temp = build.generateOp(road[4] + glm::vec3(0.5f, 0.0f, 0.0f));
	builder.insert(builder.end(), temp.begin(), temp.end());
	temp = build.generate(road_next[0] + glm::vec3(-0.5f, 0.0f, 0.0f));
	builder.insert(builder.end(), temp.begin(), temp.end());
	temp = build.generateOp(road_next[4] + glm::vec3(0.5f, 0.0f, 0.0f));
	builder.insert(builder.end(), temp.begin(), temp.end());

	for (int j = 0; j < builder.size(); j++)
	{
		std::cout << builder[j].x << std::endl;
	}

	GLuint VAO_building;
	glGenVertexArrays(1, &VAO_building);
	GLuint VBO_building;
	glGenBuffers(1, &VBO_building);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_building);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_building);
	glBufferData(GL_ARRAY_BUFFER, builder.size() * sizeof(glm::vec3), &builder.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// To verify that new platform has loaded
	bool platformChange = false;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	triangle_scale = glm::vec3(1.0f);

	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");

	GLuint object_type_loc = glGetUniformLocation(shaderProgram, "object_type");

	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture3"), 2);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture4"), 3);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture5"), 4);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture6"), 5);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture7"), 6);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture8"), 7);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture9"), 8);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture10"), 9);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture11"), 10);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture12"), 11);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture13"), 12);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 model_matrix;

		/*glm::mat4 view_matrix;
		view_matrix = glm::lookAt(eye, center, up);

		glm::mat4 model_matrix;
		model_matrix = glm::scale(model_matrix, triangle_scale);*/


		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, texture5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, texture6);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, texture7);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, texture8);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, texture9);
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, texture11);
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, texture12);
		glActiveTexture(GL_TEXTURE12);
		glBindTexture(GL_TEXTURE_2D, texture13);

		float zPos = camera.getPositionZ();
		glBindVertexArray(VAO_road_prev);
		if (road_prev[0].z > zPos + 30) {
			for (int j = 0; j < road_prev.size(); j += 2) {
				road_prev[j].z = road_prev[j].z - 120;
			}
			glBindBuffer(GL_ARRAY_BUFFER, VBO_road_prev);
			glBufferData(GL_ARRAY_BUFFER, road_prev.size() * sizeof(glm::vec3), &road_prev.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			platformChange = true;
		}

		if (road[0].z > zPos + 30) {
			glBindVertexArray(VAO_road);
			for (int j = 0; j < road.size(); j += 2) {
				road[j].z = road[j].z - 120;
			}
			glBindBuffer(GL_ARRAY_BUFFER, VBO_road);
			glBufferData(GL_ARRAY_BUFFER, road.size() * sizeof(glm::vec3), &road.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			platformChange = true;
		}

		if (road_next[0].z > zPos + 30) {
			glBindVertexArray(VAO_road_next);
			for (int j = 0; j < road_next.size(); j += 2) {
				road_next[j].z = road_next[j].z - 120;
			}
			glBindBuffer(GL_ARRAY_BUFFER, VBO_road_next);
			glBufferData(GL_ARRAY_BUFFER, road_next.size() * sizeof(glm::vec3), &road_next.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			platformChange = true;
		}

		if (grass[0].z + 20 != zPos) {
			glBindVertexArray(VAO_grass);
			grass[0].z = zPos - 200;
			grass[2].z = zPos + 200;
			grass[4].z = zPos - 200;
			grass[6].z = zPos + 200;
			grass[8].z = zPos + 200;
			grass[10].z = zPos - 200;
			glBindBuffer(GL_ARRAY_BUFFER, VBO_grass);
			glBufferData(GL_ARRAY_BUFFER, grass.size() * sizeof(glm::vec3), &grass.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}
		
		if (skyfront[0].z + 20 != zPos) {
			glBindVertexArray(VAO_skyboxfront);
			skyfront[0].z = zPos + 1000;
			skyfront[2].z = zPos + 1000;
			skyfront[4].z = zPos + 1000;
			skyfront[6].z = zPos + 1000;
			skyfront[8].z = zPos + 1000;
			skyfront[10].z = zPos + 1000;
			glBindBuffer(GL_ARRAY_BUFFER, VBO_skyboxfront);
			glBufferData(GL_ARRAY_BUFFER, skyfront.size() * sizeof(glm::vec3), &skyfront.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		if (skyup[0].z + 20 != zPos) {
			glBindVertexArray(VAO_skyboxup);
			skyup[0].z = zPos - 1000;
			skyup[2].z = zPos - 1000;
			skyup[4].z = zPos + 1000;
			skyup[6].z = zPos - 1000;
			skyup[8].z = zPos + 1000;
			skyup[10].z = zPos + 1000;
			glBindBuffer(GL_ARRAY_BUFFER, VBO_skyboxup);
			glBufferData(GL_ARRAY_BUFFER, skyup.size() * sizeof(glm::vec3), &skyup.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		if (skyback[0].z + 20 != zPos) {
			glBindVertexArray(VAO_skyboxback);
			skyback[0].z = zPos - 1000;
			skyback[2].z = zPos - 1000;
			skyback[4].z = zPos - 1000;
			skyback[6].z = zPos - 1000;
			skyback[8].z = zPos - 1000;
			skyback[10].z = zPos - 1000;
			glBindBuffer(GL_ARRAY_BUFFER, VBO_skyboxback);
			glBufferData(GL_ARRAY_BUFFER, skyback.size() * sizeof(glm::vec3), &skyback.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		if (skyleft[0].z + 20 != zPos) {
			glBindVertexArray(VAO_skyboxleft);
			skyleft[0].z = zPos - 1000;
			skyleft[2].z = zPos + 1000;
			skyleft[4].z = zPos + 1000;
			skyleft[6].z = zPos - 1000;
			skyleft[8].z = zPos + 1000;
			skyleft[10].z = zPos - 1000;
			glBindBuffer(GL_ARRAY_BUFFER, VBO_skyboxleft);
			glBufferData(GL_ARRAY_BUFFER, skyleft.size() * sizeof(glm::vec3), &skyleft.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		if (skyright[0].z + 20 != zPos) {
			glBindVertexArray(VAO_skyboxright);
			skyright[0].z = zPos + 1000;
			skyright[2].z = zPos - 1000;
			skyright[4].z = zPos - 1000;
			skyright[6].z = zPos + 1000;
			skyright[8].z = zPos - 1000;
			skyright[10].z = zPos + 1000;
			glBindBuffer(GL_ARRAY_BUFFER, VBO_skyboxright);
			glBufferData(GL_ARRAY_BUFFER, skyright.size() * sizeof(glm::vec3), &skyright.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}
		
		//now to check if the user is going backwards
		if  (road_prev[2].z < zPos - 30) {
			glBindVertexArray(VAO_road_prev);
			for (int j = 0; j < road_prev.size(); j += 2) {
				road_prev[j].z = road_prev[j].z + 120;
			}
			glBindBuffer(GL_ARRAY_BUFFER, VBO_road_prev);
			glBufferData(GL_ARRAY_BUFFER, road_prev.size() * sizeof(glm::vec3), &road_prev.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			platformChange = true;
		}

		if (road[2].z < zPos - 30) {
			glBindVertexArray(VAO_road);
			for (int j = 0; j < road.size(); j += 2) {
				road[j].z = road[j].z + 120;
			}
			glBindBuffer(GL_ARRAY_BUFFER, VBO_road);
			glBufferData(GL_ARRAY_BUFFER, road.size() * sizeof(glm::vec3), &road.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			platformChange = true;
		}

		if (road_next[2].z < zPos - 30) {
			glBindVertexArray(VAO_road_next);
			for (int j = 0; j < road_next.size(); j += 2) {
				road_next[j].z = road_next[j].z + 120;
			}
			glBindBuffer(GL_ARRAY_BUFFER, VBO_road_next);
			glBufferData(GL_ARRAY_BUFFER, road_next.size() * sizeof(glm::vec3), &road_next.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			platformChange = true;
		}

		if (platformChange) {
			glBindVertexArray(VAO_building);
			builder.empty();

			builder = build.generate(road_prev[0] + glm::vec3(-0.5f, 0.0f, 0.0f));
			temp = build.generateOp(road_prev[4] + glm::vec3(0.5f, 0.0f, 0.0f));
			builder.insert(builder.end(), temp.begin(), temp.end());
			temp = build.generate(road[0] + glm::vec3(-0.5f, 0.0f, 0.0f));
			builder.insert(builder.end(), temp.begin(), temp.end());
			temp = build.generateOp(road[4] + glm::vec3(0.5f, 0.0f, 0.0f));
			builder.insert(builder.end(), temp.begin(), temp.end());
			temp = build.generate(road_next[0] + glm::vec3(-0.5f, 0.0f, 0.0f));
			builder.insert(builder.end(), temp.begin(), temp.end());
			temp = build.generateOp(road_next[4] + glm::vec3(0.5f, 0.0f, 0.0f));
			builder.insert(builder.end(), temp.begin(), temp.end());

			glBindBuffer(GL_ARRAY_BUFFER, VBO_building);
			glBufferData(GL_ARRAY_BUFFER, builder.size() * sizeof(glm::vec3), &builder.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			platformChange = false;
		}

		glBindVertexArray(VAO_skyboxfront);
		glDrawArrays(GL_TRIANGLES, 0, skyfront.size());
		glBindVertexArray(VAO_skyboxup);
		glDrawArrays(GL_TRIANGLES, 0, skyup.size());
		glBindVertexArray(VAO_skyboxleft);
		glDrawArrays(GL_TRIANGLES, 0, skyleft.size());
		glBindVertexArray(VAO_skyboxright);
		glDrawArrays(GL_TRIANGLES, 0, skyright.size());
		glBindVertexArray(VAO_skyboxback);
		glDrawArrays(GL_TRIANGLES, 0, skyback.size());

		glBindVertexArray(VAO_grass);
		glDrawArrays(GL_TRIANGLES, 0, grass.size());

		glBindVertexArray(VAO_road);
		glDrawArrays(GL_TRIANGLES, 0, road.size());

		glBindVertexArray(VAO_road_next);
		glDrawArrays(GL_TRIANGLES, 0, road_next.size());

		glBindVertexArray(VAO_road_prev);
		glDrawArrays(GL_TRIANGLES, 0, road_prev.size());

		glBindVertexArray(VAO_building);
		glDrawArrays(GL_TRIANGLES, 0, builder.size());

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		camera.gravity(0.005f);
		camera.standUp(0.01);
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//std::cout << key << std::endl;
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);

	if (camera.getPositionY() <= 1.3f)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			deltaTime = deltaTime * 1.50f;
		}
		else {
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
				deltaTime = deltaTime * 0.7;
				camera.ProcessKeyboard(DOWN, deltaTime);
			}

		}
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
}

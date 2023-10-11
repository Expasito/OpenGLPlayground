#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "../src/Includes.h"
// these are reimported because C++ gets angry
#include <chrono>
#include <vector>


unsigned int compileShader(const char* vertex, const char* fragment);

void checkErrors();

void initRenderer(int, int);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);

void updateProjView(glm::mat4*, glm::mat4* view, int width, int height);

void postRenderingSteps(bool log, GLFWwindow* window,
	std::chrono::time_point<std::chrono::high_resolution_clock>* start,
	glm::mat4* proj, glm::mat4* view, int width, int height);

void initProfile(int waitFrames,int numFrames, bool startProfile);

void makeIBO();


void cleanup();

unsigned int loadTexture(const char* path,
	unsigned int wrapS,
	unsigned int wrapT,
	unsigned int minFilter,
	unsigned int magFilter);

extern float vertices[];
extern size_t verticesSize;
extern GLFWwindow* window;
extern unsigned int program;

extern std::vector<glm::vec3> vertexData;
extern int vertexDataSize;
extern unsigned int* indexData;
extern int indexDataSize;
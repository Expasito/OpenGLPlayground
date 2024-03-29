#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "../src/Includes.h"
#include "../utils/Mesh.h"
// these are reimported because C++ gets angry
#include <chrono>
#include <vector>
#include <map>

class Mesh;
class Material;
class Component;

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 textCoord;
};


struct BatchedMesh {
	uint32_t batchVBO;
	uint32_t batchIBO;
	std::map<Mesh*, uint32_t>* indexMeshMap;
};


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

void makeIBO(float* vertices, uint32_t verticesSize,
	std::vector<Vertex>* vertexData,
	std::vector<uint32_t>* indexData);

// this generates random vertices
void genVerts2(int, int);


void cleanup();

BatchedMesh* genBatchedMesh(std::vector<Mesh*> meshes);

unsigned int loadTexture(const char* path,
	unsigned int wrapS,
	unsigned int wrapT,
	unsigned int minFilter,
	unsigned int magFilter);

extern float vertices[];
extern size_t verticesSize;
extern GLFWwindow* window;
extern unsigned int program;


extern glm::vec3* verts2;
extern int verts2Size;
extern unsigned int* indices2;
extern int indices2Size;

extern float cubeVertices[];
extern size_t cubeVertsSize;

extern uint32_t matAlbedo;
extern uint32_t matDiffuse;
extern uint32_t matSpecular;
extern uint32_t matShininess;
extern uint32_t matAreTextures;

extern std::map<int, uint32_t> textureMap;

extern std::vector<Material*> materials;
extern std::vector<Component*> components;

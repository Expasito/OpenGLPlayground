#pragma once
#include "Headers.h"
#include "Camera.h"
#include <Render/utils/Instances.h>
#include <Render/utils/FillerArray.h>


class Render
{
public:

	struct Debug {
		unsigned int vertices, indices, heights;
		int bars;
		float width;
		float* heightsData;
	};

	static Debug debug;

	struct vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal;
		glm::vec2 texturecoord;
	};

	// Model is a temporary holder between loading the model data and giving it to the 
	// Object class
	class Model {
	public:
		std::vector<vertex> vertices;
		std::vector<unsigned int> indices;
		Model(std::vector<vertex> verts, std::vector<unsigned int> inds);
		unsigned int positions, ebo;
	private:

	};

	class Object {
	public:
		std::string name;
		std::string path;
		FillerArray* insts;
		unsigned int positions, ebo;
		int eboSize;
		// The Render::Model class should handle putting the vertex and ebo data on the gpu
		// and just pass the buffer objects across
		Object(std::string name_, const char* path_, unsigned int positions_, unsigned int ebo_, int eboSize_, FillerArray* insts_) {
			name = name_;
			insts = insts_;
			positions = positions_;
			ebo = ebo_;
			eboSize = eboSize_;
			path = std::string(path_);
			
		}
		~Object() {
			std::cout << "here\n";
			exit();
			glDeleteBuffers(1, &positions);
			glDeleteBuffers(1, &ebo);
		}

	private:
	};
	
	static int width, height;

	// This will hold the value if they key is pressed
	static std::vector<Render::Object*> objects;
	static GLFWwindow* window;

	// these are shader programs
	static unsigned int renderShader, screenShader, debugShader, shadowShader, depthShader, multiShader;

	static bool left, right, down, up, forward, backward;
	static bool leftMouseButton, rightMouseButton, middleMouseButton;
	static Camera camera;
	static glm::mat4 model;
	static glm::mat4 view;
	static glm::mat4 projection;
	static float dt;
	static float lastFrame;
	static bool keepWindow;
	// this counts which entity index we are on for hashing. This will increment and return as the hash key
	static unsigned long long instanceCounter;
	static void init(int width, int height, bool fullScreen);
	static void exit();
	static void addModel(const char* path, std::string name, int hashtablesize, int dynamicarraysize);
	static long addInstance(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,glm::vec3 colors);
	static void editInstance(std::string name, unsigned long long id, glm::vec3 pos, glm::bvec3 rot, glm::vec3 scal, glm::vec3 color);
	static void removeInstances(std::string name);
	static void removeAllInstances();
	static void draw();
	static void renderAll();

	static void loadSave(const char* path);
	static void createSave(const char* path);
	static int getKey(int key);

	static void drawDebug(float milis);
	static void initDebugScreen();
	
	static Render::Model loadModel(const char* path);
	
	static unsigned int VAO;
private:
	static char* loadShader(const char* filepath);
	static void shaderBuildStatus(unsigned int shader, int result);
	static void compileShader(const char* filepath1, const char* filepath2, unsigned int* program);
	static std::vector<std::string> strsplit(std::string str, char dem);
	static void prepBuffers();
};


#include "Utils.h"



Camera camera(0, 0, -10);

struct UserInput {
	int leftArrow = 0;
	int rightArrow = 0;
	int upArrow = 0;
	int downArrow = 0;

	int W = 0, A = 0, S = 0, D = 0;
	int B = 0;
	int leftShift = 0;
	int leftControl = 0;
	int space = 0;
	
	int leftMouseButton = 0;
	int rightMouseButton = 0;
	int middleMouseButton = 0;
} userInput;


glm::vec3* verts2;
int verts2Size = 0;

unsigned int* indices2;
int indices2Size = 0;


uint32_t matAlbedo = -1;
uint32_t matDiffuse = -1;
uint32_t matSpecular = -1;
uint32_t matShininess = -1;
uint32_t matAreTextures = -1;

void genVerts2(int points, int displacement) {
	// make it so points is a multiple of 3
	points = points / 3 * 3;
	verts2 = (glm::vec3*)malloc(sizeof(glm::vec3) * points);
	indices2 = (unsigned int*)malloc(sizeof(unsigned int) * points);
	verts2Size = 0;

	for (int i = 0; i < points; i++) {
		float f1 = rand() % displacement;
		float f2 = rand() % displacement;
		float f3 = rand() % displacement;

		glm::vec3 temp = { f1,f2,f3 };
		short found = -1;
		for (int j = 0; j < verts2Size; j++) {
			glm::vec3 temp_ = verts2[j];
			if (temp == temp_) {
				found = j;
				//std::cout << "DUPLICATE!!\n";
			}

		}
		if (found >=0) {
			indices2[i] = found;
			indices2Size++;
			//verts2Size++;
		}
		else {
			verts2[verts2Size++] = temp;
			indices2[i] = indices2Size;
			indices2Size++;

		}
		//verts2Size++;
	}


}

float cubeVertices[] = {
	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,

	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,

	-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,

	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f
};

size_t cubeVertsSize = sizeof(cubeVertices);


float vertices[] = {

	// Front face
	-1,-1,-1,
	-1,1,-1,
	1,-1,-1,

	-1,1,-1,
	1,1,-1,
	1,-1,-1,

	// Back face
	-1,-1,1,
	-1,1,1,
	1,-1,1,

	-1,1,1,
	1,1,1,
	1,-1,1,

	// Top face
	-1,1,-1,
	-1,1,1,
	1,1,-1,

	1,1,-1,
	1,1,1,
	-1,1,1,

	// Bottom face
	-1,-1,-1,
	-1,-1,1,
	1,-1,-1,

	1,-1,-1,
	1,-1,1,
	-1,-1,1,

	// Left face
	-1,-1,-1,
	-1,1,-1,
	-1,-1,1,

	-1,-1,1,
	-1,1,1,
	-1,1,-1,

	// Right face
	1,-1,-1,
	1,1,-1,
	1,-1,1,

	1,-1,1,
	1,1,1,
	1,1,-1
};





size_t verticesSize = sizeof(vertices);

int vectorContains(std::vector<glm::vec3>* v, glm::vec3 element) {
	for (int i = 0; i < v->size(); i++) {
		glm::vec3 temp = v->at(i);
		if (temp == element) {
			return i;
		}
	}
	return -1;
}

/*
* 
* makeIBO takes in a pointer to a float array along with the number of elements
* and then edits the two vectors so they have the index and vertex data
* 
*/
void makeIBO(float* vertices, uint32_t verticesSize,
	std::vector<glm::vec3>* vertexData,
	std::vector<uint32_t>* indexData) {
	std::cout << "NUM VERTICES: " << verticesSize  << "\n";
	for (int i = 0; i < verticesSize; i+=3) {
		float f1 = vertices[i];
		float f2 = vertices[i + 1];
		float f3 = vertices[i + 2];
		//std::cout << "" << f1 << ", " << f2 << ", " << f3 << "\n";
		glm::vec3 temp = { f1,f2,f3 };
		int index = vectorContains(vertexData, temp);

		// just add regardless
		//vertexData->push_back(temp);
		//indexData->push_back(vertexData->size()-1);

		if (index >= 0) {
			indexData->push_back(vertexData->size() - 1);
		}
		else {
			vertexData->push_back(temp);
			indexData->push_back(vertexData->size()-1);
		}

		//// if in the array, set the ibo to the index
		//if (index >= 0) {
		//	indexData->push_back(index);
		//}
		//// not in the array
		//else {
		//	vertexData->push_back(temp);
		//	indexData->push_back(vertexData->size());
		//}

	}

}



void checkErrors() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "ERROR!\n";
		std::cout << err << "\n";
		exit(1);
	}
}

std::vector<float>* profileResults;
int framesProfiled = 0;
int maxFramesProfile = 0;
long totalFrames = 0;
bool shouldProfile = false;

// allow for waiting a few frames before profiling
int framesToWait = 0;
int framesWaited = 0;

void initProfile(int waitFrames, int numFrames, bool startProfile) {
	profileResults = new std::vector<float>();
	profileResults->reserve(numFrames);
	maxFramesProfile = numFrames;
	totalFrames = 0;
	shouldProfile = startProfile;
	framesToWait = waitFrames;
	framesWaited = 0;
}

void profileFrame(float milis) {
	totalFrames++;
	if (shouldProfile && (framesProfiled < maxFramesProfile)) {
		framesWaited++;
		if (framesWaited > framesToWait) {
			profileResults->push_back(milis);
			framesProfiled++;
		}
		
	}

}

void logProfile() {
	float longest = INT_MIN;
	float shortest = INT_MAX;
	float sum = 0;
	float mean = 0;
	for (int i = 0; i < profileResults->size(); i++) {
		float temp = profileResults->at(i);
		if (temp > longest) {
			longest = temp;
		}
		if (temp < shortest) {
			shortest = temp;
		}
		sum += temp;
	}
	mean = sum / profileResults->size();
	std::cout << "=========================================\n";
	std::cout << "Profile Information: \n";
	std::cout << "     Frames Profiled: " << framesProfiled << "\n";
	std::cout << "     Max Frames: " << maxFramesProfile << "\n";
	std::cout << "     Total Frames Rendered: " << totalFrames << "\n";
	std::cout << "     % of Reserved Frames Profiled: " << framesProfiled / (float)maxFramesProfile * 100 << "\n";
	std::cout << "     % of Rendered Frames Profiled: " << framesProfiled / (float)totalFrames*100 << "\n";
	printf_s("     Longest Frame:    %10.4f [ms], %7.1f [FPS]\n", longest, 1000.0 / longest);
	printf_s("     Shortest Frame:   %10.4f [ms], %7.1f [FPS]\n", shortest, 1000.0 / shortest);
	printf_s("     Mean Frame:       %10.4f [ms], %7.1f [FPS]\n", mean, 1000.0 / mean);
	std::cout << "=========================================\n";

}

unsigned int loadTexture(const char* path,
	unsigned int wrapS,
	unsigned int wrapT,
	unsigned int minFilter,
	unsigned int magFilter) {
	int width, height, channels;

	// load the image using soil
	unsigned char* data = SOIL_load_image(path, &width, &height, &channels, 0);


	// if 4 channels, use rgba rather than rgb
	unsigned int format = GL_RGB;
	if (channels == 4) {
		format = GL_RGBA;
	}

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set up parameter options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	// set up the texture data and generate the mip maps
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}


void updateProjView(glm::mat4* proj, glm::mat4* view, int width, int height) {
	*view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
	*proj = glm::perspective(glm::radians(camera.fov), (float)width / height, .01f, 1000.0f);

}

GLFWwindow* window;
unsigned int program;

void initRenderer(int width, int height) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(width, height, "Game", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initalize GLAD\n");
		exit(1);
	}



	glViewport(0, 0, width, height);


	// now load shaders
	program = compileShader("shaders/vertex0.shader", "shaders/fragment0.shader");

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouseCallBack);
	glfwSetMouseButtonCallback(window, mouseButtonCallBack);
	glfwSetScrollCallback(window, scrollCallBack);

	camera.baseSpeed = 10;


}



void framebuffer_size_callback(GLFWwindow* window, int width_, int height_) {

	glViewport(0, 0, width_, height_);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	userInput.A = glfwGetKey(window, GLFW_KEY_A);
	userInput.D = glfwGetKey(window, GLFW_KEY_D);
	userInput.W = glfwGetKey(window, GLFW_KEY_W);
	userInput.S = glfwGetKey(window, GLFW_KEY_S);
	userInput.space = glfwGetKey(window, GLFW_KEY_SPACE);
	userInput.leftShift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

	userInput.B = glfwGetKey(window, GLFW_KEY_B);


	
}

float lastFrame = 0;
float dt = 0;

void postRenderingSteps(bool log, GLFWwindow* window, 
	std::chrono::time_point<std::chrono::high_resolution_clock>* start,
	glm::mat4* proj, glm::mat4* view, int width, int height) {

	float currentFrame = glfwGetTime();
	dt = currentFrame - lastFrame;
	lastFrame = currentFrame;
	camera.speed = camera.baseSpeed * dt;

	glfwSwapBuffers(window);
	glfwPollEvents();

	camera.translate(userInput.A, userInput.D, userInput.space, userInput.leftShift, userInput.W, userInput.S);
	updateProjView(proj, view, width, height);


	if (userInput.B == 1) {
		shouldProfile = true;
	}

	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	
	float milis = (end - *start).count() / 1000000.0;
	float fps = 1000.0 / milis;

	// give option to save time from writing to the console
	if (log) {
		std::cout << "Total Time difference = " << milis << "[ms]" << " FPS: " << fps << "\n";

	}

	profileFrame(milis);
	checkErrors();

}

void cleanup() {
	logProfile();
	if (profileResults == NULL) {
		delete profileResults;
	}

}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		userInput.leftMouseButton = action;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		userInput.middleMouseButton = action;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		userInput.rightMouseButton = action;
	}

}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	camera.rotate(xpos, ypos, userInput.leftMouseButton);
}

void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
	camera.zoom(xoffset, yoffset);

}


char* loadShader(const char* filepath) {
	FILE* fragment = fopen(filepath, "r");
	int size = 100;
	int index = 0;
	char* stream = (char*)malloc(sizeof(char) * size);
	while (!feof(fragment)) {
		char in;
		fscanf(fragment, "%c", &in);
		stream[index] = in;
		index++;
		if (index == size) {
			stream = (char*)realloc(stream, size + size);
			size = size + size;
		}
	}

	stream[index - 1] = '\0';
	fclose(fragment);


	return stream;
}


void shaderBuildStatus(unsigned int shader, int result) {
	std::cout << "Result" << result << "\n";
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)(alloca(length * (sizeof(char))));
		glGetShaderInfoLog(shader, length, &length, message);
		std::cout << "Failed to compile shader--\n";
		std::cout << message << "\n";
		glDeleteShader(shader);
		std::exit(1);
		return;

	}
}

unsigned int compileShader(const char* vertex, const char* fragment) {
	char* fragmentShaderSource = loadShader(fragment);
	char* vertexShaderSource = loadShader(vertex);

	unsigned int vertexShader;
	unsigned int fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	int result;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	shaderBuildStatus(vertexShader, result);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	shaderBuildStatus(fragmentShader, result);

	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//// add geometry shader here
	//char* geometryShaderSource = loadShader("shaders/geometry.shader");
	//unsigned int geometryShader;
	//geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	//glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
	//glCompileShader(geometryShader);

	//glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &result);
	//shaderBuildStatus(geometryShader, result);
	//glAttachShader(*program, geometryShader);

	glLinkProgram(program);

	glUseProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	free(vertexShaderSource);
	free(fragmentShaderSource);

	return program;
}
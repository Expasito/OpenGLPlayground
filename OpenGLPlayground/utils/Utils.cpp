#include "Utils.h"



Camera camera(0, 0, -10);

struct UserInput {
	int leftArrow = 0;
	int rightArrow = 0;
	int upArrow = 0;
	int downArrow = 0;

	int W = 0, A = 0, S = 0, D = 0;
	int leftShift = 0;
	int leftControl = 0;
	int space = 0;
	
	int leftMouseButton = 0;
	int rightMouseButton = 0;
	int middleMouseButton = 0;
} userInput;


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
	1,1,-1,
};

size_t verticesSize = sizeof(vertices);


void checkErrors() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "ERROR!\n";
		std::cout << err << "\n";
		exit(1);
	}
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
	*proj = glm::perspective(glm::radians(camera.fov), (float)width / height, .01f, 1000.0f);
	*view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, glm::vec3(0, 1, 0));
}

GLFWwindow* window;
unsigned int program;

void initRenderer(int width, int height) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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


	
}

float lastFrame = 0;
float dt = 0;

void postRenderingSteps(GLFWwindow* window, 
	std::chrono::time_point<std::chrono::high_resolution_clock>* start,

	glm::mat4* proj, glm::mat4* view, int width, int height) {

	float currentFrame = glfwGetTime();
	dt = currentFrame - lastFrame;
	lastFrame = currentFrame;
	camera.speed = camera.baseSpeed * dt;

	glfwSwapBuffers(window);
	glfwPollEvents();
	checkErrors();
	updateProjView(proj, view, width, height);

	camera.translate(userInput.A, userInput.D, userInput.space, userInput.leftShift, userInput.W, userInput.S);

	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	
	float milis = (end - *start).count() / 1000000.0;
	float fps = 1000.0 / milis;
	std::cout << "Total Time difference = " << milis << "[ms]" << " FPS: " << fps << "\n";

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
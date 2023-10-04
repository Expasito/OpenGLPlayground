#include "Utils.h"


void checkErrors() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "ERROR!\n";
		std::cout << err << "\n";
		exit(1);
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width_, int height_) {

	glViewport(0, 0, width_, height_);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Render::left = glfwGetKey(window, GLFW_KEY_A);
	//Render::right = glfwGetKey(window, GLFW_KEY_D);
	//Render::forward = glfwGetKey(window, GLFW_KEY_W);
	//Render::backward = glfwGetKey(window, GLFW_KEY_S);
	//Render::up = glfwGetKey(window, GLFW_KEY_SPACE);
	//Render::down = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);



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
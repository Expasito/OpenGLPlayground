#include "Render.h"
#include <fstream>
#include <stdlib.h>



GLFWwindow* Render::window = NULL;
unsigned int Render::renderShader = -1;
unsigned int Render::screenShader = -1;
unsigned int Render::debugShader = -1;
unsigned int Render::shadowShader = -1;
unsigned int Render::depthShader = -1;
unsigned int Render::multiShader = -1;

int Render::width = 0;
int Render::height = 0;

std::vector<Render::Object*> Render::objects;
glm::mat4 Render::model = glm::mat4(1.0f);
glm::mat4 Render::view = glm::mat4(1.0f);
glm::mat4 Render::projection = glm::mat4(1.0f);

unsigned long long Render::instanceCounter = 0;

float Render::dt;
float Render::lastFrame = 0;
bool Render::keepWindow = true;
unsigned int Render::VAO = -1;
Camera Render::camera(0, 0, -10);

bool Render::left = false, Render::right = false, Render::down = false, Render::up = false, Render::forward = false, Render::backward = false;
bool Render::leftMouseButton = false, Render::rightMouseButton = false, Render::middleMouseButton = false;

// This holds information about our 
Render::Debug Render::debug = { -1,-1,-1,10,.05,NULL };

void Render::initDebugScreen() {



	glGenBuffers(1, &Render::debug.vertices);
	glBindBuffer(GL_ARRAY_BUFFER, Render::debug.vertices);
	float width = Render::debug.width;
	float data[] = {
		// x , y , z , u , v 
		-width / 2,-1,0, 0,0,
		-width / 2,1,0,  0,1,
		width / 2,-1,0,  1,0,

		-width / 2,1,0, 0,1,
		width / 2,1,0,  1,1,
		width / 2,-1,0,  1,0

	};

	glBindBuffer(GL_ARRAY_BUFFER, Render::debug.vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);


	glGenBuffers(1, &Render::debug.indices);
	float* indexes = new float[Render::debug.bars];
	for (int i = 0; i < Render::debug.bars; i++) {
		indexes[i] = -1 + width / 2 * (i + 1);
	}
	glBindBuffer(GL_ARRAY_BUFFER, Render::debug.indices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*Render::debug.bars, indexes, GL_STATIC_DRAW);

	glGenBuffers(1, &Render::debug.heights);
	Render::debug.heightsData = new float[Render::debug.bars];
	for (int i = 0; i < Render::debug.bars; i++) {
		Render::debug.heightsData[i] = 0;
	}
	glBindBuffer(GL_ARRAY_BUFFER, Render::debug.heights);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*Render::debug.bars, Render::debug.heightsData, GL_STATIC_DRAW);

	delete[] indexes;
}


// milis is the most recent time taken to draw the frame
void Render::drawDebug(float milis) {

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH);
	glUseProgram(Render::debugShader);


	glBindBuffer(GL_ARRAY_BUFFER, Render::debug.heights);
	for (int i = 0; i < Render::debug.bars - 1; i++) {
		Render::debug.heightsData[i] = Render::debug.heightsData[i + 1];

	}
	// these are the ranges of the milis taken for a frame
	float max = 50;
	float min = 0;
	float scaled = (milis - max) / max;
	Render::debug.heightsData[Render::debug.bars - 1] = scaled - 1;
	//Render::debug.heightsData[Render::debug.bars - 1] = 0;

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 0, sizeof(float) * (Render::debug.bars), Render::debug.heightsData);

	

	glBindBuffer(GL_ARRAY_BUFFER, Render::debug.vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 0);

	// load in indexes

	glBindBuffer(GL_ARRAY_BUFFER, Render::debug.indices);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
	glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, Render::debug.heights);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
	glVertexAttribDivisor(2, 1);


	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, Render::debug.bars);
}





Render::Model::Model(std::vector<vertex> verts, std::vector<unsigned int> inds) {
	vertices = verts;
	indices = inds;
	glGenBuffers(1, &positions);
	glBindBuffer(GL_ARRAY_BUFFER, positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}



void Render::addModel(const char* filepath, std::string name, int hashtablesize, int dynamicarraysize) {
	Render::Model m = loadModel(filepath);
	for (int i = 0; i < m.indices.size(); i++) {
		//std::cout << m.indices.at(i) << "\n";
	}
	objects.push_back(new Render::Object(name,filepath, m.positions, m.ebo, m.indices.size()-3, new FillerArray(hashtablesize, dynamicarraysize)));
}



// the returned value is the unique id for the number
long Render::addInstance(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, glm::vec3 color) {
	for (Render::Object* o : Render::objects) {
		if (o->name.compare(name)==0) {
			return o->insts->add(pos, rot, scal, color);

		}
		
	}
}

void Render::editInstance(std::string name, unsigned long long id, glm::vec3 pos, glm::bvec3 rot, glm::vec3 scal, glm::vec3 color) {

}


void Render::prepBuffers() {
	// create and bind the vao
	glGenVertexArrays(1, &Render::VAO);
	glBindVertexArray(Render::VAO);


	// set the divisors for each vertex attribute
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);


	// enable each vertex attribute
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);


}


//TODO: rewrite so we set the buffer data and vertex attributes at hte same time for speed to not bind buffers as often
void Render::draw() {

	float milis;

	for (Render::Object* o : Render::objects) {
		int elements = o->insts->da->elements;
		//std::cout << elements << "\n";
		// make sure there are elements and not a null pointer
		if (elements <= 0 || o == NULL) {
			continue;
		}

		//std::cout << "Objectname: " << o->name << "\n";

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		

		// bind the position buffer and send the vertices to gpu
		glBindBuffer(GL_ARRAY_BUFFER, o->positions);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(glm::vec3) * 1));
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(glm::vec3) * 2));
		glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(glm::vec3)*3));


		// load in the buffer of all instances
		glBindBuffer(GL_ARRAY_BUFFER, o->insts->buffer);

		
		// this is translation
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int)));
		
		// this is rotation
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(glm::vec3)+sizeof(int)));
		
		// this is for scalation
		 glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 2 * sizeof(glm::vec3)));
		 
		 // this is for color
		 glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 3 * sizeof(glm::vec3)));
		 



		 // give the ebo to the gpu
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->ebo);
		


		// finally, send the draw command to the gpu
		glDrawElementsInstanced(GL_TRIANGLES, o->eboSize, GL_UNSIGNED_INT, 0, elements);





	}
}

void Render::renderAll() {
	float milis;
	// this is the start time 
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	Render::keepWindow = !glfwWindowShouldClose(Render::window);
	float currentFrame = glfwGetTime();
	Render::dt = currentFrame-Render::lastFrame;
	Render::lastFrame = currentFrame;
	Render::camera.speed = Render::camera.baseSpeed * Render::dt;
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "model"), 1, GL_FALSE, glm::value_ptr(Render::model));
	glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "view"), 1, GL_FALSE, glm::value_ptr(Render::view));
	glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "projection"), 1, GL_FALSE, glm::value_ptr(Render::projection));
	glUniform3fv(glGetUniformLocation(Render::renderShader, "camPos"), 1, glm::value_ptr(Render::camera.cameraPos));
	glUniform3fv(glGetUniformLocation(Render::renderShader, "camFront"), 1, glm::value_ptr(Render::camera.cameraFront));

	Render::camera.translate(Render::left, Render::right, Render::up, Render::down, Render::forward, Render::backward);
	Render::view = glm::lookAt(Render::camera.cameraPos, Render::camera.cameraPos + Render::camera.cameraFront, Render::camera.cameraUp);
	Render::projection = glm::perspective(glm::radians(Render::camera.fov), (float)(800.0 / 800.0), .01f, 10000.0f);



	Render::draw();



	glfwSwapBuffers(Render::window);


	
	glfwPollEvents();


}


char* Render::loadShader(const char* filepath) {
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

void Render::shaderBuildStatus(unsigned int shader, int result) {
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

void framebuffer_size_callback(GLFWwindow* window, int width_, int height_) {
	Render::width = width_;
	Render::height = height_;
	glViewport(0, 0, width_, height_);
}
bool wireframe = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	Render::left = glfwGetKey(window, GLFW_KEY_A);
	Render::right = glfwGetKey(window, GLFW_KEY_D);
	Render::forward = glfwGetKey(window, GLFW_KEY_W);
	Render::backward = glfwGetKey(window, GLFW_KEY_S);
	Render::up = glfwGetKey(window, GLFW_KEY_SPACE);
	Render::down = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);


	

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		wireframe = !wireframe;
		
	}
	if (wireframe == true) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


}

int Render::getKey(int key) {
	return glfwGetKey(window, key);
}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		Render::leftMouseButton = action;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		Render::middleMouseButton = action;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		Render::rightMouseButton = action;
	}

}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	Render::camera.rotate(xpos, ypos, Render::leftMouseButton);
}

void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
	Render::camera.zoom(xoffset, yoffset);

}

void Render::init(int width_, int height_, bool fullScreen) {
	width = width_;
	height = height_;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (fullScreen) {
		Render::window = glfwCreateWindow(width, height, "Game", glfwGetPrimaryMonitor(), NULL);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	}
	else {
		Render::window = glfwCreateWindow(width, height, "Game", NULL, NULL);

	}
	if (Render::window == NULL) {
		printf("Failed to create window\n");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(Render::window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initalize GLAD\n");
		return;
	}

	
	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(Render::window, framebuffer_size_callback);

	glfwSetKeyCallback(Render::window, key_callback);
	glfwSetCursorPosCallback(Render::window, mouseCallBack);
	glfwSetMouseButtonCallback(Render::window, mouseButtonCallBack);
	glfwSetScrollCallback(Render::window, scrollCallBack);

	compileShader("shaders/fragment.shader", "shaders/vertex.shader", &Render::renderShader);
	compileShader("shaders/debugFragment.shader", "shaders/debugVertex.shader", &Render::debugShader);
	compileShader("shaders/screenFragment.shader", "shaders/screenVertex.shader", &Render::screenShader);
	compileShader("shaders/shadowFragment.shader", "shaders/shadowVertex.shader", &Render::shadowShader);
	compileShader("shaders/depthFragment.shader", "shaders/depthVertex.shader", &Render::depthShader);
	compileShader("shaders/multiDrawFragment.shader", "shaders/multiDrawVertex.shader", &Render::multiShader);
	//glUseProgram(Render::screenShader);
	
	Render::prepBuffers();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

}

void Render::exit() {
	glfwTerminate();
	//for (object o : Render::objects) {
	//	//free(o.insts->table);
	//}

	glfwDestroyWindow(Render::window);
}

void Render::compileShader(const char* fragment, const char* vertex, unsigned int* program) {
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

	*program = glCreateProgram();
	glAttachShader(*program, vertexShader);
	glAttachShader(*program, fragmentShader);

	//// add geometry shader here
	//char* geometryShaderSource = loadShader("shaders/geometry.shader");
	//unsigned int geometryShader;
	//geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	//glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
	//glCompileShader(geometryShader);

	//glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &result);
	//shaderBuildStatus(geometryShader, result);
	//glAttachShader(*program, geometryShader);

	glLinkProgram(*program);

	glUseProgram(*program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	free(vertexShaderSource);
	free(fragmentShaderSource);
}


std::vector<std::string> Render::strsplit(std::string str, char dem) {
	std::vector<std::string> strs;
	//std::vector<std::string>* strs = new std::vector<std::string>();
	while (str.size() != 0) {
		//std::cout << str << "\n";

		size_t pos = str.find(dem);
		//std::cout << "pos: " << pos << "\n";
		// dem does exist
		if (pos != std::string::npos) {
			std::string temp = str.substr(0, pos);
			strs.push_back(temp);
			str.erase(0, pos + 1);
		}
		else {
			std::string temp = str;
			strs.push_back(temp);
			break;
		}
	}
	return strs;
}




Render::Model Render::loadModel(const char* path) {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<unsigned int> indices;

	std::vector<vertex> outputData;

	// this keeps track of which index we are on
	int i = 0;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Failed to open\n");
	}
	while (!feof(file)) {

		char stuff[1000];
		fgets(stuff, 1000, file);
		std::string buff = stuff;
		//std::cout << buff << "\n";
		// is vertex data
		if (buff[0] == 'v' && buff[1] == ' ') {
			std::vector<std::string> sub = strsplit(buff.substr(2, buff.size() - 1), ' ');
			float f1 = stof(sub[0]);
			float f2 = stof(sub[1]);
			float f3 = stof(sub[2]);
			float f4, f5, f6;
			// has color data
			if (sub.size() == 6) {
				f4 = stof(sub[3]);
				f5 = stof(sub[4]);
				f6 = stof(sub[5]);
			}
			else {
				f4 = 1;
				f5 = 1;
				f6 = 1;
			}
			
			vertices.push_back({f1,f2,f3});
			colors.push_back({f4,f5,f6});
		}

		// These are vertex normals
		if (buff[0] == 'v' && buff[1] == 'n') {
			std::vector<std::string> sub = strsplit(buff.substr(3, buff.size() - 1), ' ');
			float f1 = stof(sub[0]);
			float f2 = stof(sub[1]);
			float f3 = stof(sub[2]);

			normals.push_back({ f1,f2,f3 });
		}

		// These are vertex texture coordinates
		if (buff[0] == 'v' && buff[1] == 't') {
			std::vector<std::string> sub = strsplit(buff.substr(3, buff.size() - 1), ' ');
			float f1 = stof(sub[0]);
			float f2 = stof(sub[1]);

			textures.push_back({ f1,f2});

		}

		// is indice data
		if (buff[0] == 'f' && buff[1] == ' ') {
			// 2 steps to this
			// 1: get the index data
			// 2: put the vertex data in the correct order in the output buffer
			std::vector<std::string> sub = strsplit(buff.substr(2, buff.size() - 1), ' ');

			// process first vertex
			
			float i1, i2, i3;
			std::vector<std::string> subi1 = strsplit(sub[0], '/');
			vertex v = {
				// position data first
				vertices.at(stoi(subi1[0])-1),
				// color
				colors.at(stoi(subi1[0])-1),
				// normal
				normals.at(stoi(subi1[2]) - 1),
				//texture coord
				textures.at(stoi(subi1[1]) - 1)
				
			};
			outputData.push_back(v);

			std::vector<std::string> subi2 = strsplit(sub[1], '/');
			vertex v2 = {
				// position data first
				vertices.at(stoi(subi2[0]) - 1),
				// color
				colors.at(stoi(subi2[0]) - 1),
				// normal
				normals.at(stoi(subi2[2]) - 1),
				//texture coord
				textures.at(stoi(subi2[1]) - 1)

			};
			outputData.push_back(v2);

			std::vector<std::string> subi3 = strsplit(sub[2], '/');
			vertex v3 = {
				// position data first
				vertices.at(stoi(subi3[0]) - 1),
				// color
				colors.at(stoi(subi3[0]) - 1),
				// normal
				normals.at(stoi(subi3[2]) - 1),
				//texture coord
				textures.at(stoi(subi3[1]) - 1)

			};
			outputData.push_back(v3);
			indices.push_back(i++);
			indices.push_back(i++);
			indices.push_back(i++);

		}

		char type;

	}
	fclose(file);
	Render::Model ret(outputData, indices);
	return ret;
}

void Render::loadSave(const char* path) {
	std::ifstream file(path);
	std::string res;
	if (file.is_open()) {
		std::string name;
		std::string path_;
		while (file.good()) {
			std::getline(file, res);
			const char* value = res.c_str();
			

			// check if the string is detailed about a descriptor
			if (value[0] == ':') {
				// First, we have the name of the object
				// Then, the filepath
				// Finally, the hashtable and dynamic array sizes
				name = res.substr(1, res.length());
				std::cout << "Name is: " << name << "\n";
				std::getline(file, res);
				path_ = res.substr(1, res.length());
				std::cout << "Path is: " << path_ << "\n";
				std::getline(file, res);
				value = res.c_str();
				int hashSize, dynamSize;
				sscanf(value, "%d,%d", &hashSize, &dynamSize);
				std::cout << "Table Sizes: " << hashSize << " " << dynamSize << "\n";
				Render::addModel(path_.c_str(), name, hashSize,dynamSize);
				

				// now prepare next line
				std::getline(file, res);
				value = res.c_str();
			}
			// these are the floats for the vertex data
			float t0, t1, t2, r0, r1, r2, s0, s1, s2, c0, c1, c2;
			sscanf(value, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
				&t0, &t1, &t2, &r0, &r1, &r2, &s0, &s1, &s2, &c0, &c1, &c2);
			Render::addInstance(name, {t0,t1,t2}, {r0,r1,r2}, {s0,s1,s2}, {c0,c1,c2});


		}
	}
	file.close();

}

void Render::createSave(const char* path) {
	std::ofstream file(path,std::ios::out);
	for (Render::Object* o : Render::objects) {
		std::cout << o->path << "\n";
		file << ":";
		file << o->name << "\n";
		file << ":";
		file << std::string(o->path) << "\n";
		file << o->insts->ht->size << "," << o->insts->da->size << "\n";
		// now iterate through all objects in the scene
		for (int i = 0; i < o->insts->da->elements; i++) {
			FillerArray::Element elem = o->insts->da->get(i);
			file << elem.translations.x << "," << elem.translations.y << "," << elem.translations.z << ","
				<< elem.rotations.x << "," << elem.rotations.y << "," << elem.rotations.z << ","
				<< elem.scalations.x << "," << elem.scalations.x << "," << elem.scalations.x << ","
				<< elem.colors.x << "," << elem.colors.x << "," << elem.colors.x << "\n";
		}

	}
	file.close();
}

//std::ostream operator<<(std::ofstream& stream_, FillerArray::Element elem) {
//	stream_ << elem.translations.x << "," << elem.translations.y << "," << elem.translations.z << ","
//		<< elem.rotations.x << "," << elem.rotations.y << "," << elem.rotations.z << ","
//		<< elem.scalations.x << "," << elem.scalations.x << "," << elem.scalations.x << ","
//		<< elem.colors.x << "," << elem.colors.x << "," << elem.colors.x << "\n";
//}
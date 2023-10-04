#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


unsigned int compileShader(const char* vertex, const char* fragment);

void checkErrors();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
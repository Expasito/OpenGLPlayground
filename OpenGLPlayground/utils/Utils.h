#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <glad/glad.h>


unsigned int compileShader(const char* vertex, const char* fragment);

void checkErrors();
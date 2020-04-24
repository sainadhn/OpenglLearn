#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



void GLClearError();
bool GLLogCall(const char* function, const char* file, int line); 

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray* va, const IndexBuffer* ib, const Shader& shader) const;
};


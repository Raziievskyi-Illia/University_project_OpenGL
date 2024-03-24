#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "EBO.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include <stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "Camera.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define M_PI 3.14

std::vector<GLuint> generateSphereIndices(GLuint sectors, GLuint stacks) {
	std::vector<GLuint> indices;

	for (GLuint i = 0; i < stacks; ++i) {
		for (GLuint j = 0; j < sectors; ++j) {
			GLuint currentIdx = i * (sectors + 1) + j;
			GLuint nextIdx = currentIdx + sectors + 1;

			indices.push_back(currentIdx);
			indices.push_back(nextIdx);
			indices.push_back(currentIdx + 1);

			indices.push_back(nextIdx);
			indices.push_back(nextIdx + 1);
			indices.push_back(currentIdx + 1);

		}
	}

	return indices;
}


std::vector<GLfloat> generateSphereVertices(GLfloat radius, GLuint sectors, GLuint stacks) {
	std::vector<GLfloat> vertices;

	for (GLuint i = 0; i <= stacks; ++i) {
		GLfloat stackAngle = M_PI / 2.0f - i * M_PI / stacks; 
		GLfloat xy = radius * cosf(stackAngle);
		GLfloat z = radius * sinf(stackAngle);

		for (GLuint j = 0; j <= sectors; ++j) {
			GLfloat sectorAngle = 2.0f * M_PI * j / sectors; 
			GLfloat x = xy * cosf(sectorAngle);
			GLfloat y = xy * sinf(sectorAngle);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			/*GLfloat texCoordX = (x + 1.0f) / 2.0f;  
			GLfloat texCoordY = (y + 1.0f) / 2.0f;

			vertices.push_back(texCoordX);
			vertices.push_back(texCoordY);*/
		}
	}

	return vertices;
}

void loadCoordinatesFromFile(const std::string& filename , std::vector<GLfloat> &coordinates) {
	std::ifstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}
	GLfloat x , y ,z;
	while (file >> x >> y >> z) {
		coordinates.push_back(x);
		coordinates.push_back(y);
		coordinates.push_back(z);
		coordinates.push_back(0.0f);
		coordinates.push_back(0.0f);
		coordinates.push_back(0.0f);
		//GLfloat texCoordX = (x + 1.0f) / 2.0f;  
		//GLfloat texCoordY = (y + 1.0f) / 2.0f;

		//coordinates.push_back(texCoordX);
		//coordinates.push_back(texCoordY);
	}

	file.close();
}

void loadIndicesFromFile(const std::string& filename , std::vector<GLuint> &indices) {
	std::ifstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return ;
	}

	GLuint value;
	while (file >> value) {
		indices.push_back(value);
	}

	file.close();
}

float v = 0.f; 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		// Обработка нажатия клавиши
		if (key == GLFW_KEY_ESCAPE) {
			v += 0.1f;			 
		}
		else if (key == GLFW_KEY_BACKSPACE)
		{
			v -= 0.1f;
		}
		else if (key == GLFW_KEY_1)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (key == GLFW_KEY_2)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (key == GLFW_KEY_3)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
	}
}

	
int main()
{
	glfwInit();
	const int width = 1600; 
	const int height = 1024;


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//std::vector<GLfloat> vertices;
	//std::vector<GLuint> indices;
	std::vector<GLfloat> vertices = generateSphereVertices(1 , 30 ,30);
	//loadCoordinatesFromFile("vertices.txt", vertices);
	std::vector<GLuint> indices = generateSphereIndices(30,30); 
	//loadIndicesFromFile("indices.txt" , indices);


	GLFWwindow* window = glfwCreateWindow(width, height, "window", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices);
	EBO EBO1(indices);

	
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));   
	//VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));                  
	
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	GLuint s = glGetUniformLocation(shaderProgram.ID, "scale"); 
	glfwSetKeyCallback(window, key_callback);


	//int widthIMG, heightIMG, numColCh; 
	//stbi_set_flip_vertically_on_load(true);

	//unsigned char* bytes = stbi_load("brick.jpg", &widthIMG, &heightIMG, &numColCh,0);

	//GLuint texture; 
	//glGenTextures(1, &texture);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthIMG, heightIMG, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	//glGenerateMipmap(GL_TEXTURE_2D);

	//stbi_image_free(bytes);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//GLuint tex0Uni=glGetUniformLocation(shaderProgram.ID,"tex0");
	//shaderProgram.Activate();
	//glUniform1i(tex0Uni, 0);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	glEnable(GL_DEPTH_TEST);
	
	glPointSize(5.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.00f,1.00f ,1.0f , 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
	
		glUniform1f(s, v); 
		//glBindTexture(GL_TEXTURE_2D, texture);
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//glDeleteTextures(1, &texture);
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

}
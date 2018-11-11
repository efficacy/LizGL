#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <iostream>

#include "shader.h"

GLFWwindow *setup(const int w, const int h) {
	if (!glfwInit()) exit(EXIT_FAILURE);

	GLFWwindow *window = glfwCreateWindow(w, h, "glfw", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; // enable modern OpenGL support
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		printf("glew init error\n%s\n", glewGetErrorString(glewError));
		exit(EXIT_FAILURE);
	}

	// Get OpenGL Version String and GLSL Version string
	const GLubyte * opengl_version = glGetString(GL_VERSION);
	printf("Open GL Graphics Driver version: %s\n", opengl_version);
	const GLubyte * glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("GLSL Version: %s\n", glsl_version);

	if (!GLEW_VERSION_2_1) {
		printf("Modern OpenGL not supported!\n");
		 exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3); // default to 3px lines

	return window;
}

int teardown(GLFWwindow *window) {
	//Destroy window and terminate glfw
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

struct shape {
	const char* name;
	GLfloat vertices[];
};

int main() {
	GLFWwindow *window = setup(640, 480);

	//	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");
	GLuint shaderProgram = initShader("vert_simple.glsl", "frag_simple.glsl");
	GLuint positionID = glGetAttribLocation(shaderProgram, "position");
	GLuint colorID = glGetAttribLocation(shaderProgram, "color");
	glUseProgram(0);

	GLfloat shape[] = {
		-0.5f,   0.5f,  0.0f,  1.0f, 0.0f, 0.0f, // Left 
		-0.5f,  -0.5f,  0.0f,  1.0f, 0.0f, 0.0f, // Right
		0.0f,   0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // Top 
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);  // set attribute index of the position attribute to 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same, note that the stride is 6*sizeof(GLloat)			

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		//printf("window reports width %d and heught %d\n", width, height);

		glViewport(0, 0, width, height); //Set Viewport to whole window, even if resized
		glClearColor(1, 1, 1, 1); // background to opaque white
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//put drawing code in here
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		//Draw Triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Unbind Vertex Array Object and Shader
		glBindVertexArray(0);
		glUseProgram(0);


		// leave this at the end
		glfwSwapBuffers(window);                //<-- SWAP BUFFERS
		glfwPollEvents();                       //<-- LISTEN FOR WINDOW EVENTS
	}

	return teardown(window);
}
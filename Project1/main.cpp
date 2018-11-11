#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <iostream>

int main() {

	if (!glfwInit()) exit(EXIT_FAILURE);

	const int w = 640; const int h = 640;
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

	while (!glfwWindowShouldClose(window)) {
		glViewport(0, 0, w, h); //Set Viewport to whole window
		glClearColor(1, 1, 1, 1); // background to opaque white
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//put drawing code in here

		glfwSwapBuffers(window);                //<-- SWAP BUFFERS
		glfwPollEvents();                       //<-- LISTEN FOR WINDOW EVENTS
	}

	//Destroy window and terminate glfw
	glfwDestroyWindow(window);
	glfwTerminate();
	printf("goodbye window\n");

	return 0;
}
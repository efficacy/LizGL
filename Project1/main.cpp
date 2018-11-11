#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <iostream>

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

	return window;
}

int teardown(GLFWwindow *window) {
	//Destroy window and terminate glfw
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

int main() {
	GLFWwindow *window = setup(640, 480);

	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		//printf("window reports width %d and heught %d\n", width, height);

		glViewport(0, 0, width, height); //Set Viewport to whole window, even if resized
		glClearColor(1, 1, 1, 1); // background to opaque white
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//put drawing code in here

		glfwSwapBuffers(window);                //<-- SWAP BUFFERS
		glfwPollEvents();                       //<-- LISTEN FOR WINDOW EVENTS
	}

	return teardown(window);
}
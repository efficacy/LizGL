#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_interpolation.hpp>

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

struct Position {
	GLfloat x;
	GLfloat y;
};

struct Colour {
	GLfloat r;
	GLfloat g;
	GLfloat b;
};

struct Vertex {
	Position pos;
	Colour col;
};

#define RED {1.0f, 0.0f, 0.0f}

int main() {
	GLFWwindow *window = setup(640, 480);

	printf("sizeof(Poition)=%d, 3 x sizeof(GLfloat)=%d\n", sizeof(Position), (3 * sizeof(GLfloat)));

	//	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");
	GLuint shaderProgram = initShader("vert_simple.glsl", "frag_simple.glsl");
	GLuint positionID = glGetAttribLocation(shaderProgram, "position");
	GLuint colorID = glGetAttribLocation(shaderProgram, "color");
	GLuint txID = glGetUniformLocation(shaderProgram, "transform");

	glUseProgram(0);

	Vertex shape[] = {
		{{-0.5f,  0.5f}, RED }, // Left
		{{-0.5f, -0.5f}, RED}, // Right
		{{ 0.0f,  0.0f}, RED}, // Top
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);

	glEnableVertexAttribArray(positionID);  // set attribute index of the position attribute to 0 in the vertex shader
	glVertexAttribPointer(positionID, sizeof(Position)/ sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(colorID, sizeof(Colour) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Position));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const glm::vec3 move = glm::vec3(0.5,-0.5f, 0.0f);
	const int steps = 5000;
	int step = 0;
	int direction = 1;

	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		//printf("window reports width %d and heught %d\n", width, height);

		glViewport(0, 0, width, height); //Set Viewport to whole window, even if resized
		glClearColor(1, 1, 1, 1); // background to opaque white
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// example frame-based animation. Could easily look up rather than calculate this stuff
		if (step > steps || step < 0) {
			direction = -direction;
		}
		step += direction;

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		float dist = step / (float)steps;
//		printf("step=%d, dist=%f\n", step, dist);
		glm::vec3 part = move * dist;
		//set tranlation matrix in shader to move 0.5 right
		glm::mat4 tansform = glm::translate(glm::mat4(), part);
		glUniformMatrix4fv(txID, 1, GL_FALSE, glm::value_ptr(tansform));

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
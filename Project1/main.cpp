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

struct P {
	GLfloat x;
	GLfloat y;

	P() { x = 0; y = 0; }
	P(GLfloat x, GLfloat y) {
		P::x = x;
		P::y = y;
	}
};

struct Colour {
	GLfloat r;
	GLfloat g;
	GLfloat b;

	Colour() { r = 0.5f; g = 0.5f; b = 0.5f; }
	Colour(GLfloat r, GLfloat g, GLfloat b) {
		Colour::r = r;
		Colour::g = g;
		Colour::b = b;
	}
};

struct Vertex {
	P pos;
	Colour col;

	Vertex() {}
	Vertex(P p, Colour c) {
		pos = p;
		col = c;
	}
};

struct Shape {
	glm::mat4 transform = glm::mat4();
	GLuint VBO, VAO;

	virtual int nPoints() = 0;
	virtual Vertex* getPoints() = 0;
	int size() { return nPoints() * sizeof(Vertex); }
};

struct Triangle : Shape {
	Vertex points[3];
	virtual int nPoints() { return 3; }
	virtual Vertex* getPoints() { return points;  }

	Triangle(Colour colour, P p1, P p2, P p3) {
		points[0] = Vertex(p1, colour);
		points[1] = Vertex(p2, colour);
		points[2] = Vertex(p3, colour);
	}
};

struct Quadrangle : Shape {
	Vertex points[6];
	virtual int nPoints() { return 6; }
	virtual Vertex* getPoints() { return points; }

	Quadrangle(Colour colour, P p1, P p2, P p3, P p4) {
		points[0] = Vertex(p1, colour);
		points[1] = Vertex(p2, colour);
		points[2] = Vertex(p3, colour);

		points[3] = Vertex(p1, colour);
		points[4] = Vertex(p3, colour);
		points[5] = Vertex(p4, colour);
	}
};

#define RED Colour(1.0f, 0.0f, 0.0f)
#define GREEN Colour(0.0f, 1.0f, 0.0f)
#define BLUE Colour(0.0f, 0.0f, 1.0f)

#define YELLOW Colour(1.0f, 1.0f, 0.0f)
#define CYAN Colour(0.0f, 1.0f, 1.0f)
#define MAGENTA Colour(1.0f, 0.0f, 1.0f)

int main() {
	GLFWwindow *window = setup(640, 640);

	//	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");
	GLuint shaderProgram = initShader("vert_simple.glsl", "frag_simple.glsl");
	GLuint positionID = glGetAttribLocation(shaderProgram, "position");
	GLuint colorID = glGetAttribLocation(shaderProgram, "color");
	GLuint txID = glGetUniformLocation(shaderProgram, "transform");

	glUseProgram(0);

	Triangle largeTriangleRed = Triangle(RED, P(-0.5f, 0.5f), P(-0.5f, -0.5f), P(0.0f, 0.0f));
	Triangle largeTriangleBlue = Triangle(BLUE, P(-0.5f, 0.5f), P(0.0f, 0.0f), P(0.5f, 0.5f));
	Triangle medTriangle = Triangle(MAGENTA, P(0.0f, -0.5f), P(0.5f, -0.5f), P(0.5f, 0.0f));
	Triangle smallTriangle1 = Triangle(YELLOW, P(0.25f, 0.25f), P(0.5f, 0.0f), P(0.5f, 0.5f));
	Triangle smallTriangle2 = Triangle(CYAN, P(-0.25f, -0.25f), P(0.25f, -0.25f), P(0.0f, 0.0f));
	Quadrangle square = Quadrangle(GREEN, P(0.0f, 0.0f), P(0.25f, -0.25), P(0.5f, 0.0f), P(0.25f, 0.25f));
	//Triangle square1 = Triangle(GREEN, P(0.0f, 0.0f), P(0.25f, -0.25f), P(0.5f, 0.0f));
	//Triangle square2 = Triangle(GREEN, P(0.0f, 0.0f), P(0.5f, 0.0f), P(0.25f, 0.25f));


	Shape* shapes[] = { &largeTriangleRed, &largeTriangleBlue, &medTriangle, &smallTriangle1, &smallTriangle2, &square };

	for (int si = 0; si < sizeof(shapes) / sizeof(Shape*); ++si) {
		Shape* s = shapes[si];

		glGenVertexArrays(1, &(s->VAO));
		glBindVertexArray(s->VAO);

		glGenBuffers(1, &(s->VBO));

		glBindBuffer(GL_ARRAY_BUFFER, s->VBO);
		glBufferData(GL_ARRAY_BUFFER, s->size(), s->getPoints(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(positionID);  // set attribute index of the P attribute to 0 in the vertex shader
		glVertexAttribPointer(positionID, sizeof(P) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

		glEnableVertexAttribArray(colorID);
		glVertexAttribPointer(colorID, sizeof(Colour) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(P));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const glm::vec3 move = glm::vec3(0.5,-0.5f, 0.0f); // example movement vector
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

		for (int si = 0; si < sizeof(shapes) / sizeof(Shape*); ++si) {
			glBindVertexArray(shapes[si]->VAO);

			float dist = step / (float)steps;
			// printf("step=%d, dist=%f\n", step, dist);
			glm::vec3 part = move * dist;
			//set tranlation matrix in shader to move 0.5 right
			glm::mat4 tansform = glm::translate(glm::mat4(), part);
			glUniformMatrix4fv(txID, 1, GL_FALSE, glm::value_ptr(tansform));

			//Draw Triangle
			glDrawArrays(GL_TRIANGLES, 0, shapes[si]->nPoints());

			//Unbind Vertex Array Object and Shader
			glBindVertexArray(0);
		}

		glUseProgram(0);

		// leave this at the end
		glfwSwapBuffers(window);                //<-- SWAP BUFFERS
		glfwPollEvents();                       //<-- LISTEN FOR WINDOW EVENTS
	}

	return teardown(window);
}
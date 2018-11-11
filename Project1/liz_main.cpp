// Code adapted from www.learnopengl.com, www.glfw.org

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

int xmain(void)
{
	//++++create a glfw window+++++++++++++++++++++++++++++++++++++++
	GLFWwindow* window;

	if (!glfwInit()) //Initialize the library
		return -1;

	window = glfwCreateWindow(640, 640, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);//Make the window's context current

	//++++Initialize GLEW to setup the OpenGL Function pointers+++++++
	glewExperimental = GL_TRUE;
	glewInit();

	//++++Define the viewport dimensions++++++++++++++++++++++++++++
	glViewport(0, 0, 640, 640);

	//++++++++++Set up vertex data (and buffer(s)) and attribute pointers+++++++++
	GLfloat largeTriangleRed[] = {
		-0.5f,   0.5f,  0.0f,  1.0f, 0.0f, 0.0f,// Left 
		-0.5f,  -0.5f,  0.0f,  1.0f, 0.0f, 0.0f, // Right
		 0.0f,   0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // Top 
	};

	GLfloat largeTriangleBlue[] = {
		-0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,// Left
		 0.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,// Right
		 0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,// Top 
	};

	GLfloat medTriangle[] = {
		 0.0f, -0.5f,  0.0f,  1.0f, 0.0f, 1.0f,// Left
		 0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 1.0f,// Right
		 0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,// Top 
	};

	GLfloat smallTriangle1[] = {
		 0.25f, 0.25f, 0.0f,  1.0f, 1.0f, 0.0f,// Left 
		 0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // Right
		 0.5f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f, // Top 
	};

	GLfloat smallTriangle2[] = {
		-0.25f, -0.25f, 0.0f,  0.0f, 1.0f, 1.0f,// Left 
		 0.25f, -0.25f, 0.0f,  0.0f, 1.0f, 1.0f, // Right
		 0.0f,   0.0f, 0.0f,  0.0f, 1.0f, 1.0f, // Top 
	};

	GLfloat square[] = {
		 0.0f,   0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Left 
		 0.25f, -0.25f, 0.0f,  0.0f, 1.0f, 0.0f, // Right
		 0.5f,   0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Top
		 0.0f,   0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Left 
		 0.5f,   0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Right
		 0.25f,   0.25f, 0.0f,  0.0f, 1.0f, 0.0f, // Top 
	};

	GLfloat parallelogram[] = {
		-0.5f,  -0.5f, 0.0f,  0.5f, 0.3f, 0.3f, // Left 
		 0.0f,  -0.5f, 0.0f,  0.5f, 0.3f, 0.3f, // Right
		 0.25f, -0.25f, 0.0f,  0.5f, 0.3f, 0.3f, // Top
		-0.5f, -0.5f, 0.0f,  0.5f, 0.3f, 0.3f, // Left 
		 0.25f, -0.25f, 0.0f,  0.5f, 0.3f, 0.3f, // Right
		-0.25f, -0.25f, 0.0f,  0.5f, 0.3f, 0.3f, // Top 
	};

	GLuint VBOs[7], VAOs[7];
	glGenVertexArrays(7, VAOs); // the address-of operator (&) is not needed here, as the array name is a pointer type
	glGenBuffers(7, VBOs);

	// ================================
	// Large Red Triangle setup
	// ===============================
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(largeTriangleRed), largeTriangleRed, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same, note that the stride is 6*sizeof(GLloat)			
	glEnableVertexAttribArray(0);  // set attribute index of the position attribute to 0 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	// ================================
	// Large Blue Triangle setup
	// ===============================
	glBindVertexArray(VAOs[1]);	// Note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// And a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(largeTriangleBlue), largeTriangleBlue, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// ================================
	// Medium Triangle setup
	// ===============================
	glBindVertexArray(VAOs[2]);	// Note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);	// And a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(medTriangle), medTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// ================================
	// Small Triangle 1 setup
	// ===============================
	glBindVertexArray(VAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(smallTriangle1), smallTriangle1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same, note that the stride is 6*sizeof(GLloat)			
	glEnableVertexAttribArray(0);  // set attribute index of the position attribute to 0 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	// ================================
	// Small Triangle 2 setup
	// ===============================
	glBindVertexArray(VAOs[4]);	// Note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);	// And a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(smallTriangle2), smallTriangle2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// ================================
	// Square setup
	// ===============================
	glBindVertexArray(VAOs[5]);	// Note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]);	// And a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// ================================
	// Parallelogram setup
	// ===============================
	glBindVertexArray(VAOs[6]);	// Note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[6]);	// And a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(parallelogram), parallelogram, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//++++++++++Build and compile shader program+++++++++++++++++++++
	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");

	//++++++++++++++++++++++++++++++++++++++++++++++
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float time = (GLfloat)glfwGetTime() * 0.1f;

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		GLint transformLoc;

		// Draw Large Triangle Red
		glm::mat4 transform;
		glm::mat4 rotate;
		glm::mat4 shrink;
		glm::mat4 temp;

		glm::mat4 shapeTransformations[9];
		transform = glm::mat4();

		shrink = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.0f));
		rotate = glm::rotate(rotate, (GLfloat)(3.14 / 4.0), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 iden;

		/*if (time <= 1.0)
		{
			if (time <= 0.5)
			{
				for (int i = 0; i < 7; i++)
				{
					shapeTransformations[i] = glm::mix(iden, rotate, time);
				}
			}
			if (time >= 0.5 )
			{
				for (int i = 0; i < 7; i++)
				{
					shapeTransformations[i] = glm::mix(iden, shrink, time);
				}
			}
		} */

		if ((time >= 0.1) && (time <= 1))
		{
			glm::mat4 temp2;
			glm::mat4 temp3;
			glm::mat4 temp4;

			temp = glm::mat4();
			temp2 = glm::mat4();

			//Move the Large Red Triangle
			temp = glm::translate(temp, glm::vec3(0.5f, 0.0f, 0.0f));
			shapeTransformations[0] = glm::mix(iden, temp, time);


			//Move the Large Blue Triangle
			temp = glm::mat4();
			temp = glm::rotate(temp, (GLfloat)(3.14 / 1), glm::vec3(0.0f, 0.0f, 1.0f));
			temp2 = glm::translate(temp, glm::vec3(-0.5f, 0.0f, 0.0f));
			shapeTransformations[1] = glm::mix(iden, temp2, time);

			//Move the Medium Triangle
			temp = glm::mat4();
			temp2 = glm::mat4();
			temp3 = glm::mat4();

			temp = glm::translate(temp, glm::vec3(-0.5f, 0.0f, 0.0f));
			temp2 = glm::rotate(temp, (GLfloat)(3.14 / -4), glm::vec3(0.0f, 0.0f, 1.0f));
			temp3 = glm::translate(temp2, glm::vec3(0.5f, 0.25f, 0.0f));
			shapeTransformations[2] = glm::mix(iden, temp3, time);

			//Move the Small Triangle 1
			temp = glm::mat4();

			temp = glm::rotate(temp, (GLfloat)((3.14 / 4) + (3.14 / 2)), glm::vec3(0.0f, 0.0f, 1.0f));
			shapeTransformations[3] = glm::mix(iden, temp, time);

		}

		for (int i = 0; i < 4; i++)
		{
			transform = shapeTransformations[i];

			transformLoc = glGetUniformLocation(shaderProgram, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

			glBindVertexArray(VAOs[i]);
			glDrawArrays(GL_TRIANGLES, 0, 6);

		}

		glBindVertexArray(0);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(5, VAOs);
	glDeleteBuffers(5, VBOs);

	glfwTerminate();
	return 0;
}





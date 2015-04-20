
#include "lsms.h"
#include "draw_lsms.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

int main( void )
{
        int i = 0;
        lsms * l = lsms_create_from_file("test_spring_1.txt");
	draw_lsms * dl = draw_lsms_create_from_file("test_spring_draw_1.txt");

        lsms_first_step(l, .001);
	printf("done \n");
	printf("done %f \n", l->p[0]->x_a);
	printf("done %f \n", l->p[0]->y_a);
	printf("done %f \n", l->p[0]->z_a);
        printf("done %f \n", l->p[1]->x_a);
	printf("done %f \n", l->p[1]->y_a);
	printf("done %f \n", l->p[1]->z_a);
        printf("done %f \n", l->p[2]->x_a);
	printf("done %f \n", l->p[2]->y_a);
	printf("done %f \n", l->p[2]->z_a);



        

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 0 - Keyboard and Mouse", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");


	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static GLfloat * g_vertex_buffer_data = new GLfloat[3 * dl->num_draw_vertices];
	static GLfloat * g_color_buffer_data = new GLfloat[3 * dl->num_draw_vertices];

                for(i = 0; i < dl->num_draw_vertices; i++)
                {
                 //   glm::vec3 pos = 
	            g_vertex_buffer_data[3*i + 0] = (GLfloat)particle_get_x_a(l->p[dl->draw_vertices_list[i]]);
	            g_vertex_buffer_data[3*i + 1] = (GLfloat)particle_get_y_a(l->p[dl->draw_vertices_list[i]]);
	            g_vertex_buffer_data[3*i + 2] = (GLfloat)particle_get_z_a(l->p[dl->draw_vertices_list[i]]);
	            g_color_buffer_data[3*i + 0] = 1;
	            g_color_buffer_data[3*i + 1] = .2;
	            g_color_buffer_data[3*i + 2] = .2;
                }

	static GLfloat test_g_vertex_buffer_data[] = { 
		-30.0f,
                -10.0f,
                -10.0f,
		-1.0f,-1.0f, 1.0f,
		-30.0f, 10.0f, 1.0f,
	};
        printf("sdlafkj %f \n", g_vertex_buffer_data[3]);

	// Two UV coordinatesfor each vertex. They were created withe Blender.
	static GLfloat test_g_color_buffer_data[] = { 
		0.1f, 0.4f, .1f,
		0.1f, 0.3f, .1f,
		0.1f, 0.335903f, .1f, 
	};





//	printf("done %f \n", g_color_buffer_data[1]);
//	printf("done %f \n", g_color_buffer_data[2]);
//	printf("done %f \n", g_color_buffer_data[3]);
//	printf("done %f \n", g_color_buffer_data[4]);
//	printf("done %f \n", g_vertex_buffer_data[5]);
//	printf("done %f \n", g_vertex_buffer_data[6]);
//	printf("done %f \n", g_vertex_buffer_data[7]);
//	printf("done %f \n", g_vertex_buffer_data[8]);
	printf("done %i \n", sizeof(test_g_vertex_buffer_data));
	printf("done %i \n", sizeof(g_vertex_buffer_data));

        int size = 3 * dl->num_draw_vertices * sizeof(GLfloat);


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);

        double lastTime = glfwGetTime();
	do{
                
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
                int num_steps = (int)(delta*10000.0);
                if (num_steps > 100)
                    num_steps = 100;
                
		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

                lsms_update(l, .001, num_steps);

                for(i = 0; i < dl->num_draw_vertices; i++)
                {
	            g_vertex_buffer_data[3*i + 0] = particle_get_x_a(l->p[dl->draw_vertices_list[i]]);
	            g_vertex_buffer_data[3*i + 1] = particle_get_y_a(l->p[dl->draw_vertices_list[i]]);
	            g_vertex_buffer_data[3*i + 2] = particle_get_z_a(l->p[dl->draw_vertices_list[i]]);
	            g_color_buffer_data[3*i + 0] = 1;
	            g_color_buffer_data[3*i + 1] = .2;
	            g_color_buffer_data[3*i + 2] = .2;
                }

                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, size, g_vertex_buffer_data);
                glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, size, g_color_buffer_data);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);




		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_POINTS, 0, 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


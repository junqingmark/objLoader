#include "objLoader.h"
#include "shader.h"
#include "texture.h"

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

int current[2] = {0, 0};
int last[2] = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};

int main(int argc, char* argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("unable to initialize SDL: %s!\n", SDL_GetError());
		return 1;

	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_Window* window = SDL_CreateWindow("model loading", 0, 0, 1024, 768, SDL_WINDOW_OPENGL);
	if(NULL == window)
	{
		SDL_Log("Fail to create window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if(NULL == context)
	{
		SDL_Log("Fail to create context: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		return 1;
	}

	SDL_GL_MakeCurrent(window, context);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);

	//create and compile our GLSL program
	GLuint program = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	GLuint mvp = glGetUniformLocation(program, "mvp");
	printf("The mvp matrix id is %d\n", mvp);

	//Load the texture
	GLuint texture = loadDDS("uvmap.DDS");

	GLuint vertexShaderID = glGetAttribLocation(program, "v_position");
	GLuint fragmentShaderID = glGetAttribLocation(program, "v_coord");

	//read obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	bool result = loadOBJ("cube.obj", vertices, uvs, normals);

	//load it into a vbo
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glm::mat4 model(1.0f);
	glm::mat4 projectionMatrix = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
	glm::vec3 position = glm::vec3( 5, 5, 5 );
	

	glm::mat4 viewMatrix = glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

	glm::mat4 mymvp = projectionMatrix * viewMatrix * model;
	int exit = 0; 

	SDL_Event event;

	while(!exit)
	{
		/*
		if(SDL_PollEvent(&event))
		{
			if(event.type == SDL_MOUSEMOTION)
			{
				current[0] = event.motion.x;
				current[1] = event.motion.y;

				float angle_x = (current[0] - last[0]) * 360.0f / SCREEN_WIDTH;
				float angle_y = (current[1] - last[1]) * 360.0f / SCREEN_HEIGHT;

				last[0] = current[0];
				last[1] = current[1];

				model = glm::rotate(glm::mat4(1.0f), angle_x, glm::vec3(1.0, 0.0, 0.0));
				model = glm::rotate(model, angle_y, glm::vec3(0.0, 1.0, 0.0));

			}

			if(event.type == SDL_QUIT)
			{
				exit = 1;
			}
		}
		*/

		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);

		glUniformMatrix4fv(mvp, 1, GL_FALSE, &mymvp[0][0]);
		glBindTexture(GL_TEXTURE_2D, texture);

		//vertices
		glEnableVertexAttribArray(vertexShaderID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(vertexShaderID, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//uv
		glEnableVertexAttribArray(fragmentShaderID);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(fragmentShaderID, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		SDL_GL_SwapWindow(window);

	}


	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
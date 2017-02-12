#include <SDL2/SDL.h>
#include "controls.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

int current[2] = {0, 0};
int last[2] = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};

glm::mat4 getViewMatrix()
{
	return viewMatrix;
}

glm::mat4 getProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 computeMatricesFromInputs()
{
	SDL_Event event;
	if(SDL_PollEvents(&event))
	{
		if(event.type == SDL_MOUSEMOTION)
		{
			current[0] = event.motion.x;
			current[1] = event.motion.y;

			float angle_x = (current[0] - last[0]) * 360.0f / SCREEN_WIDTH;
			float angle_y = (current[1] - last[1]) * 360.0f / SCREEN_HEIGHT;

			last[0] = current[0];
			last[1] = current[1];

		}
	}

	glm::mat4 model; 
	model = glm::rotate(glm::mat4(1.0f), angle_x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, angle_y, glm::vec3(0.0, 1.0, 0.0));

	return model;

}

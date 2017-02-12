#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void computeMatricesFromInputs();

glm::mat4 getViewMatrix();

glm::mat4 getProjectionMatrix();

#endif
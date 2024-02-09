#include "camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(const glm::vec3& position, const glm::vec2& orientation) : m_position(position), m_orientation(orientation)
{
	// Initialisation des attributs de la classe
}

glm::mat4 Camera::getFirstPersonViewMatrix()
{
	// Calcul et retour de la matrice de vue en première personne
	// à partir des attributs de la classe
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::rotate(view, m_orientation.y, glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, m_orientation.x, glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, -m_position);
	return view;
}

// glm::mat4 Camera::getThirdPersonViewMatrix()
// {
// 	// TODO: Calcul et retour de la matrice de vue en troisième personne
// 	// à partir des attributs de la classe
// }


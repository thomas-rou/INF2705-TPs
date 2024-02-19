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
	view = glm::translate(view, -m_position);
	view = glm::rotate(view, m_orientation.x, glm::vec3(0.0f, abs(m_position.x), 0.0f));
	view = glm::rotate(view, m_orientation.y, glm::vec3(abs(m_position.x), 0.0f, 0.0f));
	
	return view;
}

 glm::mat4 Camera::getThirdPersonViewMatrix()
 {
 	// TODO: Calcul et retour de la matrice de vue en troisième personne
 	// à partir des attributs de la classe
	 float radius = 6.0;
	 float yaw = m_orientation.y;
	 float pitch = m_orientation.x + M_PI/2;
	 glm::vec3 cameraPosition = glm::vec3(
		 radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch)), 
		 radius * sin(glm::radians(pitch)), 
		 radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	 );

	 glm::mat4 view = glm::lookAt(cameraPosition, m_position, glm::vec3(0.0f, abs(m_position.x), 0.0f));

	 return view;
 }


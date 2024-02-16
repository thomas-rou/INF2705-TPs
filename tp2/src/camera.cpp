#include "camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

Camera::Camera(const vec3& position, const vec2& orientation) : m_position(position), m_orientation(orientation)
{
	// Initialisation des attributs de la classe
}

mat4 Camera::getFirstPersonViewMatrix()
{
	// Calcul et retour de la matrice de vue en première personne
	// à partir des attributs de la classe
	mat4 view = mat4(1.0f);
	view = rotate(view, m_orientation.y, vec3(1.0f, 0.0f, 0.0f));
	view = rotate(view, m_orientation.x, vec3(0.0f, 1.0f, 0.0f));
	view = translate(view, -m_position);
	return view;
}

mat4 Camera::getThirdPersonViewMatrix()
{
	// Calcul et retour de la matrice de vue en troisième personne
	// à partir des attributs de la classe

	float radius = 6.0f;
	float theta = m_orientation.x + (M_PI / 2);
	float phi = m_orientation.y;
	vec3 cameraPosition = vec3(
		radius * sin(phi) * cos(theta),
		radius * cos(phi),
		radius * sin(phi) * sin(theta)
	);

	cameraPosition += m_position;
	vec3 lookAtPosition = m_position;

	mat4 view = lookAt(cameraPosition, lookAtPosition, vec3(0.0f, 1.0f, 0.0f));

	return view;
}
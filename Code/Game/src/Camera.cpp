#include "Camera.h"

Camera::Camera()
{
	UpdateOrientation();
}

Camera::~Camera()
{

}

void Camera::SetYaw(float aAngle) 
{ 
	m_theta = aAngle; 

	UpdateOrientation(); 
}

void Camera::SetPitch(float aAngle)
{
	if (aAngle > 89.0f)
		m_phi = 89.0f;
	else if (aAngle < -89.0f)
		m_phi = -89.0f;
	else
		m_phi = aAngle;

	UpdateOrientation();
}

void Camera::LookAt(glm::mat4& aModelView)
{
	aModelView = glm::lookAt(m_position, m_position + m_orientation, glm::vec3(0.0, 1.0, 0.0));
}

void Camera::UpdateOrientation()
{
	float phiRadian = m_phi * M_PI / 180;
	float thetaRadian = m_theta * M_PI / 180;

	m_orientation.x = cos(phiRadian) * sin(thetaRadian);
	m_orientation.y = sin(phiRadian);
	m_orientation.z = cos(phiRadian) * cos(thetaRadian);

	m_crossOrientation = glm::normalize(glm::cross(m_orientation, glm::vec3(0.0f, 1.0f, 0.0f)));
}
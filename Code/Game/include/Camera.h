#pragma once

class Camera
{
public:

	Camera();
	~Camera();

	void LookAt(glm::mat4& aModelView);

	void SetYaw(float aAngle);
	void SetPitch(float aAngle);

	float GetYaw() const { return m_theta; }
	float GetPitch() const { return m_phi; }

	const glm::vec3 GetOrientation() const { return m_orientation; }

	void Yaw(float aAngle){ SetYaw(m_theta + aAngle); }
	void Pitch(float aAngle){ SetPitch(m_phi + aAngle); }

	void SetPosition(const glm::vec3& acPosition) { m_position = acPosition; }
	glm::vec3 GetPosition() const { return m_position; }

	void Forward(float aDistance){ m_position += m_orientation * aDistance; }
	void Strafe(float aDistance){ m_position += m_crossOrientation * aDistance; }

protected:

	void UpdateOrientation();

private:

	float m_theta{ 0.0f };
	float m_phi{ 0.0f };

	glm::vec3 m_position;
	glm::vec3 m_orientation;
	glm::vec3 m_crossOrientation;
};
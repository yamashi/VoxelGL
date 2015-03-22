#pragma once

#include "Shader.h"

struct UIRenderContext
{
	glm::mat4 Projection;
	glm::mat4 ModelView;
	glm::mat4 MVP;
	GLuint ProjectionIndex;
	GLuint ModelViewIndex;
	GLuint MVPIndex;
};

class UIElement
{
public:

	UIElement(){};
	virtual ~UIElement(){};

	virtual void Draw(const UIRenderContext& acRenderContext);

	void AddChild(const std::shared_ptr<UIElement>& acElement);
	void RemoveChild(const std::shared_ptr<UIElement>& acElement);

	void SetPosition(float aX, float aY) { m_posX = aX; m_posY = aY; }
	float GetPositionX() const { return m_posX; }
	float GetPositionY() const { return m_posY; }

private:

	std::list<std::shared_ptr<UIElement> > m_childrenList;

	float m_posX{ 0.0f };
	float m_posY{ 0.0f };
};

using UIElementPtr = std::shared_ptr < UIElement > ;
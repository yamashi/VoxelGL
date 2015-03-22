#pragma once

#include "UIElement.h"

void InitializeUI();
void ShutdownUI();

class UserInterface
{
public:

	UserInterface();
	~UserInterface();

	void Draw(float aScreenWidth, float aScreenHeight);
	void AddChild(const UIElementPtr& acElement);
	void RemoveChild(const UIElementPtr& acElement);

private:

	UIElement m_root;
	ShaderPtr m_pShader;
	GLuint m_projectIndex;
	GLuint m_modelViewIndex;
	GLuint m_mvpIndex;
};
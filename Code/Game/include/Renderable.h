#pragma once

class Renderable
{
public:

	Renderable(){}
	virtual ~Renderable(){}

	virtual void Draw(glm::mat4 aProjection, glm::mat4 aModelView) = 0;
};
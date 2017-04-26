#pragma once
#include "Prerequisites.hpp"
#include <glm/mat4x4.hpp>
#include "gl_core_3_3.hpp"

class GuiObject
{
public:
	GuiObject();
	virtual ~GuiObject() = 0;

	virtual void draw(class Renderer& renderer) const = 0;
	virtual void grabInput(const glm::mat4& orthoMatrix, class Application& app) = 0;
	virtual void update(double deltaTime) = 0;

protected:
};


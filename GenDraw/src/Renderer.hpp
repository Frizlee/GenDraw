#pragma once
#include "Prerequisites.hpp"
#include "gl_core_3_3.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include "Shader.hpp"


class Renderer
{
public:
	Renderer();
	~Renderer();

	void init();

	// Consider using const references while not drunk

	void bindShader(class Shader& shader);
	void bindTexture(class Texture& texture, std::int32_t slot);
	void bindVertexArray(class VertexArray &arr);
	void bindBuffer(class GpuBuffer &buf);
	void bindBuffer(class GpuBuffer &buf, enum class GpuBufferType &type);
	void bindOrthoMatrix(const glm::mat4& orthoMatrix);
	void setDepthTest(bool state);
	void setViewport(const glm::tvec2<std::int32_t>& dimensions);

	void prepareDrawSprite();
	void prepareDrawText();
	void prepareDrawSpriteInstanced();

	void drawSprite(const class Sprite& sprite, const glm::mat4& transformMatrix);
	void drawText(const class Text& text, const glm::mat4& transformMatrix);
	void drawSpriteInstanced(const class Sprite& sprite, const glm::mat4& transformMatrix, 
		std::uint32_t instances);

	bool isInitialized() const;
	
private:
	bool mInitialized;

	enum class Setup
	{
		SPRITE,
		TEXT,
		SPRITE_INSTANCED,

		NONE
	} mSetup;

	glm::mat4 mOrthoMatrix;

	Shader mTextShader;
	Shader mSpriteShader;
	Shader mSpriteInstancedShader;
	Shader mWaterShader;

	GLuint mSamplerMipmapLinear;
	GLuint mSamplerLinear;

	GLint mOrthographicMatrixLocationSprite;
	GLint mSamplerLocationSprite;

	GLint mOrthographicMatrixLocationText;
	GLint mSamplerLocationText;
	GLint mTextColorLocationText;
	
	GLint mOrthographicMatrixLocationSpriteInstanced;
	GLint mSamplerLocationSpriteInstanced;

	GLint mOrthographicMatrixLocationWater;
	GLint mSamplerLocationWater;
	GLint mDisplacementLocationWater;
};




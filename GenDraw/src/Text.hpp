#pragma once
#include "Prerequisites.hpp"
#include "VertexLayout.hpp"
#include <glm/vec4.hpp>

class Text
{
	friend class Renderer;

public:
	Text();
	Text(const std::string& content, std::shared_ptr<class Font> fnt, 
		std::shared_ptr<class VertexArray>& vao, std::uint32_t vboPosition, Renderer& renderer);
	void create(const std::string& content, std::shared_ptr<class Font> fnt, 
		std::shared_ptr<class VertexArray>& vao, std::uint32_t vboPosition, Renderer& renderer);

	void updateContent(const std::string& content, class Renderer& renderer);

	void setColor(const glm::vec4& color);

	const glm::vec4& getColor() const;
	std::weak_ptr<const class Texture> getTexture() const;
	const std::vector<TextVertexLayout::Data>& getBuffer() const;

private:
	std::vector<TextVertexLayout::Data> mBuffer;
	glm::vec4 mColor;
	std::shared_ptr<class VertexArray> mVao;
	std::uint32_t mVboPosition;
	std::shared_ptr<class Texture> mTexture;
	std::shared_ptr<class Font> mFont;
};




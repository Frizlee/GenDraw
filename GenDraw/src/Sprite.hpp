#pragma once
#include "Prerequisites.hpp"
#include "VertexLayout.hpp"
#include <glm/vec2.hpp>

class Sprite
{
	friend class Renderer;

public:
	Sprite();
	
	Sprite(const glm::vec2& topRight, const glm::vec2& uvDownLeft,
		const glm::vec2& uvTopRight, float z, std::shared_ptr<class Texture>& tex,
		std::shared_ptr<class VertexArray>& vao, class Renderer& renderer);
	void create(const glm::vec2& topRight, const glm::vec2& uvDownLeft, 
		const glm::vec2& uvTopRight, float z, std::shared_ptr<class Texture>& tex,
		std::shared_ptr<class VertexArray>& vao, class Renderer& renderer);
	
	
	~Sprite();

	std::weak_ptr<const class Texture> getTexture() const;
	const std::vector<TextureVertexLayout::Data>& getBuffer() const;
	std::weak_ptr<const class VertexArray> getVao() const;
	std::weak_ptr<class VertexArray> getVao();
	const glm::vec2& getBounds() const;

private:
	std::vector<TextureVertexLayout::Data> mBuffer;
	std::shared_ptr<class VertexArray> mVao;
	std::uint32_t mVboPosition;
	std::shared_ptr<class Texture> mTexture;
	glm::vec2 mBounds;
};


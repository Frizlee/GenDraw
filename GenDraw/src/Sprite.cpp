#include "Sprite.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Renderer.hpp"
using namespace std;


Sprite::Sprite()
{
}

Sprite::Sprite(const glm::vec2& topRight, const glm::vec2& uvDownLeft,
	const glm::vec2& uvTopRight, float z, shared_ptr<Texture>& tex, shared_ptr<VertexArray>& vao,
	Renderer& renderer)
{
	create(topRight, uvDownLeft, uvTopRight, z, tex, vao, renderer);
}

void Sprite::create(const glm::vec2& topRight, const glm::vec2& uvDownLeft, 
	const glm::vec2& uvTopRight, float z, shared_ptr<Texture>& tex, shared_ptr<VertexArray>& vao, 
	Renderer& renderer)
{
	typedef TextureVertexLayout::Data Vertex;

	mTexture = tex;
	mVao = vao;
	mBounds = topRight;

	mBuffer.push_back(Vertex{ 0.0f, 0.0f, z, uvDownLeft.x, uvDownLeft.y });
	mBuffer.push_back(Vertex{ 0.0f, topRight.y, z, uvDownLeft.x, uvTopRight.y });
	mBuffer.push_back(Vertex{ topRight.x, 0.0f, z, uvTopRight.x, uvDownLeft.y });
	mBuffer.push_back(Vertex{ 0.0f, topRight.y, z, uvDownLeft.x, uvTopRight.y });
	mBuffer.push_back(Vertex{ topRight.x, topRight.y, z, uvTopRight.x, uvTopRight.y });
	mBuffer.push_back(Vertex{ topRight.x, 0.0f, z, uvTopRight.x, uvDownLeft.y });

	auto buffer = dynamic_pointer_cast<VertexBuffer<TextureVertexLayout>>(mVao->getBuffer().lock());
	mVboPosition = buffer->add(mBuffer, renderer);
}

Sprite::~Sprite()
{
}

weak_ptr<const Texture> Sprite::getTexture() const
{
	return mTexture;
}

const vector<TextureVertexLayout::Data>& Sprite::getBuffer() const
{
	return mBuffer;
}

std::weak_ptr<const class VertexArray> Sprite::getVao() const
{
	return mVao;
}

std::weak_ptr<class VertexArray> Sprite::getVao()
{
	return mVao;
}

const glm::vec2& Sprite::getBounds() const
{
	return mBounds;
}

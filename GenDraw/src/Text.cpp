#include "Text.hpp"
#include "Font.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include <glm/vec2.hpp>
using namespace std;

Text::Text()
	: mColor{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

Text::Text(const string& content, std::shared_ptr<Font> fnt, std::shared_ptr<VertexArray>& vao,
	uint32_t vboPosition, Renderer& renderer)
	: mColor{ 0.0f, 0.0f, 0.0f, 1.0f }
{
	create(content, fnt, vao, vboPosition, renderer);
}

void Text::create(const string& content, std::shared_ptr<Font> fnt,
	std::shared_ptr<VertexArray>& vao, uint32_t vboPosition, Renderer& renderer)
{
	mVao = vao;
	mVboPosition = vboPosition;
	mFont = fnt;

	if (!content.empty())
		updateContent(content, renderer);

	mTexture = fnt->getTexture(renderer).lock();
}

void Text::updateContent(const std::string& content, Renderer& renderer)
{
	mBuffer.clear();

	basic_string<uint32_t> text32{ wstring_convert<
		codecvt_utf8<uint32_t>, uint32_t>{}.from_bytes(content) };

	mBuffer.reserve(text32.length() * 6);

	glm::vec2 penPos(0.0f, 0.0f);

	for (auto& i : text32)
		mFont->putChar(i, mBuffer, penPos);

	auto vbo = dynamic_pointer_cast<VertexBuffer<TextVertexLayout>>(mVao->getBuffer().lock());
	mVboPosition = vbo->add(mBuffer, mVboPosition, renderer);
}

void Text::setColor(const glm::vec4& color)
{
	mColor = color;
}

const glm::vec4& Text::getColor() const
{
	return mColor;
}

weak_ptr<const Texture> Text::getTexture() const
{
	return mTexture;
}

const std::vector<TextVertexLayout::Data>& Text::getBuffer() const
{
	return mBuffer;
}


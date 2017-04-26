#pragma once
#include "Prerequisites.hpp"
#include "Resource.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "VertexLayout.hpp"
#include <glm/vec2.hpp>

class FontInitializationException : public std::exception
{
	virtual const char* what() const noexcept
	{
		return "Initialization of the Font object failed.";
	}
};

class Font : public Resource
{
	friend class Text;

public:
	Font();
	Font(const std::string& facePath, std::uint8_t size, 
		std::shared_ptr<class ImageAtlas>& atlas, class Renderer& renderer, float spacing = 1.5f);
	void create(const std::string& facePath, std::uint8_t size, 
		std::shared_ptr<class ImageAtlas> atlas, class Renderer& renderer, float spacing = 1.5f);

	~Font();
	void clear() override;

	std::weak_ptr<const class ImageAtlas> getAtlas() const;
	std::uint32_t getSize() const override;

private:
	struct GlyphInfo
	{
		float ax, ay;
		float bw, bh;
		float bl, bt;
		uint32_t tx, ty;
	};

	FT_Library FTLib; 
	FT_Face mFace;
	std::shared_ptr<ImageAtlas> mAtlas;
	std::shared_ptr<class Texture> mTexture;
	bool mIsTextureChanged;

	std::uint8_t mSize;
	std::uint32_t mTabWidth;
	std::int32_t mAscender;
	std::int32_t mDescender;
	std::uint16_t mHeight;
	float mSpacing;

	std::unordered_map<std::uint32_t, GlyphInfo> mChars;

	bool cacheChar(std::uint32_t charCode);
	void putChar(std::uint32_t charCode, std::vector<TextVertexLayout::Data>& buffer, 
		glm::vec2 &penPos);
	std::weak_ptr<class Texture> getTexture(class Renderer& renderer);
};




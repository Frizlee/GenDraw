#include "Font.hpp"
#include "ImageAtlas.hpp"
#include "Texture.hpp"
#include <glm/vec2.hpp>
using namespace std;

static const int tabSize = 8;

Font::Font()
	: mIsTextureChanged{ true }
{
	FT_Init_FreeType(&FTLib);
}

Font::Font(const string& facePath, uint8_t size, shared_ptr<ImageAtlas>& atlas, 
	Renderer& renderer, float spacing)
	: mIsTextureChanged{ true }
{
	if (FT_Init_FreeType(&FTLib))
	{
		// TODO: Error handling
	}

	create(facePath, size, atlas, renderer, spacing);
}

void Font::create(const string& facePath, uint8_t size, shared_ptr<ImageAtlas> atlas, 
	Renderer& renderer, float spacing)
{
	if (FT_New_Face(FTLib, facePath.c_str(), 0, &mFace))
		return;

	FT_Error err = FT_Select_Charmap(mFace, ft_encoding_unicode);
	FT_GlyphSlot g = mFace->glyph;
	swap(mAtlas, atlas);
	mTexture = make_shared<Texture>(*mAtlas, renderer);
	mSize = size;

	FT_Set_Pixel_Sizes(mFace, 0, mSize);

	mAscender = mFace->bbox.yMax >> 6;
	mDescender = mFace->bbox.yMin >> 6;
	mHeight = mFace->height >> 6;
	mSpacing = spacing;

	// Default characters
	for (uint8_t i = 32; i < 128; i++)
	{
		cacheChar(i);
		// Current glyph remains until next cacheChar call

		// Calculate tab width
		if (i == 32)
			mTabWidth = (g->advance.x * tabSize) >> 6;
	}
}

Font::~Font()
{
}

void Font::clear()
{
	mAtlas->clear();
	mChars.clear();
}

weak_ptr<const ImageAtlas> Font::getAtlas() const
{
	return mAtlas;
}

uint32_t Font::getSize() const
{
	return mChars.size() * (sizeof(GlyphInfo) + sizeof(uint32_t));
}

bool Font::cacheChar(uint32_t charCode)
{
	FT_GlyphSlot g = mFace->glyph;

	if (FT_Load_Char(mFace, charCode, FT_LOAD_RENDER))
		return false;

	GlyphInfo gi;
	gi.tx = 0;
	gi.ty = 0;

	if (g->bitmap.buffer != nullptr)
	{
		Image img(g->bitmap.width, g->bitmap.rows, Image::Format::R, 
			move(vector<uint8_t>(g->bitmap.buffer, 
				g->bitmap.buffer + g->bitmap.width * g->bitmap.rows)));
		img.flipVerticaly();

		ImageAtlas::Rect rc = mAtlas->insert(img);
		mIsTextureChanged = true;

		if (rc == ImageAtlas::Rect(0, 0, 0, 0))
			return false;

		gi.tx = rc.x;
		gi.ty = rc.y;
	}

	gi.ax = static_cast<float>(g->advance.x >> 6);
	gi.ay = static_cast<float>(g->advance.y >> 6);
	gi.bw = static_cast<float>(g->bitmap.width);
	gi.bh = static_cast<float>(g->bitmap.rows);
	gi.bl = static_cast<float>(g->bitmap_left);
	gi.bt = static_cast<float>(g->bitmap_top);

	mChars.insert({ charCode, gi });

	return true;
}

void Font::putChar(uint32_t charCode, vector<TextVertexLayout::Data>& buffer, 
	glm::vec2 &penPos)
{
	typedef TextVertexLayout::Data Vertex;

	// Control characters.
	if (charCode == '\n')
	{
		penPos.y -= static_cast<float>(mHeight) * mSpacing;
		penPos.x = 0.0f;
		return;
	}

	if (charCode == '\t')
	{
		// It should start from 0.0f
		uint32_t tabs = static_cast<uint32_t>(penPos.x / mTabWidth + 1.0f);
		penPos.x = static_cast<float>(tabs * mTabWidth);
		return;
	}
	
	unordered_map<uint32_t, GlyphInfo>::iterator it = mChars.find(charCode);

	if (it == mChars.end())
	{
		if (cacheChar(charCode) == false)
			return;

		it = mChars.find(charCode);
	}
	
	float x2 = penPos.x + it->second.bl;
	float y2 = penPos.y + it->second.bt - mAscender;
	float tu = static_cast<float>(it->second.tx) / mAtlas.get()->getWidth();
	float tv = static_cast<float>(it->second.ty) / mAtlas.get()->getHeight();

	penPos.x += it->second.ax;
	penPos.y += it->second.ay;

	float w = it->second.bw;
	float h = it->second.bh;

	if (!w || !h)
		return;

	buffer.push_back(Vertex{ x2, y2, tu, tv + h / mAtlas->getHeight() });
	buffer.push_back(Vertex{ x2 + w, y2, tu + w / mAtlas->getWidth(), 
		tv + h / mAtlas->getHeight() });
	buffer.push_back(Vertex{ x2, y2 - h, tu, tv });
	buffer.push_back(Vertex{ x2 + w, y2, tu + w / mAtlas->getWidth(),
		tv + h / mAtlas->getHeight() });
	buffer.push_back(Vertex{ x2, y2 - h, tu, tv });
	buffer.push_back(Vertex{ x2 + w, y2 - h, tu + w / mAtlas->getWidth(), tv });
}

weak_ptr<Texture> Font::getTexture(Renderer& renderer)
{
	if (mIsTextureChanged && mAtlas != nullptr)
	{
		mTexture->create(*mAtlas, renderer);
		mIsTextureChanged = false;
	}

	return mTexture;
}


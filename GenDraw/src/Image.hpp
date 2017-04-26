#pragma once
#include "Prerequisites.hpp"
#include "Resource.hpp"


class Image : public Resource
{
	friend class Texture;
	friend class ImageAtlas;

public:
	enum class Format
	{
		UNKNOWN = 0,

		R8,
		RG8,
		RGB8,
		RGBA8,
		R3G3B2,
		R5G6B5,
		RGBA4,
		RGB5A1,
		RGB10A2,
		
		SRGB8,
		SRGB8A8,

		// Compressed formats.
		BC1_RGB,
		BC1_RGBA,
		BC2_RGBA,
		BC3_RGBA,
		BC4_R,
		BC4_SIGNED_R,
		BC5_RG,
		BC5_SIGNED_RG,

		// Aliases.
		R = R8,
		RG = RG8,
		RGB = RGB8,
		RGBA = RGBA8,
	};	
	
	Image();

	// Copy/Move constructors and assignments
	Image(const Image &lhs);
	Image(Image &&rhs);
	Image& operator=(Image cas);
	Image& operator=(Image &&rhs);
	
	// Copy bytes.
	Image(std::uint32_t width, std::uint32_t height, Format format, 
		const std::vector<std::uint8_t> &bytes);
	void create(std::uint32_t width, std::uint32_t height, Format format,
		const std::vector<std::uint8_t> &bytes);

	// Move bytes.
	Image(std::uint32_t width, std::uint32_t height, Format format, 
		std::vector<std::uint8_t> &&bytes);
	void create(std::uint32_t width, std::uint32_t height, Format format,
		std::vector<std::uint8_t> &&bytes);

	// Without bytes.
	Image(std::uint32_t width, std::uint32_t height, Format format);
	void create(std::uint32_t width, std::uint32_t height, Format format);
	
	virtual ~Image();
	void clear() override;
	
	Format getFormat() const;
	std::uint32_t getWidth() const;
	std::uint32_t getHeight() const;
	std::vector<std::uint8_t> getPixel(std::uint32_t x, std::uint32_t y) const;
	bool isCompressed() const;

	// Return block size if compressed
	std::uint8_t getBytesPerPixel() const;
	static std::uint8_t CalculateBytesPerPixel(Format format);

	// x and y must be in range [0, mWidth] and [0, mHeight]
	void setPixel(std::uint32_t x, std::uint32_t y, const std::vector<std::uint8_t> &bytes);
	std::vector<std::uint8_t>& getBytes();
	void flipVerticaly();
	
	std::uint32_t getSize() const override;

protected:
	std::vector<std::uint8_t> mBytes;

private:
	std::uint32_t mWidth;
	std::uint32_t mHeight;
	Format mFormat;
};


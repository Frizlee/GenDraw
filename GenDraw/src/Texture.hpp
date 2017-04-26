#pragma once
#include "Prerequisites.hpp"
#include "Image.hpp"
#include "GpuResource.hpp"

class Texture : public GpuResource
{
public:
	enum class Type
	{
		UNCOMPRESSED_2D,
		COMPRESSED_2D,
		UNCOMPRESSED_2D_ARRAY,
		COMPRESSED_2D_ARRAY
	};

	Texture();

	// Copy/Move constructors and assignments
	Texture(const Texture& lhs) = delete;
	Texture(Texture&& rhs);
	Texture& operator=(const Texture& lhs) = delete;
	Texture& operator=(Texture&& rhs);

	// Create texture
	Texture(const Image& img, Renderer& renderer);
	void create(const Image& img, Renderer& renderer);

	// Create texture array
	// This functions needs to be reimlemented differently
	Texture(std::vector<Image>& imgs, Renderer& renderer);
	void create(std::vector<Image>& imgs, Renderer& renderer);
	
	~Texture();
	void clear() override;

	void generateMipmaps(class Renderer& renderer);
	Image::Format getFormat() const;
	bool hasMipmap() const;
	Type getType() const;
	std::uint32_t getSize() const override;

private:
	Image::Format mFormat;
	Type mType;
	std::uint32_t mSize;
	bool mHasMipmap;

	static void ParseFormat(Image::Format imgFormat, GLint &format, 
		GLint &internalFormat, GLenum &dataType, GLenum &compressedInternalFormat);
};


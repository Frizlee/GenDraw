#include "Texture.hpp"
#include "Renderer.hpp"
using namespace std;

Texture::Texture()
	: mHasMipmap{ false }
{
}

Texture::Texture(Texture&& rhs)
	: GpuResource(move(rhs))
{
	swap(mFormat, rhs.mFormat);
	swap(mType, rhs.mType);
	swap(mHasMipmap, rhs.mHasMipmap);
}

Texture& Texture::operator=(Texture&& rhs)
{
	GpuResource::operator=(move(rhs));
	swap(mFormat, rhs.mFormat);
	swap(mType, rhs.mType);
	swap(mHasMipmap, rhs.mHasMipmap);
	return *this;
}

Texture::Texture(const Image& img, Renderer& renderer)
	: mHasMipmap{ false }
{
	create(img, renderer);
}

Texture::Texture(std::vector<Image>& imgs, Renderer& renderer)
	: mHasMipmap{ false }
{
	create(imgs, renderer);
}

void Texture::create(const Image& img, Renderer& renderer)
{
	GLint format;
	GLint internalFormat;
	GLenum dataType;
	GLenum compInternalFormat;

	ParseFormat(img.getFormat(), format, internalFormat, dataType, compInternalFormat);

	if (format == 0 &&
		internalFormat == 0 &&
		dataType == 0 &&
		compInternalFormat == 0)
	{
		// Unsupported format 
		return;
	}

	if (mID != 0) 
		clear();
	gl::GenTextures(1, &mID);
	mFormat = img.getFormat();
	mSize = img.mBytes.size();

	if (compInternalFormat != 0)
	{
		// Compressed
		// Type must be set before binding
		mType = Type::COMPRESSED_2D;
		renderer.bindTexture(*this, 0);
		gl::CompressedTexImage2D(gl::TEXTURE_2D, 0, internalFormat, img.getWidth(),
			img.getHeight(), 0, img.mBytes.size(), img.mBytes.data());
	}
	else
	{
		// Uncompressed
		// Type must be set before binding
		mType = Type::UNCOMPRESSED_2D;
		renderer.bindTexture(*this, 0);
		gl::TexImage2D(gl::TEXTURE_2D, 0, internalFormat, img.getWidth(), img.getHeight(),
			0, format, dataType, img.mBytes.data());
	}
}

void Texture::create(std::vector<Image>& imgs, Renderer& renderer)
{
	if (imgs.empty())
		return;

	Image::Format imageFormat{ imgs[0].getFormat() };
	uint32_t width{ imgs[0].getWidth() };
	uint32_t height{ imgs[0].getHeight() };

	GLint format;
	GLint internalFormat;
	GLenum dataType;
	GLenum compInternalFormat;
	
	ParseFormat(imageFormat, format, internalFormat, dataType, compInternalFormat);

	if (format == 0 &&
		internalFormat == 0 &&
		dataType == 0 &&
		compInternalFormat == 0)
	{
		// Unsupported format 
		return;
	}

	if (mID != 0)
		clear();
	gl::GenTextures(1, &mID);
	mSize = imgs[0].mBytes.size() * imgs.size();
	bool isCompressed{ compInternalFormat != 0 };
	mType = isCompressed ? Type::COMPRESSED_2D_ARRAY : Type::UNCOMPRESSED_2D_ARRAY;
	renderer.bindTexture(*this, 0);
	
	if (isCompressed)
		gl::CompressedTexImage3D(gl::TEXTURE_2D_ARRAY, 0, compInternalFormat, width, height, imgs.size(),
			0, 0, nullptr);
	else
		gl::TexImage3D(gl::TEXTURE_2D_ARRAY, 0, internalFormat, width, height, imgs.size(), 0,
			format, dataType, nullptr);

	for (uint32_t i = 0; i < imgs.size(); i++)
	{
		if (imgs[i].getFormat() != imageFormat || imgs[i].getWidth() != width || imgs[i].getHeight() != height)
			return;

		if (isCompressed)
			gl::CompressedTexSubImage3D(gl::TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1,
				compInternalFormat, imgs[i].mBytes.size(), imgs[i].mBytes.data());
		else
			gl::TexSubImage3D(gl::TEXTURE_2D_ARRAY, 0, 0, 0, i, imgs[i].getWidth(),
				imgs[i].getHeight(), 1, format, dataType, imgs[i].mBytes.data());
	}
}

Texture::~Texture()
{
	clear();
}

void Texture::clear()
{
	gl::DeleteTextures(1, &mID);
}

void Texture::generateMipmaps(Renderer & renderer)
{
	renderer.bindTexture(*this, 0);
	gl::GenerateMipmap(gl::TEXTURE_2D);
	mHasMipmap = true;
}

Image::Format Texture::getFormat() const
{
	return mFormat;
}

bool Texture::hasMipmap() const
{
	return mHasMipmap;
}

Texture::Type Texture::getType() const
{
	return mType;
}

std::uint32_t Texture::getSize() const
{
	return mSize;
}

void Texture::ParseFormat(Image::Format imgFormat, GLint &format,
	GLint &internalFormat, GLenum &dataType, GLenum &compressedInternalFormat)
{
	format = 0;
	internalFormat = 0;
	dataType = 0;
	compressedInternalFormat = 0;
	bool noS3TCExt = !gl::exts::var_EXT_texture_compression_s3tc;

	switch (imgFormat)
	{
	// Uncompressed
	case Image::Format::R8:
		format = gl::RED;
		internalFormat = gl::R8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	case Image::Format::RG8:
		format = gl::RG;
		internalFormat = gl::RG8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	case Image::Format::RGB8:
		format = gl::RGB;
		internalFormat = gl::RGB8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	case Image::Format::RGBA8:
		format = gl::RGBA;
		internalFormat = gl::RGBA8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	case Image::Format::R3G3B2:
		format = gl::RGB;
		internalFormat = gl::R3_G3_B2;
		dataType = gl::UNSIGNED_BYTE_2_3_3_REV;
		break;

	case Image::Format::R5G6B5:
		format = gl::RGB;
		internalFormat = gl::RGB8;
		dataType = gl::UNSIGNED_SHORT_5_6_5_REV;
		break;

	case Image::Format::RGBA4:
		format = gl::RGBA;
		internalFormat = gl::RGBA4;
		dataType = gl::UNSIGNED_SHORT_4_4_4_4_REV;
		break;

	case Image::Format::RGB5A1:
		format = gl::RGBA;
		internalFormat = gl::RGB5_A1;
		dataType = gl::UNSIGNED_SHORT_1_5_5_5_REV;
		break;

	case Image::Format::RGB10A2:
		format = gl::RGBA;
		internalFormat = gl::RGB10_A2;
		dataType = gl::UNSIGNED_INT_2_10_10_10_REV;
		break;

	case Image::Format::SRGB8:
		format = gl::RGB;
		internalFormat = gl::SRGB8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	case Image::Format::SRGB8A8:
		format = gl::RGBA;
		internalFormat = gl::SRGB8_ALPHA8;
		dataType = gl::UNSIGNED_BYTE;
		break;

	// Compressed
	case Image::Format::BC1_RGB:
		if (noS3TCExt)
			return;

		compressedInternalFormat = gl::COMPRESSED_RGB_S3TC_DXT1_EXT;
		break;

	case Image::Format::BC1_RGBA:
		if (noS3TCExt)
			return;

		compressedInternalFormat = gl::COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;

	case Image::Format::BC2_RGBA:
		if (noS3TCExt)
			return;

		compressedInternalFormat = gl::COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;

	case Image::Format::BC3_RGBA:
		if (noS3TCExt)
			return;

		compressedInternalFormat = gl::COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;

	case Image::Format::BC4_R:
		compressedInternalFormat = gl::COMPRESSED_RED_RGTC1;
		break;

	case Image::Format::BC4_SIGNED_R:
		compressedInternalFormat = gl::COMPRESSED_SIGNED_RED_RGTC1;
		break;

	case Image::Format::BC5_RG:
		compressedInternalFormat = gl::COMPRESSED_RG_RGTC2;
		break;

	case Image::Format::BC5_SIGNED_RG:
		compressedInternalFormat = gl::COMPRESSED_SIGNED_RG_RGTC2;
		break;

	default:
		return;
	}
}


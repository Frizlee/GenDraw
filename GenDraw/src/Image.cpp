#include "Image.hpp"
using namespace std;

Image::Image() 
	: mFormat{ Format::UNKNOWN }
{
}

Image::Image(const Image &lhs)
{
	mBytes = lhs.mBytes;
	mWidth = lhs.mWidth;
	mHeight = lhs.mHeight;
	mFormat = lhs.mFormat;
}

Image::Image(Image &&rhs)
{
	swap(mBytes, rhs.mBytes);
	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;
	mFormat = rhs.mFormat;
}

Image& Image::operator=(Image cas)
{
	// Copy-and-swap idiom
	swap(mBytes, cas.mBytes);
	mWidth = cas.mWidth;
	mHeight = cas.mHeight;
	mFormat = cas.mFormat;
	return *this;
}

Image& Image::operator=(Image &&rhs)
{
	swap(mBytes, rhs.mBytes);
	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;
	mFormat = rhs.mFormat;
	return *this;
}

Image::Image(uint32_t width, uint32_t height, Format format, const std::vector<uint8_t> &bytes)
	: mFormat{ Format::UNKNOWN }
{
	create(width, height, format, bytes);	
}

Image::Image(uint32_t width, uint32_t height, Format format, std::vector<uint8_t> &&bytes)
	: mFormat{ Format::UNKNOWN }
{
	create(width, height, format, move(bytes));
}

Image::Image(std::uint32_t width, std::uint32_t height, Format format)
{
	create(width, height, format);
}

void Image::create(uint32_t width, uint32_t height, Format format, const std::vector<uint8_t> &bytes)
{
	mWidth = width;
	mHeight = height;
	mFormat = format;
	mBytes = bytes;
}

void Image::create(uint32_t width, uint32_t height, Format format, std::vector<uint8_t> &&bytes)
{
	mWidth = width;
	mHeight = height;
	mFormat = format;
	swap(mBytes, bytes);
}

void Image::create(std::uint32_t width, std::uint32_t height, Format format)
{
	mWidth = width;
	mHeight = height;
	mFormat = format;
	mBytes.resize(width * height * getBytesPerPixel());
}

Image::~Image()
{
}

void Image::clear()
{
	mFormat = Format::UNKNOWN;
	mBytes.clear();
	mWidth = 0;
	mHeight = 0;
}

Image::Format Image::getFormat() const
{
	return mFormat;
}

uint32_t Image::getWidth() const
{
	return mWidth;
}

uint32_t Image::getHeight() const
{
	return mHeight;
}

vector<uint8_t> Image::getPixel(uint32_t x, uint32_t y) const
{
	uint8_t bytesPerPixel = getBytesPerPixel();

	if (bytesPerPixel == 0 || x > mWidth || y > mHeight || isCompressed())
	{
		// TODO: Error handling, unsuported format or not created propertly
		return vector<uint8_t>{};
	}

	vector<uint8_t> bytes;
	bytes.resize(bytesPerPixel);

	uint32_t row = y * mWidth * bytesPerPixel;
	uint32_t col = x * bytesPerPixel;

	copy(begin(mBytes) + row + col, begin(mBytes) + row + col + bytesPerPixel, begin(bytes));

	return move(bytes);
}

bool Image::isCompressed() const
{
	switch (mFormat)
	{
	case Format::BC1_RGB:
	case Format::BC1_RGBA:
	case Format::BC2_RGBA:
	case Format::BC3_RGBA:
	case Format::BC4_R:
	case Format::BC4_SIGNED_R:
	case Format::BC5_RG:
	case Format::BC5_SIGNED_RG:
		return true;
	}

	return false;
}

uint8_t Image::getBytesPerPixel() const
{
	return CalculateBytesPerPixel(mFormat);
}

std::uint8_t Image::CalculateBytesPerPixel(Format format)
{
	switch (format)
	{
	case Format::R8:
	case Format::R3G3B2:
		return 1;

	case Format::RG8:
	case Format::R5G6B5:
	case Format::RGB5A1:
	case Format::RGBA4:
		return 2;

	case Format::RGB8:
	case Format::SRGB8:
		return 3;

	case Format::RGBA8:
	case Format::RGB10A2:
	case Format::SRGB8A8:
		return 4;

	// Block size
	case Format::BC1_RGB:
	case Format::BC4_SIGNED_R:
	case Format::BC4_R:
		return 8;

	case Format::BC2_RGBA:
	case Format::BC3_RGBA:
	case Format::BC5_SIGNED_RG:
	case Format::BC5_RG:
		return 16;

	case Format::UNKNOWN:
	default:
		return 0;
	}
}

void Image::setPixel(uint32_t x, uint32_t y, const vector<uint8_t> &bytes)
{
	uint8_t bytesPerPixel = getBytesPerPixel();

	if (bytesPerPixel == 0 || x > mWidth || y > mHeight || isCompressed())
	{
		// TODO: Error handling, unsuported format or not created propertly
		return;
	}

	if (bytes.size() != bytesPerPixel)
	{
		// TODO: Error handling, bad pixel data
		return;
	}
	
	uint32_t row = y * mWidth * bytesPerPixel;
	uint32_t col = x * bytesPerPixel;

	copy(begin(bytes), end(bytes), begin(mBytes) + row + col);
}

std::vector<std::uint8_t>& Image::getBytes()
{
	return mBytes;
}

void Image::flipVerticaly()
{
	uint8_t bytesPerPixel = getBytesPerPixel();
	uint8_t flipByte;

	if (bytesPerPixel == 0)
	{
		// TODO: Error handling, unsupported format or not created propertly
		return;
	}
	
	for (uint32_t v = 0; v < mHeight / 2; v++)
	{
		uint32_t row1 = v * mWidth * bytesPerPixel;
		uint32_t row2 = (mHeight - v - 1) * mWidth * bytesPerPixel;

		for (unsigned int u = 0; u < mWidth; u++)
		{
			uint32_t pixel1 = row1 + u * bytesPerPixel;
			uint32_t pixel2 = row2 + u * bytesPerPixel;

			for (uint8_t i = 0; i < bytesPerPixel; i++)
			{
				flipByte = mBytes.at(pixel1 + i);
				mBytes.at(pixel1 + i) = mBytes.at(pixel2 + i);
				mBytes.at(pixel2 + i) = flipByte;
			}
		}
	}
}

std::uint32_t Image::getSize() const
{
	return mBytes.size();
}


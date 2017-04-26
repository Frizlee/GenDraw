#include "PngCodec.hpp"
#include "Image.hpp"
using namespace std;


PngCodec::PngCodec()
{
}

PngCodec::~PngCodec()
{
}

void PngCodec::decode(istream &input, Resource &res)
{
	try
	{
		const uint32_t SIGNATURE_LENGTH = 8;
		png_structp pngStruct;
		png_infop pngInfo;
		int32_t bitDepth = 0;
		int32_t colorType = -1;
		uint32_t width;
		uint32_t height;
		Image::Format format;
		vector<uint8_t> bytes;
		uint8_t bytesPerPixel;
		Image &imgRes = dynamic_cast<Image&>(res);

		if ((pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr)) == nullptr)
		{
			// TODO: Error handling
			return;
		}

		if ((pngInfo = png_create_info_struct(pngStruct)) == nullptr)
		{
			// TODO: Error handling
			return png_destroy_read_struct(&pngStruct, nullptr, nullptr);;
		}

		png_set_read_fn(pngStruct, &input, PngReadCallback);
		png_read_info(pngStruct, pngInfo);

		if (png_check_sig(png_get_signature(pngStruct, pngInfo), SIGNATURE_LENGTH) == false)
		{
			// TODO: Error handling
			return png_destroy_read_struct(&pngStruct, &pngInfo, nullptr);;
		}

		if (png_get_IHDR(pngStruct, pngInfo, &width, &height,
			&bitDepth, &colorType, nullptr, nullptr, nullptr) != 1)
		{
			// TODO: Error handling
			return png_destroy_read_struct(&pngStruct, &pngInfo, nullptr);
		}

		switch (colorType)
		{
		case PNG_COLOR_TYPE_PALETTE:
			if (bitDepth == 8)
				format = Image::Format::R;
			else
				format = Image::Format::RGB;
			break;

		case PNG_COLOR_TYPE_RGB:
			format = Image::Format::RGB;
			break;

		case PNG_COLOR_TYPE_RGBA:
			format = Image::Format::RGBA;
			break;

		default:
			// TODO: Error handling
			return png_destroy_read_struct(&pngStruct, &pngInfo, nullptr);
		}
		
		bytesPerPixel = Image::CalculateBytesPerPixel(format);
		bytes.resize(width * height * bytesPerPixel);

		// Before 0 will be INT_MAX not -1 
		for (uint32_t i = height; i > 0; i--)
			png_read_row(pngStruct, &bytes.at((i - 1) * width * bytesPerPixel), nullptr);

		imgRes.create(width, height, format, move(bytes));

		return png_destroy_read_struct(&pngStruct, &pngInfo, nullptr);
	}
	catch (const std::bad_cast &e)
	{
		// TODO: Error handling
	}
}

void PngCodec::encode(ostream &output, Resource &res)
{
	return;
}

void PngCodec::PngReadCallback(png_structp pngPtr, png_bytep outBytes,
	png_size_t byteCountToRead)
{
	istream *inputStream = reinterpret_cast<istream*>(png_get_io_ptr(pngPtr));

	inputStream->read(reinterpret_cast<char*>(outBytes), byteCountToRead);

	if (!(*inputStream))
		png_error(pngPtr, "Couldn't read from stream.");
}


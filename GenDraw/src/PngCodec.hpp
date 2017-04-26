#pragma once
#include "Codec.hpp"
#include <png.h>

class PngCodec : public Codec
{
public:
	PngCodec();
	~PngCodec();
	
	void decode(std::istream &input, Resource &res);
	void encode(std::ostream &output, Resource &res);
	
private:
	static void PngReadCallback(png_structp pngPtr, png_bytep outBytes, png_size_t byteCountToRead);
};




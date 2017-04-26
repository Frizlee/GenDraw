#include "VertexLayout.hpp"
using namespace std;

VertexFormat::VertexFormat()
	: empty{ true }
{
}

VertexFormat::VertexFormat(GLint size, GLenum type, GLboolean normalized, GLsizei stride, 
	uint32_t pointer)
	: empty{ false }, size { size }, type{ type }, normalized{ normalized }, stride{ stride }, 
		pointer{ pointer }
{
}

VertexLayout::VertexLayout()
{
}

VertexLayout::~VertexLayout()
{
}

const vector<VertexFormat>& VertexLayout::getFormats()
{
	return mFormats;
}

TextVertexLayout::Data::Data()
{
}

TextVertexLayout::Data::Data(float x, float y, float u, float v)
	: x{ x }, y{ y }, u{ u }, v{ v }
{
}

TextVertexLayout::TextVertexLayout()
{
	mFormats = {
		VertexFormat{ 2, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, x) },
		VertexFormat{ 2, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, u) } 
	};
}

TextVertexLayout::~TextVertexLayout()
{
}

uint32_t TextVertexLayout::Size()
{
	return sizeof(Data);
}

TextureVertexLayout::Data::Data()
{
}

TextureVertexLayout::Data::Data(float x, float y, float z, float u, float v)
	: x{ x }, y{ y }, z{ z }, u{ u }, v{ v }
{
}

TextureVertexLayout::TextureVertexLayout()
{
	mFormats = {
		VertexFormat{ 3, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, x) },
		VertexFormat{ 2, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, u) }
	};
}

TextureVertexLayout::~TextureVertexLayout()
{
}

uint32_t TextureVertexLayout::Size()
{
	return sizeof(Data);
}

ColorVertexLayout::Data::Data()
{
}

ColorVertexLayout::Data::Data(float x, float y, float z, uint32_t color)
	: x{ x }, y{ y }, z{ z }, color{ color }
{
}

ColorVertexLayout::ColorVertexLayout()
{
	mFormats = {
		VertexFormat{ 3, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, x) },
		VertexFormat{ 4, gl::UNSIGNED_BYTE, gl::TRUE_, static_cast<GLsizei>(Size()), 
			offsetof(Data, color) }
	};
}

ColorVertexLayout::~ColorVertexLayout()
{
}

uint32_t ColorVertexLayout::Size()
{
	return sizeof(Data);
}

PositionVertexLayout::Data::Data()
{
}

PositionVertexLayout::Data::Data(float x, float y, float z)
	: x{ x }, y{ y }, z{ z }
{
}

PositionVertexLayout::PositionVertexLayout()
{
	mFormats = {
		VertexFormat{ 3, gl::FLOAT, gl::FALSE_, static_cast<GLsizei>(Size()), offsetof(Data, x) }
	};
}

PositionVertexLayout::~PositionVertexLayout()
{
}

std::uint32_t PositionVertexLayout::Size()
{
	return sizeof(Data);
}

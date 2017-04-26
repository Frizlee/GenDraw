#include "VertexArray.hpp"
#include "GpuBuffer.hpp"
using namespace std;

VertexArray::VertexArray()
	: mVertexAttribCount{ 0 }, mInstanceAttribCount{ 0 }
{
}

VertexArray::VertexArray(VertexArray &&rhs)
	: GpuResource(move(rhs))
{
}

VertexArray& VertexArray::operator=(VertexArray &&rhs)
{
	GpuResource::operator=(move(rhs));

	return *this;
}

VertexArray::~VertexArray()
{
	gl::DeleteVertexArrays(1, &mID);
}

std::weak_ptr<GpuBuffer> VertexArray::getBuffer()
{
	return mBuffer;
}

std::weak_ptr<GpuBuffer> VertexArray::getInstanceBuffer()
{
	return mInstanceBuffer;
}

std::uint32_t VertexArray::getVertexSize() const
{
	return mBuffer->getEntitySize();
}

void VertexArray::clear()
{
}

std::uint32_t VertexArray::getSize() const
{
	return 0;
}


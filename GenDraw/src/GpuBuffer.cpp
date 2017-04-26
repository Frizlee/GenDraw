#include "GpuBuffer.hpp"
using namespace std;

GpuBuffer::GpuBuffer(GpuBufferType type)
	: mType{ type }
{
}

GpuBuffer::GpuBuffer(GpuBuffer &&rhs)
	: GpuResource(move(rhs))
{
	swap(mType, rhs.mType);
}

GpuBuffer& GpuBuffer::operator=(GpuBuffer &&rhs)
{
	GpuResource::operator=(move(rhs));
	swap(mType, rhs.mType);
	return *this;
}

GpuBuffer::~GpuBuffer()
{
}

GpuBufferType GpuBuffer::getType() const
{
	return mType;
}


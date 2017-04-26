#pragma once
#include "Prerequisites.hpp"
#include "GpuResource.hpp"

enum class GpuBufferType
{
	VERTEX_BUFFER,
	COPY_WRITE_BUFFER,
	COPY_READ_BUFFER
};

class GpuBuffer : public GpuResource
{
public:
	GpuBuffer() = delete;
	GpuBuffer(GpuBufferType type);

	GpuBuffer(const GpuBuffer &lhs) = delete;
	GpuBuffer(GpuBuffer &&rhs);
	GpuBuffer& operator=(const GpuBuffer &lhs) = delete;
	GpuBuffer& operator=(GpuBuffer &&rhs);

	virtual ~GpuBuffer() = 0;

	virtual std::uint32_t getEntitySize() const = 0;;
	GpuBufferType getType() const;

private:
	GpuBufferType mType;
	
};


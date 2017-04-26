#pragma once
#include "Prerequisites.hpp"
#include "gl_core_3_3.hpp"
#include "Resource.hpp"

class GpuResource : public Resource
{
	friend class Renderer;

public:	
	GpuResource();
	
	// Can't be copied
	GpuResource(const GpuResource& lhs) = delete;
	GpuResource(GpuResource&& rhs);
	GpuResource& operator=(const GpuResource& lhs) = delete;
	GpuResource& operator=(GpuResource&& rhs);

	virtual ~GpuResource() = 0;

protected:
	GLuint mID;

private:
	GLuint getID() const;
};


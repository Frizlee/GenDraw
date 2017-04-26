#include "GpuResource.hpp"
using namespace std;

GpuResource::GpuResource()
	: mID{ 0 }
{
}

GpuResource::GpuResource(GpuResource&& rhs)
{
	swap(mID, rhs.mID);
}

GpuResource& GpuResource::operator=(GpuResource&& rhs)
{
	swap(mID, rhs.mID);
	return *this;
}

GpuResource::~GpuResource()
{
}

GLuint GpuResource::getID() const
{
	return mID;
}


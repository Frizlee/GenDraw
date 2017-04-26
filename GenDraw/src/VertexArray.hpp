#pragma once
#include "Prerequisites.hpp"
#include "GpuResource.hpp"
#include "gl_core_3_3.hpp"

class VertexLayout;
class Renderer;
class GpuBuffer;
template <typename T>
class VertexBuffer;

class VertexArray : public GpuResource
{
public:
	VertexArray();

	// Copy/Move constructors and assignments
	VertexArray(const VertexArray& lhs) = delete;
	VertexArray(VertexArray&& rhs);
	VertexArray& operator=(VertexArray cas) = delete;
	VertexArray& operator=(VertexArray&& rhs);
	
	template <typename T>
	VertexArray(std::shared_ptr<VertexBuffer<T>> buffer, Renderer& renderer);
	template <typename T>
	void create(std::shared_ptr<VertexBuffer<T>> buffer, Renderer& renderer);

	~VertexArray();

	template <typename T>
	void addInstanceBuffer(std::shared_ptr<VertexBuffer<T>> buffer, Renderer& renderer);

	std::weak_ptr<GpuBuffer> getBuffer();
	std::weak_ptr<GpuBuffer> getInstanceBuffer();
	std::uint32_t getVertexSize() const;

	void clear() override;
	std::uint32_t getSize() const override;

private:
	std::shared_ptr<GpuBuffer> mBuffer;
	std::shared_ptr<GpuBuffer> mInstanceBuffer;
	std::uint32_t mVertexAttribCount;
	std::uint32_t mInstanceAttribCount;

};


template <typename T>
VertexArray::VertexArray(std::shared_ptr<VertexBuffer<T>> buffer, Renderer& renderer)
	: mVertexAttribCount{ 0 }, mInstanceAttribCount{ 0 }
{
	create(buffer, renderer);
}

template <typename T>
void VertexArray::create(std::shared_ptr<VertexBuffer<T>> buffer, Renderer& renderer)
{
	mBuffer = std::dynamic_pointer_cast<GpuBuffer>(buffer);

	gl::GenVertexArrays(1, &mID);
	T layout;
	renderer.bindVertexArray(*this);
	renderer.bindBuffer(*buffer);
	std::uint32_t i = 0;

	for (auto it : layout.getFormats())
	{
		if (it.empty)
			gl::DisableVertexAttribArray(i);
		else
		{
			gl::EnableVertexAttribArray(i);
			gl::VertexAttribPointer(i, it.size, it.type, it.normalized, it.stride,
				reinterpret_cast<void*>(it.pointer));
		}

		i++;
	}

	mVertexAttribCount = i;
}

template<typename T>
inline void VertexArray::addInstanceBuffer(std::shared_ptr<VertexBuffer<T>> buffer, 
	Renderer& renderer)
{
	mInstanceBuffer = std::dynamic_pointer_cast<GpuBuffer>(buffer);

	T layout;
	renderer.bindVertexArray(*this);
	renderer.bindBuffer(*buffer);

	std::uint32_t i = mVertexAttribCount;


	for (auto it : layout.getFormats())
	{
		if (it.empty)
			gl::DisableVertexAttribArray(i);
		else
		{
			gl::EnableVertexAttribArray(i);
			gl::VertexAttribPointer(i, it.size, it.type, it.normalized, it.stride,
				reinterpret_cast<void*>(it.pointer));
			gl::VertexAttribDivisor(i, 1);
		}

		i++;
	}
	mInstanceAttribCount = i - mVertexAttribCount;
}


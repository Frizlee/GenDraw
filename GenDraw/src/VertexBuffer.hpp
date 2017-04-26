#pragma once
#include "Prerequisites.hpp"
#include "GpuBuffer.hpp"
#include "VertexLayout.hpp"
#include "Renderer.hpp"

template <typename T>
class VertexBuffer : public GpuBuffer
{
public:
	typedef T Layout;

	VertexBuffer();

	VertexBuffer<T>(const VertexBuffer<T>& lhs) = delete;
	VertexBuffer<T>(VertexBuffer<T>&& rhs);
	VertexBuffer<T>& operator=(VertexBuffer<T> cas) = delete;
	VertexBuffer<T>& operator=(VertexBuffer<T>&& rhs);

	VertexBuffer(std::uint32_t elements, Renderer &renderer, bool dynamic = false);
	void create(std::uint32_t elements, Renderer &renderer, bool dynamic = false);

	~VertexBuffer();
	void clear() override;

	std::uint32_t getSize() const override;
	std::uint32_t getEntitySize() const override;
	std::uint32_t getElementsCount() const;

	std::uint32_t add(const std::vector<typename T::Data>& lhsData, Renderer& renderer);
	std::uint32_t add(const std::vector<typename T::Data>& lhsData, std::uint32_t position, 
		Renderer& renderer);
	std::uint32_t add(std::vector<typename T::Data>&& rhsData, Renderer& renderer);
	std::uint32_t add(std::vector<typename T::Data>&& rhsData, std::uint32_t position,
		Renderer& renderer);

private:
	std::uint32_t mPosition;
	std::uint32_t mElements;
};

template<typename T>
inline VertexBuffer<T>::VertexBuffer()
	: GpuBuffer(GpuBufferType::VERTEX_BUFFER), mPosition{ 0 }
{
	static_assert(std::is_base_of<VertexLayout, T>(), "VertexBuffer must operate on VertexLayout");
}

template<typename T>
inline VertexBuffer<T>::VertexBuffer(VertexBuffer<T>&& rhs)
	: GpuBuffer(move(rhs))
{
	swap(mPosition, rhs.mPosition);
}

template<typename T>
inline VertexBuffer<T>& VertexBuffer<T>::operator=(VertexBuffer<T>&& rhs)
{
	GpuBuffer::operator=(move(rhs));
	swap(mPosition, rhs.mPosition);
	return *this;
}

template<typename T>
inline VertexBuffer<T>::VertexBuffer(std::uint32_t elements, Renderer& renderer, bool dynamic)
	: GpuBuffer(GpuBufferType::VERTEX_BUFFER), mPosition{ 0 }
{
	static_assert(std::is_base_of<VertexLayout, T>(), "VertexBuffer must operate on VertexLayout");
	create(elements, renderer, dynamic);
}

template<typename T>
inline void VertexBuffer<T>::create(std::uint32_t elements, Renderer& renderer, bool dynamic)
{
	gl::GenBuffers(1, &mID);
	renderer.bindBuffer(*this);
	mElements = elements;
	gl::BufferData(gl::ARRAY_BUFFER, elements * T::Size(), nullptr, 
		dynamic ? gl::DYNAMIC_DRAW : gl::STATIC_DRAW);
}

template<typename T>
inline VertexBuffer<T>::~VertexBuffer()
{
	gl::DeleteBuffers(1, &mID);
}

template<typename T>
inline void VertexBuffer<T>::clear()
{
	mPosition = 0;
}

template<typename T>
inline std::uint32_t VertexBuffer<T>::getSize() const
{
	return mElements * T::Size();
}

template<typename T>
inline std::uint32_t VertexBuffer<T>::getEntitySize() const
{
	return T::Size();
}

template<typename T>
inline std::uint32_t VertexBuffer<T>::getElementsCount() const
{
	return mElements;
}

template<typename T>
inline std::uint32_t VertexBuffer<T>::add(const std::vector<typename T::Data>& lhsData, 
	Renderer& renderer)
{
	std::uint32_t currentPos = mPosition;

	renderer.bindBuffer(*this);
	gl::BufferSubData(gl::ARRAY_BUFFER, mPosition, lhsData.size() * T::Size(), lhsData.data());
	mPosition += lhsData.size() * T::Size();

	return currentPos;
}

template<typename T>
inline std::uint32_t VertexBuffer<T>::add(const std::vector<typename T::Data>& lhsData,
	std::uint32_t position, Renderer& renderer)
{
	renderer.bindBuffer(*this);
	gl::BufferSubData(gl::ARRAY_BUFFER, position, lhsData.size() * T::Size(), lhsData.data());

	return position;
}

template<typename T>
inline std::uint32_t VertexBuffer<T>::add(std::vector<typename T::Data>&& rhsData, 
	Renderer& renderer)
{
	std::uint32_t currentPos = mPosition;

	renderer.bindBuffer(*this);
	gl::BufferSubData(gl::ARRAY_BUFFER, mPosition, rhsData.size() * T::Size(), rhsData.data());
	mPosition += rhsData.size() * T::Size();

	return currentPos;
}

template<typename T>
inline std::uint32_t VertexBuffer<T>::add(std::vector<typename T::Data>&& rhsData, 
	std::uint32_t position, Renderer& renderer)
{
	renderer.bindBuffer(*this);
	gl::BufferSubData(gl::ARRAY_BUFFER, position, rhsData.size() * T::Size(), rhsData.data());

	return position;
}




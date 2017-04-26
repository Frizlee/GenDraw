#include "ImageAtlas.hpp"
using namespace std;

ImageAtlas::ImageAtlas() 
	: mRoot{ make_unique<Node>() }
{
}

ImageAtlas::ImageAtlas(ImageAtlas &&rhs) 
	: Image(move(rhs))
{
	swap(mRoot, rhs.mRoot);
}

ImageAtlas& ImageAtlas::operator=(ImageAtlas &&rhs)
{
	Image(move(rhs));
	swap(mRoot, rhs.mRoot);

	return *this;
}

ImageAtlas::ImageAtlas(std::uint32_t width, uint32_t height, Format format, const vector<uint8_t> &bytes)
	: Image(width, height, format, bytes), mRoot{ make_unique<Node>() }
{
	mRoot->mRect.x = 0;
	mRoot->mRect.y = 0;
	mRoot->mRect.z = width;
	mRoot->mRect.w = height;
}

ImageAtlas::ImageAtlas(uint32_t width, uint32_t height, Format format, vector<uint8_t> &&bytes)
	: Image(width, height, format, move(bytes)), mRoot{ make_unique<Node>() }
{
	mRoot->mRect.x = 0;
	mRoot->mRect.y = 0;
	mRoot->mRect.z = width;
	mRoot->mRect.w = height;
}

ImageAtlas::ImageAtlas(uint32_t width, uint32_t height, Format format)
	: Image(width, height, format), mRoot{ make_unique<Node>() }
{
	mRoot->mRect.x = 0;
	mRoot->mRect.y = 0;
	mRoot->mRect.z = width;
	mRoot->mRect.w = height;
}

void ImageAtlas::create(uint32_t width, uint32_t height, Format format, const vector<uint8_t> &bytes)
{
	Image::create(width, height, format, bytes);

	mRoot->mRect.x = 0;
	mRoot->mRect.y = 0;
	mRoot->mRect.z = width;
	mRoot->mRect.w = height;
}

void ImageAtlas::create(uint32_t width, uint32_t height, Format format, vector<uint8_t> &&bytes)
{
	Image::create(width, height, format, move(bytes));

	mRoot->mRect.x = 0;
	mRoot->mRect.y = 0;
	mRoot->mRect.z = width;
	mRoot->mRect.w = height;
}

void ImageAtlas::create(std::uint32_t width, std::uint32_t height, Format format)
{
	Image::create(width, height, format);

	mRoot->mRect.x = 0;
	mRoot->mRect.y = 0;
	mRoot->mRect.z = width;
	mRoot->mRect.w = height;
}

ImageAtlas::~ImageAtlas()
{
}

void ImageAtlas::clear()
{
	mRoot.release();
	Image::clear();
}

ImageAtlas::Rect ImageAtlas::insert(Image &img)
{
	if (img.getFormat() != getFormat())
	{
		// TODO: Error handling.
		return Rect(0, 0, 0, 0);
	}

	Rect rc = mRoot->insert(img.getWidth(), img.getHeight());

	if (rc == Rect(0, 0, 0, 0))
	{
		// TODO: Error handling.
		return Rect(0, 0, 0, 0);
	}

	for (uint32_t i = 0; i < img.getHeight(); i++)
	{
		copy(begin(img.mBytes) + i * img.getWidth(),
			begin(img.mBytes) + (i + 1) * img.getWidth(),
			begin(mBytes) + (rc.y + i) * getWidth() + rc.x);
	}

	return rc;
}

ImageAtlas::Node::Node()
	: mFree{ true }
{
}

ImageAtlas::Node::Node(Node &&rhs)
{
	swap(mChilds[0], rhs.mChilds[0]);
	swap(mChilds[1], rhs.mChilds[1]);
	swap(mRect, rhs.mRect);
	mFree = rhs.mFree;
}

ImageAtlas::Node& ImageAtlas::Node::operator=(Node && rhs)
{
	swap(mChilds[0], rhs.mChilds[0]);
	swap(mChilds[1], rhs.mChilds[1]);
	swap(mRect, rhs.mRect);
	mFree = rhs.mFree;

	return *this;
}

void * ImageAtlas::Node::operator new(size_t i)
{
	return _aligned_malloc(i, 16);
}

void ImageAtlas::Node::operator delete(void *p)
{
	_aligned_free(p);
}

ImageAtlas::Rect ImageAtlas::Node::insert(uint32_t width, uint32_t height)
{
	if (mChilds[0] != nullptr && mChilds[1] != nullptr)
	{
		Rect rc = mChilds[0]->insert(width, height);

		if (rc != Rect(0, 0, 0, 0))
			return rc;
		else
			return mChilds[1]->insert(width, height);

	}
	else
	{
		if (!mFree)
			return Rect(0, 0, 0, 0);

		if (mRect.z < width || mRect.w < height)
			return Rect(0, 0, 0, 0);

		if (mRect.z == width && mRect.w == height)
		{
			mFree = false;
			return mRect;
		}

		mChilds[0] = make_unique<Node>();
		mChilds[1] = make_unique<Node>();

		uint32_t dw = mRect.z - width;
		uint32_t dh = mRect.w - height;

		if (dw > dh)
		{
			mChilds[0]->mRect = Rect(mRect.x, mRect.y, width, mRect.w);
			mChilds[1]->mRect = Rect(mRect.x + width, mRect.y, mRect.z - width, mRect.w);
		}
		else
		{
			mChilds[0]->mRect = Rect(mRect.x, mRect.y, mRect.z, height);
			mChilds[1]->mRect = Rect(mRect.x, mRect.y + height, mRect.z, mRect.w - height);
		}

		return mChilds[0]->insert(width, height);
	}
}


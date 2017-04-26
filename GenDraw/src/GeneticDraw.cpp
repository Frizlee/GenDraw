#include "GeneticDraw.hpp"
using namespace std;

GeneticDraw::GeneticDraw(size_t specCount, size_t bestCount, uint32_t width, uint32_t height,
	std::shared_ptr<Image> pattern)
	: mSpecCount{ specCount }, mBestCount{ bestCount }, mPattern{ pattern }
{
	mWidth = width;
	mHeight = height;

	for (size_t i = 0; i < mSpecCount; i++)
		mSpecimens.push_back(Image(mWidth, mHeight, Image::Format::R));

	for (size_t i = 0; i < mBestCount; i++)
		mBests.push_back(Image(mWidth, mHeight, Image::Format::R));
}


GeneticDraw::~GeneticDraw()
{
}

void GeneticDraw::mutate()
{
	uniform_int_distribution<size_t> xDist{ 0, mWidth };
	uniform_int_distribution<size_t> yDist{ 0, mHeight };
	uniform_int_distribution<uint32_t> lumDist{ 0, 255 };

	for (auto& image : mSpecimens)
	{
		vector<size_t> x, y;
		uint32_t lum;
		x.push_back(xDist(mRandomDev));
		x.push_back(xDist(mRandomDev));
		y.push_back(yDist(mRandomDev));
		y.push_back(yDist(mRandomDev));
		lum = lumDist(mRandomDev);

		sort(begin(x), end(x));
		sort(begin(y), end(y));

		for (uint32_t i = x[0]; i < x[1]; i++)
			for (uint32_t j = y[0]; j < y[1]; j++)
				image.getBytes()[j * mWidth + i] = (image.getBytes()[j * mWidth + i] + lum) / 2;
	}
}

void GeneticDraw::select()
{
	mScores.clear();

	for (int i = 0; i < mSpecCount; i++)
	{
		Score s;
		s.index = i;
		s.score = 0.0;

		for (int x = 0; x < mWidth; x++)
		{
			for (int y = 0; y < mHeight; y++)
			{
				double pixel = mSpecimens[i].getBytes()[y * mWidth + x];
				double patternPixel = mPattern->getBytes()[y * mWidth + x];

				s.score += (pixel - patternPixel) * (pixel - patternPixel);
			}
		}

		mScores.push_back(s);
	}

	sort(begin(mScores), end(mScores), [](const Score& a, const Score& b) {
		return a.score < b.score;
	});
}

void GeneticDraw::dumpBest()
{
	for (size_t i = 0; i < mBestCount; i++)
		mBests[i] = mSpecimens[mScores[i].index];
}

Image& GeneticDraw::pickRandomBest()
{
	uniform_int_distribution<size_t> indexDist{ 0, mBestCount - 1 };
	return mBests[indexDist(mRandomDev)];
}

void GeneticDraw::prepareNextGen()
{
	for (int i = 0; i < mSpecCount; i++)
		mSpecimens[i] = mBests[i % mBestCount];
}

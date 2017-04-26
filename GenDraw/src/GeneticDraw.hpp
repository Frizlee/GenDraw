#pragma once
#include "Prerequisites.hpp"
#include "Image.hpp"

class GeneticDraw
{
public:
	struct Score
	{
		size_t index;
		double score;
	};

	GeneticDraw(size_t specCount, size_t bestCount, std::uint32_t width, std::uint32_t height,
		std::shared_ptr<Image> pattern);
	~GeneticDraw();

	void mutate();
	void select();
	void dumpBest();
	Image& pickRandomBest();
	void prepareNextGen();
	
private:
	size_t mSpecCount;
	size_t mBestCount;
	std::shared_ptr<Image> mPattern;
	std::random_device mRandomDev;
	std::uint32_t mWidth;
	std::uint32_t mHeight;
	std::vector<Image> mSpecimens;
	std::vector<Image> mBests;
	std::vector<Score> mScores;
};


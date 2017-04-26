#pragma once
#include "Prerequisites.hpp"

// Move it somewhere
struct memorybuf : std::streambuf
{
	memorybuf(char *base, std::ptrdiff_t size)
	{
		setg(base, base, base + size);
	}
};


class Codec
{
public:
	Codec();
	virtual ~Codec() = 0;

	virtual void decode(std::istream &input, class Resource &res) = 0;
	virtual void encode(std::ostream &output, class Resource &res) = 0;

private:

};




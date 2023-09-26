#pragma once
#include "Classes/script/types.hpp"

#pragma pack(push, 1)
struct ImageDimensions
{
	int x, y;
	ImageDimensions() { x = y = 0; }
	ImageDimensions(int _x, int _y) { x = _x; y = _y; }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Color
{
	std::uint8_t r{ 255 };
	std::uint8_t g{ 255 };
	std::uint8_t b{ 255 };
	std::uint8_t a{ 255 };
};
#pragma pack(pop)
#pragma once

#include <ccTypes.h>

using namespace cocos2d;
class RGB2HSV
{
public:
	// h = r, s = g, v = b
	static Color3B rgb2hsv(Color3B rgb);

	// h = r, s = g, v = b
	static Color3B hsv2rgb(Color3B hsv);

};


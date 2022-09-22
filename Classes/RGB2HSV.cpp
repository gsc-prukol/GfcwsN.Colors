#include "RGB2HSV.h"

Color3B RGB2HSV::rgb2hsv(Color3B rgb)
{
    Color3B hsv;
    uint8_t rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.b = rgbMax;
    if (hsv.b == 0)
    {
        hsv.r = 0;
        hsv.g = 0;
        return hsv;
    }

    hsv.g = 255 * long(rgbMax - rgbMin) / hsv.b;
    if (hsv.g == 0)
    {
        hsv.r = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.r = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.r = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.r = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

Color3B RGB2HSV::hsv2rgb(Color3B hsv)
{
    Color3B rgb;
    uint8_t region, remainder, p, q, t;

    if (hsv.g == 0)
    {
        rgb.r = hsv.b;
        rgb.g = hsv.b;
        rgb.b = hsv.b;
        return rgb;
    }

    region = hsv.r / 43;
    remainder = (hsv.r - (region * 43)) * 6;

    p = (hsv.b * (255 - hsv.g)) >> 8;
    q = (hsv.b * (255 - ((hsv.g * remainder) >> 8))) >> 8;
    t = (hsv.b * (255 - ((hsv.g * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
    case 0:
        rgb.r = hsv.b; rgb.g = t; rgb.b = p;
        break;
    case 1:
        rgb.r = q; rgb.g = hsv.b; rgb.b = p;
        break;
    case 2:
        rgb.r = p; rgb.g = hsv.b; rgb.b = t;
        break;
    case 3:
        rgb.r = p; rgb.g = q; rgb.b = hsv.b;
        break;
    case 4:
        rgb.r = t; rgb.g = p; rgb.b = hsv.b;
        break;
    default:
        rgb.r = hsv.b; rgb.g = p; rgb.b = q;
        break;
    }

    return rgb;
}

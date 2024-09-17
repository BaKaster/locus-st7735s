//
// Types.cpp
//
// Created on: Jan 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <fonts/font7x13-KOI8-R.h>
#include "Graphics/Types.hpp"
#include <cstring>
#include <algorithm>
#include <fontx.h>
#include "Graphics/Display.hpp"

namespace Graphics {

Font Font::kDefault = Font{font7x13_KOI8_R};

Text &Text::operator=(const char *aText)
{
	auto len = std::strlen(aText);

	clear();
	reserve(len);
	std::copy(aText, aText + len, begin());

	return *this;
}

bool Offset::isValid() const
{
	return coord.at(0) >= 0 && coord.at(1) >= 0 && coord.at(0) <= Display::getCorner()[0] && coord.at(1) <= Display::getCorner()[1];
}


void Offset::fitToScreen()
{
	auto dispSz = Display::getCorner();
	if (coord.at(0) > dispSz[0]) {
		coord.at(0) = dispSz[0];
	}

	if (coord.at(1) > dispSz[1]) {
		coord.at(1) = dispSz[1];
	}
}

Vec2d Font::getGlyphSize()
{
	if (!font) {
		return {{-1, -1}};
	}

	fontx_meta_t meta;
	fontx_meta(&meta, font);

	return {{meta.width, meta.height}};
}

Vec2d Area::getSize() const
{
	return dotProduct<Vec2d, std::minus>(se.coord, nw.coord);
}

bool Area::checkValid() const
{
	return nw.isValid() && se.isValid();
}

void Area::applyOffset(const Vec2d &aOffset)
{
	if (checkValid()) {
		dotProduct<Vec2d, std::plus>(nw.coord, nw.coord, aOffset);
		dotProduct<Vec2d, std::plus>(se.coord, se.coord, aOffset);
	}
}

void Area::setOffset(const Vec2d &aOffset)
{
	if (checkValid()) {
		Vec2d diff = dotProduct<Vec2d, std::minus>(aOffset, nw.coord);
		nw.coord = aOffset;
		dotProduct<Vec2d, std::plus>(se.coord, se.coord, diff);
	}
}

void Area::setSize(const Vec2d &aSize)
{
	if (checkValid()) {
		dotProduct<Vec2d, std::plus>(se.coord, nw.coord, aSize);
	}
}

void Area::offsetSize(const Vec2d &aOffset)
{
	if (checkValid()) {
		dotProduct<Vec2d, std::plus>(se.coord, se.coord, aOffset);
	}
}

}  // namespace Graphics

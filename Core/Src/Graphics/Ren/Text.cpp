//
// Text.cpp
//
// Created on: Jan 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Graphics/Ren/Text.hpp"
#include <hagl.h>
#include <fontx.h>
#include "Graphics/Display.hpp"

namespace Graphics {

///
/// \brief Performs text rendering using HAL library's facilities
/// \pre All the properties are set to valid values
///
void Ren::Text::render()
{
	if (!(checkSetProperty<Graphics::Text, Font, Area>() && (checkSetProperty<Color>() || checkSetProperty<BgColor>()))) {  // The should be all properties set, either Color or BgColor may be missing
		return;
	}

	if (!checkSetProperty<Graphics::Units>()) {
		setProperty(Graphics::Units::Px);
	}

	fetchProperty(getProperty<Font>());
	fetchProperty(getProperty<Area>());

	auto areaSize = getProperty<Area>().getSize();
	if (areaSize.at(0) < glyphSize.at(0) || areaSize.at(1) < glyphSize.at(1)) {  // Won't fit
		return;
	}

	Vec2d origin = getProperty<Area>().nw.coord;
	const auto x0 = origin.at(0);
	const Vec2d &bound = getProperty<Area>().se.coord;

	// Fit the text into the Area
	for (unsigned short fWrap = 0, pos = 0;
		origin[1] < bound[1] && getProperty<Graphics::Text>().size() > pos;
		origin[1] += (fWrap ? glyphSize[1] : 0), origin[0] = (fWrap ? x0 : origin[0] + glyphSize[0]))
	{
		if (origin[0] + glyphSize[0] > bound[0]) {
			fWrap = true;
		} else {
			if (checkSetProperty<BgColor>()) {
				hagl_put_char_bg(getProperty<Graphics::Text>().at(pos), origin[0], origin[1], getProperty<BgColor>().fg, getProperty<BgColor>().bg, getProperty<Font>().font);
			} else {
				hagl_put_char(getProperty<Graphics::Text>().at(pos), origin[0], origin[1], getProperty<Color>().color, getProperty<Font>().font);
			}
			++pos;
			fWrap = false;
		}
	}
}

void Ren::Text::fetchProperty(Font &aFont)
{
	glyphSize = aFont.getGlyphSize();
}

void Ren::Text::fetchProperty(Area &aArea)
{
	if (!aArea.nw.isValid()) {
		aArea.nw.coord = Vec2d{{0, 0}};
	}

	if (!aArea.se.isValid()) {
		aArea.se.coord = Display::getCorner();
	}

	if (Units::Char == getProperty<Units>()) {
		dotProduct(aArea.nw.coord, aArea.nw.coord, glyphSize);
	}

	if (Units::Char == getProperty<Units>()) {
		dotProduct(aArea.se.coord, aArea.se.coord, glyphSize);
	}

	aArea.nw.fitToScreen();
	aArea.se.fitToScreen();
}

}  // namespace Graphics

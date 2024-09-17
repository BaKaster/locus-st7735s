//
// Rectangle.cpp
//
// Created on: Jan 28, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Graphics/Ren/Rectangle.hpp"
#include "Graphics/Display.hpp"

namespace Graphics {
namespace Ren {

void Rectangle::render()
{
	if (!checkSetProperty<Area>() || !(checkSetProperty<Color>() || checkSetProperty<BgColor>())) {
		return;
	}

	auto &area = getProperty<Area>();

	area.nw.fitToScreen();
	area.se.fitToScreen();

	if (checkSetProperty<Color>()) {
		hagl_draw_rectangle(area.nw.coord[0], area.nw.coord[1], area.se.coord[0], area.se.coord[1], getProperty<Color>().color);
	} else {
		if (area.se.coord == Display::getCorner() && area.nw.coord == Display::getOrigin()) {
			static const auto kColorWhite = hagl_color(255, 255, 255);

			if (asHold<BgColor>()->bg == kColorWhite) {
				hagl_clear_screen();
			} else {
				hagl_clear_screen_color(asHold<BgColor>()->bg);
			}
		} else {
			hagl_fill_rectangle(area.nw.coord[0], area.nw.coord[1], area.se.coord[0], area.se.coord[1], getProperty<BgColor>().bg);
		}
		hagl_draw_rectangle(area.nw.coord[0], area.nw.coord[1], area.se.coord[0], area.se.coord[1], getProperty<BgColor>().fg);
	}
}

}  // namespace Ren
}  // namespace Graphics

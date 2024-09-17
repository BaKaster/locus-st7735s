//
// Display.hpp
//
// Created on: Jan 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_GRAPHICS_DISPLAY_HPP
#define CORE_INC_GRAPHICS_DISPLAY_HPP

#include "Graphics/Types.hpp"

namespace Graphics {

struct Display {
	static inline const Vec2d &getSize()
	{
		static const Vec2d sz{{DISPLAY_WIDTH, DISPLAY_HEIGHT}};
		return sz;
	}

	static inline const Vec2d &getOrigin() {
		static const Vec2d sz{{0, 0}};
		return sz;
	}

	static inline const Vec2d &getCorner()
	{
		static const Vec2d sz{{DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1}};
		return sz;
	}

	static inline const Area &getArea()
	{
		static const Area area {
			Offset{getOrigin()},
			Offset{getCorner()}
		};

		return area;
	}
};

}  // namespace Graphics

#endif // CORE_INC_GRAPHICS_DISPLAY_HPP

//
// Render.hpp
//
// Created on: Jan 25, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_GRAPHICS_RENDER_HPP
#define CORE_INC_GRAPHICS_RENDER_HPP

namespace Graphics {

class Render {
public:
	virtual void render() = 0;
	virtual ~Render() = default;
};

}  // namespace Graphics

#endif // CORE_INC_GRAPHICS_RENDER_HPP

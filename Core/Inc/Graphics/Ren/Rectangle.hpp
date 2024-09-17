//
// Rectangle.hpp
//
// Created on: Jan 28, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_GRAPHICS_REN_RECTANGLE_HPP
#define CORE_INC_GRAPHICS_REN_RECTANGLE_HPP

#include "Graphics/Render.hpp"
#include "Graphics/Types.hpp"
#include "Graphics/Property.hpp"

namespace Graphics {
namespace Ren {

struct Rectangle : Render, PropertyPack<Graphics::Area, Graphics::Color, Graphics::BgColor> {

	template <class ...Tprop>
	Rectangle(Tprop &&...aArgs) : PropertyPack{std::forward<Tprop>(aArgs)...}
	{
	}

	void render() override;
};

}  // namespace Ren
}  // namespace Graphics

#endif // CORE_INC_GRAPHICS_REN_RECTANGLE_HPP

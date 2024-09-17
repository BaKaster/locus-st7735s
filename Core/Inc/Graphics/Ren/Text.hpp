//
// Text.hpp
//
// Created on: Jan 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef INC_GRAPHICS_REN_TEXT_HPP
#define INC_GRAPHICS_REN_TEXT_HPP

#include "Graphics/Render.hpp"
#include "Graphics/Property.hpp"
#include "Graphics/Types.hpp"
#include <utility>

namespace Graphics {
namespace Ren {

struct Text : Render, PropertyPack<Font, Color, BgColor, Area, Graphics::Text, Units> {
public:
	template <class ...Ts>
	Text(Ts &&...aArgs): PropertyPack(std::forward<Ts>(aArgs)...)  // Pass all the properties to the property initializer
	{
	}

	void render() override;

protected:
	void fetchProperty(Font &);
	void fetchProperty(Area &);

private:
	Vec2d glyphSize;
};

}  // namespace Ren
}  // namespace Graphics

#endif // INC_GRAPHICS_REN_TEXT_HPP

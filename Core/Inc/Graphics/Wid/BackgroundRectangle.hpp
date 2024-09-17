//
// BackgroundRectangle.hpp
//
// Created on: Feb 08, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_GRAPHICS_WID_BACKGROUND_RECTANGLE_HPP
#define CORE_INC_GRAPHICS_WID_BACKGROUND_RECTANGLE_HPP

#include "Graphics/Ren/Rectangle.hpp"
#include "Graphics/Widget.hpp"

namespace Graphics {
namespace Prio {

class OpQueue;

}  // namespace Prio

namespace Wid {

struct BackgroundRectangle : Widget, Ren::Rectangle {  ///< Draws an underlying (background) rectangle under another widget

	template <class ...Ts>
	BackgroundRectangle(Prio::OpQueue &aQueue, Ts &&...aArgs):
		Widget{aQueue},
		Ren::Rectangle{std::forward<Ts>(aArgs)...}
	{
	}

	void update(const PropertyHold<Graphics::Area> &);
	void performOp() override;
};

}  // namespace Wid
}  // namespace Graphics

#endif // CORE_INC_GRAPHICS_WID_BACKGROUND_RECTANGLE_HPP

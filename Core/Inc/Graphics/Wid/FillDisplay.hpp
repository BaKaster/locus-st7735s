//
// FillDisplay.hpp
//
// Created on: Jan 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef INC_GRAPHICS_WID_FILLDISPLAY
#define INC_GRAPHICS_WID_FILLDISPLAY

#include "Graphics/Ren/Rectangle.hpp"
#include "Graphics/Widget.hpp"
#include "Graphics/Render.hpp"
#include "Graphics/Types.hpp"
#include "Graphics/Property.hpp"

namespace Graphics {

namespace Wid {

class FillDisplay : public Widget, protected Ren::Rectangle {
public:
	using PropertyHold<BgColor>::set;
	FillDisplay(Prio::OpQueue &aQueue, BgColor);
	FillDisplay(Prio::OpQueue &aQueue, BgColor *);

	void performOp() override;
};

}  // namespace Wid
}  // namespace Graphics

#endif // INC_GRAPHICS_WID_FILLDISPLAY

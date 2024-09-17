//
// Text.hpp
//
// Created on: Jan 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef INC_GRAPHICS_WID_TEXT_HPP
#define INC_GRAPHICS_WID_TEXT_HPP

#include "Graphics/Widget.hpp"
#include "Graphics/Ren/Text.hpp"

namespace Graphics {
namespace Prio {

class OpQueue;

}  // namespace Prio
namespace Wid {

struct Text : Widget, Ren::Text {
	template <class ...Ts>
	Text(Prio::OpQueue &aQueue, Ts &&...aArgs) : Widget(aQueue), Ren::Text{std::forward<Ts>(aArgs)...}
	{
	}

	void performOp() override;
};

}  // namespace Wid
}  // namespace Graphics

#endif // INC_GRAPHICS_WID_TEXT_HPP

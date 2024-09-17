//
// FillDisplay.cpp
//
// Created on: Jan 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Graphics/Prio/OpQueue.hpp"
#include "Graphics/Types.hpp"
#include "Graphics/Ren/Rectangle.hpp"
#include "Graphics/Wid/FillDisplay.hpp"
#include "Graphics/Display.hpp"

namespace Graphics {
namespace Wid {

FillDisplay::FillDisplay(Prio::OpQueue &aQueue, BgColor aColor):
	Widget{aQueue},
	Ren::Rectangle{Display::getArea(), aColor}
{
}

FillDisplay::FillDisplay(Prio::OpQueue &aQueue, BgColor *aColor):
	Widget{aQueue},
	Ren::Rectangle{Display::getArea(), aColor}
{
}

void FillDisplay::performOp()
{
	render();
}

}  // namespace Wid
}  // namespace Graphics

//
// BackgroundRectangle.cpp
//
// Created on: Feb 08, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Graphics/Prio/OpQueue.hpp"
#include "Graphics/Wid/BackgroundRectangle.hpp"

namespace Graphics {
namespace Wid {

void BackgroundRectangle::update(const PropertyHold<Area> &aHoldArea)
{
	setProperty(aHoldArea.get());
}

void BackgroundRectangle::performOp()
{
	render();
}

}  // namespace Wid
}  // namespace Graphics

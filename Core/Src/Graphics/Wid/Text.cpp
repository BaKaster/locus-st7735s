//
// Text.cpp
//
// Created on: Feb 01, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Graphics/Prio/OpQueue.hpp"
#include "Graphics/Wid/Text.hpp"

namespace Graphics {
namespace Wid {

void Text::performOp()
{
	render();
}

}  // namespace Wid
}  // namespace Graphics

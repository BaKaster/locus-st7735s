//
// Priority.cpp
//
// Created on: Jan 25, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Graphics/Prio/Op.hpp"

namespace Graphics {
namespace Prio {

Op::Op(): Op(0)
{
}

Op::Op(Priority aPriority): priority{aPriority}
{
}

void Op::setPriority(Priority aPriority)
{
	priority = aPriority;
}

Priority Op::getPriority() const
{
	return priority;
}

bool Op::Compare::operator ()(const Graphics::Prio::Op &aLhs, const Graphics::Prio::Op &aRhs)
{
	return aLhs.getPriority() < aRhs.getPriority();
}

bool Op::Compare::operator ()(const Graphics::Prio::Op *aLhs, const Graphics::Prio::Op *aRhs)
{
	return aLhs->getPriority() < aRhs->getPriority();
}

}  // namespace Prio
}  // namespace Graphics

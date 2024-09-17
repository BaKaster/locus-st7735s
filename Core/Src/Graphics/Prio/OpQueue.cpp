//
// OpQueue.cpp
//
// Created on: Jan 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Graphics/Prio/Op.hpp"
#include "Graphics/Prio/OpQueue.hpp"
#include <algorithm>

namespace Graphics {
namespace Prio {

void OpQueue::run()
{
	for (auto op : *this) {
		op->performOp();
	}
}

OpQueue::iterator OpQueue::enqueue(Op &aOp)
{
	return insert(std::lower_bound(begin(), end(), &aOp, Op::Compare{}), &aOp);
}

void OpQueue::dequeue(OpQueue::iterator aHint)
{
	erase(aHint);
}

void OpQueue::requeue(OpQueue::iterator &aHint)
{
	auto *ptr = *aHint;
	dequeue(aHint);
	aHint = enqueue(*ptr);
}

}  // namespace Prio
}  // namespace Graphics

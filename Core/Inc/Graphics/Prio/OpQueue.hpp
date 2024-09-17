//
// OpQueue.hpp
//
// Created on: Jan 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_GRAPHICS_CONTAINER_HPP
#define CORE_INC_GRAPHICS_CONTAINER_HPP

#include <list>

namespace Graphics {
namespace Prio {

class Op;

struct OpQueue : protected std::list<Op *> {
	using std::list<Op *>::iterator;
	using std::list<Op *>::list;

	void run();
	iterator enqueue(Op &);
	void dequeue(iterator hint);
	void requeue(iterator &hint);  ///< Useful when the priority value is changed
};

}  // namespace Prio
}  // namespace Graphics

#endif // CORE_INC_GRAPHICS_CONTAINER_HPP

//
// Widget.hpp
//
// Created on: Jan 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_GRAPHICS_WIDGET_HPP
#define CORE_INC_GRAPHICS_WIDGET_HPP

#include "Graphics/Render.hpp"
#include "Graphics/Prio/Op.hpp"

namespace Graphics {
namespace Prio {

class OpQueue;

}  // namespace Prio

class Widget : public Prio::Op {
private:
	Prio::OpQueue *queue;  ///< Must be valid during Widget's lifespan. Never equals nullptr.

	using Iterator = typename Prio::OpQueue::iterator;
	Iterator it;  ///< Always valid. The Widget is always registered in a queue.

public:
	Widget(Prio::OpQueue &);
	Widget(Prio::OpQueue &, Prio::Priority);
	Widget(const Widget &);
	Widget(Widget &&);

	Widget &operator=(const Widget &);
	Widget &operator=(Widget &&);

	~Widget();
	void setQueue(Prio::OpQueue &);
	void setPriority(Prio::Priority) override;
};

}  // namespace Graphics

#endif // CORE_INC_GRAPHICS_WIDGET_HPP

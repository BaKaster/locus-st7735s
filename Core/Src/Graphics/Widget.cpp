//
// Widget.cpp
//
// Created on: Jan 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Graphics/Prio/OpQueue.hpp"
#include "Graphics/Widget.hpp"

namespace Graphics {

///
/// \brief Widget::Widget
///
Widget::Widget(Prio::OpQueue &aQueue, Prio::Priority aPriority): Prio::Op{aPriority}, queue{&aQueue}, it{aQueue.enqueue(*this)}
{
}

Widget::Widget(Prio::OpQueue &aQueue): Widget(aQueue, 0)
{
}

Widget::~Widget()
{
	queue->dequeue(it);
}

///
/// \brief Widget::setQueue Adds the option to swap a queue if needed
///
void Widget::setQueue(Prio::OpQueue &aQueue)
{
	queue->dequeue(it);
	queue = &aQueue;
	it = queue->enqueue(*this);
}

///
/// \brief Widget::setPriority Triggers changes in rendering sequences, if any are present
///
void Widget::setPriority(Prio::Priority aPriority)
{
	if (aPriority != getPriority()) {
		Op::setPriority(aPriority);
		queue->requeue(it);
	}
}

Widget::Widget(const Graphics::Widget &aWidget): Widget(*aWidget.queue, aWidget.getPriority())
{
}

Widget::Widget(Widget &&aWidget): Prio::Op{aWidget.getPriority()}, queue{aWidget.queue}, it{aWidget.it}
{
	queue->enqueue(aWidget);
}

}  // namespace Graphics

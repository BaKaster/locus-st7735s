//
// Op.hpp
//
// Created on: Jan 25, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_GRAPHICS_PRIORITYRENDER_HPP
#define CORE_INC_GRAPHICS_PRIORITYRENDER_HPP

namespace Graphics {
namespace Prio {

using Priority = char;  ///< The lower the value, the higher the priority

///
/// \brief The Op class Enables layered rendering, i.e. stacking UI
/// items on top of each other.
///
class Op {
private:
	Priority priority;

public:
	Op();
	Op(Priority);
	Op(const Op&) = default;
	Op(Op &&) = default;
	Op &operator=(const Op &) = default;
	Op &operator=(Op &&) = default;

	virtual void performOp() = 0;
	virtual void setPriority(Priority);
	Priority getPriority() const;

	virtual ~Op() = default;

	struct Compare {
		bool operator()(const Graphics::Prio::Op &lhs, const Graphics::Prio::Op &rhs);
		bool operator()(const Graphics::Prio::Op *lhs, const Graphics::Prio::Op *rhs);
	};
};

}  // namespace Prio
}  // namespace Graphics

#endif // CORE_INC_GRAPHICS_PRIORITYRENDER_HPP

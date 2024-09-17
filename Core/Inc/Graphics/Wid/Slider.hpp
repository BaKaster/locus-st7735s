//
// Slider.hpp
//
// Created on: Jan 31, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef INC_GRAPHICS_WID_SLIDER_HPP
#define INC_GRAPHICS_WID_SLIDER_HPP

#include "Graphics/Widget.hpp"
#include <array>
#include <limits>

namespace Graphics {

namespace Prio {

class OpQueue;

}  // namespace Prio

namespace Wid {
 class Composite;

template <unsigned short N>
class Slider : public Widget, public std::array<Prio::OpQueue, N> {
	unsigned short iSlide;
public:
	Slider(Prio::OpQueue &aQueue): Widget{aQueue}, std::array<Prio::OpQueue, N>{}, iSlide{0}
	{
	}

	void setSlide(unsigned aSlide)
	{
		iSlide = aSlide;
	}

	void shift(short aShift)  ///< Circular shift
	{
		static_assert(N < (unsigned)std::numeric_limits<short>::max(), "");
		aShift %= (short)N;
		auto fOverflow = (int(-aShift) > iSlide);

		iSlide = fOverflow ? (short(N) + aShift) % N : (iSlide + aShift) % N;
	}

	void performOp() override
	{
		this->at(iSlide).run();
	}
};

}  // namespace Wid
}  // namespace Graphics

#endif // INC_GRAPHICS_WID_SLIDER_HPP

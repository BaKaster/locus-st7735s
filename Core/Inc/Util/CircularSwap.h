//
// CircularSwap.hpp
//
// Created on: Feb 03, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_APP_SWAPCIRCULARBUFFER_HPP
#define CORE_INC_APP_SWAPCIRCULARBUFFER_HPP

#include <array>
#include <stack>
#include "Util/CircularBuffer.hpp"
#include "Util/PosArray.hpp"
#include <algorithm>

namespace Util {

template <unsigned Nbuffers, unsigned Nbytes>
class CircularSwap {  /// Stores a number of stack-allocated buffers organized in a circular manner
public:
	using Buffer = std::array<std::uint8_t, Nbytes>;
	using BufPtrArray = PosArray<Buffer *, Nbuffers>;

private:
	std::array<Buffer, Nbuffers> buffers;
	std::stack<Buffer *, BufPtrArray> stack;  ///< Free buffers
	Util::CircularBuffer<Buffer *, Nbuffers> ring;  ///< Buffers with payload

public:
	CircularSwap(): buffers{}, stack{}, ring{}
	{
		for (auto &buffer : buffers) {
			stack.push(&buffer);
		}
	}

	BufPtrArray popFree(unsigned aN)
	{
		BufPtrArray arrays;

		while (aN-- && countFull()) {
			arrays.push_back(&popFull());
		}

		return arrays;
	}

	BufPtrArray popFull(unsigned aN)
	{
		BufPtrArray arrays;

		while (aN-- && countFull()) {
			arrays.push_back(&popFull());
		}

		return arrays;
	}

	void pushFree(BufPtrArray &aArrays)
	{
		for (auto &a : aArrays) {
			pushFree(*a);
		}
	}

	void pushFull(BufPtrArray &aArrays)
	{
		for (auto &a : aArrays) {
			pushFull(*a);
		}
	}

	Buffer &popFree() {  ///< Get a free buffer form stack. \pre It must be checked whether one is available.
		auto *ret = stack.top();
		stack.pop();

		return *ret;
	}

	void pushFree(Buffer &aBuffer)  ///< Return a previously "borrowed" buffer as empty
	{
		stack.push(&aBuffer);
	}

	Buffer &popFull()  ///< Get a filled buffer from ring buffer. \pre It must be checked whether one is available.
	{
		auto *ret = ring.front();
		ring.pop_front();

		return *ret;
	}

	void pushFull(Buffer &aBuffer)  ///< Return a previously "borrowed" buffer as filled
	{
		ring.push_back(&aBuffer);
	}

	unsigned countFree() const
	{
		return stack.size();
	}

	unsigned countFull() const
	{
		return ring.size();
	}
};

}  // namespace Util

#endif // CORE_INC_APP_SWAPCIRCULARBUFFER_HPP

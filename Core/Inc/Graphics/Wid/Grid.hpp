//
// Grid.hpp
//
// Created on: Feb 01, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_GRAPHICS_WID_GRID_HPP
#define CORE_INC_GRAPHICS_WID_GRID_HPP

#include "Graphics/Widget.hpp"
#include "Graphics/Prio/OpQueue.hpp"
#include "Graphics/Property.hpp"
#include "Graphics/Types.hpp"
#include <numeric>
#include <algorithm>

namespace Graphics {
namespace Wid {
namespace GridImpl {

using SizeType = Vec2d::value_type;
///
/// \brief A helper. (1) holds casted pointers, (2) helps to keep associated
/// `PropertyHold<Area> *` instances in grid alignment. Tightly bound w/
/// Wid::Grid.
///
template <unsigned Nrows, unsigned Ncols>
struct Storage {
	struct Traits {
		PropertyHold<Area> *holdArea = nullptr;
		Prio::Op *op = nullptr;

		template <class T>
		Traits &operator=(T &aT)
		{
			holdArea = &aT;
			op = &aT;
			return *this;
		}

		void reset()
		{
			holdArea = nullptr;
			op = nullptr;
		}

		bool checkValid() const
		{
			return holdArea != nullptr && op != nullptr;
		}
	};

	Traits storage[Nrows][Ncols] = {{{}}};

	SizeType getColumnWidth(unsigned aColumn)
	{
		SizeType res = 0;
		iterateColumn([&res](Traits &a) {
			if (a.checkValid()) {
				res = std::max<SizeType>(res, a.holdArea->get().getSize()[0]);
			}
			}, aColumn);

		return res;
	}

	SizeType getRowHeight(unsigned aRow)
	{
		SizeType res = 0;
		iterateRow([&res](Traits &a) {
			if (a.checkValid()) {
				res = std::max<SizeType>(res, a.holdArea->get().getSize()[1]);
			}
			}, aRow);

		return res;
	}

	Vec2d getOffset(const Vec2d &aOffset, SizeType aRow, SizeType aColumn) {  ///< Calculates offsets based on those items' width and height values that prepend the given (row, col) pair
		Vec2d res = aOffset;

		for (auto row = 0; row < aRow; ++row) {
			res[1] += getRowHeight(row);
		}

		for (auto column = 0; column < aColumn; ++column) {
			res[0] += getColumnWidth(aColumn);
		}

		return res;
	}

	void updateOffsets(const Vec2d &aOffset)  ///< Update offsets starting from the provided one
	{
		for (unsigned row = 0, rowOffset = 0; row < Nrows; ++row) {
			for (unsigned col = 0; col < Ncols; ++col) {
				if (at(row, col).checkValid()) {
					SizeType vdiff = aOffset[1] + rowOffset - at(row, col).holdArea->get().nw.coord[1];
					at(row, col).holdArea->get().applyOffset(Vec2d{{0, vdiff}});
				}
			}
			rowOffset += getRowHeight(row);
		}

		for (unsigned col = 0, colOffset = 0; col < Ncols; ++col) {
			for (unsigned row = 0; row < Nrows; ++row) {
				if (at(row, col).checkValid()) {
					SizeType hdiff = aOffset[0] + colOffset - at(row, col).holdArea->get().nw.coord[0];
					at(row, col).holdArea->get().applyOffset(Vec2d{{hdiff, 0}});
				}
			}
			colOffset += getColumnWidth(col);
		}
	}

	template <class Tfn>
	void iterate(Tfn &&aFn, unsigned aStartRow, unsigned aStartCol, unsigned aStepSize, unsigned aNsteps)
	{
		Traits *it = &storage[aStartRow][aStartCol];
		Traits *end = it + aNsteps * aStepSize;

		for (; it < end; it += aStepSize) {
			aFn(*it);
		}
	}

	template <class Tfn>
	inline void iterateAll(Tfn &&aFn)
	{
		iterate(std::forward<Tfn>(aFn), 0, 0, 1, Ncols * Nrows);
	}

	template <class Tfn>
	inline void iterateColumn(Tfn &&aFn, unsigned aColumn)  ///< Iterate over columns
	{
		iterate(std::forward<Tfn>(aFn), 0, aColumn, Ncols, Nrows);
	}

	template <class Tfn>
	inline void iterateRow(Tfn &&aFn, unsigned aRow)  ///< Iterate over a row
	{
		iterate(std::forward<Tfn>(aFn), aRow, 0, 1, Ncols);
	}

	Traits &at(unsigned aRow, unsigned aColumn)
	{
		return storage[aRow][aColumn];
	}

	template <class T>
	Traits *tryPlaceAt(T &aT, unsigned aRow, unsigned aColumn)  ///< Returns `Traits *` pointer, if there is no widget occupying the requested cell. Nullptr otherwise.
	{
		if (at(aRow, aColumn).checkValid()) {
			return nullptr;
		}
		at(aRow, aColumn) = aT;

		return &at(aRow, aColumn);
	}
};

using GridPropertyPack = PropertyPack<Offset>;

}  // namespace GridImpl

template <unsigned Nrows, unsigned Ncols>
struct Grid : Widget, Prio::OpQueue, GridImpl::GridPropertyPack {
private:
	GridImpl::Storage<Nrows, Ncols> storage;

protected:
	using HoldArea = PropertyHold<Area>;
	using Traits = typename GridImpl::Storage<Nrows, Ncols>::Traits;

	void purgeDequeued()  ///< Widgets manage their rendering sequence attachments implicitly, separately from Grid<...>::storage. Therefore, to re-calculate offsets of those that are still present, detached instances shall be purged.
	{
		storage.iterateAll([this](Traits &aTraits) {
			if (aTraits.checkValid()) {
				auto it = std::find(begin(), end(), aTraits.op);
				if (it == end()) {
					aTraits.reset();
				}
			} });
	}

	void purge(Prio::Op &aOp)  ///< Tries to match the instance against already stored ones. If finds any, removes it from the table
	{
		storage.iterateAll([&](Traits &aTraits) {
			if (aTraits.op == &aOp) {
				aTraits.reset();
			}
			} );
	}

	enum class PlaceStrategy {
		Place,
		Fit,
		FitFill,
	};

	template <class T>
	bool tryPlaceImpl(T &aWidget, unsigned aRow, unsigned aColumn, PlaceStrategy aStrategy)
	{
		if (!checkSetProperty()) {
			return false;
		}

		if (!aWidget.template checkSetProperty<Graphics::Area>()) {
			return false;
		}

		purge(aWidget);
		Vec2d cellSize;

		if (PlaceStrategy::Place != aStrategy) {
			purgeDequeued();  // To re-calculate the cell's dimensions properly
			cellSize = Vec2d{{storage.getColumnWidth(aColumn), storage.getRowHeight(aRow)}};
		}

		static_cast<Widget &>(aWidget).setQueue(*this);
		auto *traits = storage.tryPlaceAt(aWidget, aRow, aColumn);

		if (!traits) {
			return false;
		}

		auto &area = traits->holdArea->get();
		area.setOffset(storage.getOffset(getProperty<Offset>().coord, aRow, aColumn));

		if (PlaceStrategy::Place == aStrategy) {
			return true;
		}

		auto itemSize = area.getSize();

		// Stretch the cell's sizes, if necessary
		if (cellSize[0] == 0) {
			cellSize[0] = itemSize[0];
		}

		if (cellSize[1] == 0) {
			cellSize[1] = itemSize[1];
		}

		if (PlaceStrategy::FitFill == aStrategy) {
			area.setSize(cellSize);
		} else if (PlaceStrategy::Fit == aStrategy) {
			auto sizeDiff = dotProduct<Vec2d, std::minus>(cellSize, itemSize);
			auto diffFlag = dotProduct<Vec2d, std::less>(cellSize, itemSize);
			// only shrink if it does not fit
			dotProduct<Vec2d, std::multiplies>(sizeDiff, sizeDiff, diffFlag);
			area.offsetSize(sizeDiff);
		}

		return true;
	}

	void updateOffsets()
	{
		storage.updateOffsets(getProperty<Offset>().coord);
	}

public:
	template <class ...Ts>
	Grid(Prio::OpQueue &aMasterQueue, Ts &&...aArgs):
		Widget{aMasterQueue},
		Prio::OpQueue{},
		GridImpl::GridPropertyPack{std::forward<Ts>(aArgs)...},
		storage{}
	{
	}

	template <class T>  ///< T is expected to implement PropertyHold<Area>, and Widget
	inline bool tryPlace(T &aWidget, unsigned aRow, unsigned aColumn)  ///< Places a widget into a cell, if it is not occupied yet. The item's size gets preserved, the grid gets resized if necessary.
	{
		return tryPlaceImpl(aWidget, aRow, aColumn, PlaceStrategy::Place);
	}

	template <class T>
	bool tryFit(T &aWidget, unsigned aRow, unsigned aColumn)  ///< Fits an item into a cell. The item's size is preserved only if it fits into the cell (it also applies when the item is the first one occupying the given row and column).
	{
		return tryPlaceImpl(aWidget, aRow, aColumn, PlaceStrategy::Fit);
	}

	template <class T>
	bool tryFitFill(T &aWidget, unsigned aRow, unsigned aColumn)  ///< Fits an item into a cell. The item's size is preserved only if it fits into the cell (it also applies when the item is the first one occupying the given row and column).
	{
		return tryPlaceImpl(aWidget, aRow, aColumn, PlaceStrategy::FitFill);
	}

	void performOp() override
	{
		purgeDequeued();
		updateOffsets();
		run();
	}

	constexpr unsigned countRows() const
	{
		return Nrows;
	}

	constexpr unsigned countColumns() const
	{
		return Ncols;
	}
};

}  // namespace Wid
}  // namespace Graphics

#endif // CORE_INC_GRAPHICS_WID_GRID_HPP

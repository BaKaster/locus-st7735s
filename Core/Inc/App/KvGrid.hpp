//
// KvGrid.hpp
//
// Created on: Feb 07, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef INC_APP_KVGRID_HPP
#define INC_APP_KVGRID_HPP

#include "Graphics/Wid/BackgroundRectangle.hpp"
#include "Graphics/Property.hpp"
#include "Graphics/Wid/Text.hpp"
#include "Graphics/Wid/Grid.hpp"
#include <cstring>
#include <cstdio>
#include <vector>
#include <functional>
#include <cassert>
#include <type_traits>

namespace App {

template <unsigned Nrows>
class KvGrid : public Graphics::Wid::Grid<Nrows, 2>, public Graphics::PropertyHold<Graphics::Area> {
private:
	using Base = typename Graphics::Wid::Grid<Nrows, 2>;
	const char *prefix;
	bool useNumSuffix;
	std::vector<Graphics::Wid::Text> textWidgets;
	Graphics::Ren::Rectangle backgroundRectangle;

	void brUpdate()
	{
		backgroundRectangle.setProperty(Graphics::Area{this->template getProperty<Graphics::Offset>(), getAt(Nrows - 1, 1).template getProperty<Graphics::Area>().se});
	}

protected:
	using Base::purgeDequeued;
	using Base::updateOffsets;
	using Base::run;

public:
	using Base::tryFit;

	inline Graphics::Wid::Text &getAt(unsigned aRow, unsigned aColumn)
	{
		assert(aRow < Nrows);
		assert(aColumn < 2);
		return textWidgets[aRow * 2 + aColumn];
	}

	void foreachText(std::function<void(Graphics::Wid::Text &, unsigned /*row*/, unsigned/*column*/)> aFn)
	{
		for (unsigned i = 0; i < textWidgets.size(); ++i) {
			unsigned row = i / 2;
			unsigned col = i % 2;
			aFn(getAt(row, col), row, col);
		}
	}

	template <class Toffset, class TbackgroundColor, class ...Ts>
	KvGrid(Graphics::Prio::OpQueue &aQueue, const char *aPrefix, bool aUseNumSuffix, Toffset &&aOffset, TbackgroundColor &&aBackgroundColor, Ts &&...aTxtArgs):  ///< A prefix gets prepended to every key string. fNumeric suffix, when set, is appeared at the end of each key string.
		Graphics::Wid::Grid<Nrows, 2>{aQueue, std::forward<Toffset>(aOffset)},
		prefix{aPrefix},
		useNumSuffix{aUseNumSuffix},
		textWidgets{Nrows * 2, Graphics::Wid::Text{*this, std::forward<Ts>(aTxtArgs)...}},
		backgroundRectangle{std::forward<TbackgroundColor>(aBackgroundColor)}
	{
		foreachText([this](Graphics::Wid::Text &aText, unsigned aRow, unsigned aCol) mutable {
			setKey(aRow, "");
			setValue(aRow, "-1");
			tryFit(aText, aRow, aCol);
		});
	}

	void setKey(unsigned aRow, const char *aKey)
	{
		if (useNumSuffix) {
			static constexpr const char *kFormat = "%s%s%d";
			getAt(aRow, 0).template setProperty(Graphics::Text{kFormat, prefix, aKey, aRow + 1});
		} else {
			static constexpr const char *kFormat = "%s%s";
			getAt(aRow, 0).template setProperty(Graphics::Text{kFormat, prefix, aKey});
		}
	}

	void setValue(unsigned aRow, const char *aValue)
	{
		getAt(aRow, 1).template setProperty(Graphics::Text{aValue});
	}

	template <class T, bool F = std::is_floating_point<T>::value>  // To restrain implicit int / float conversion
	typename std::enable_if<!F>::type setValue(unsigned aRow, T aValue)
	{
		getAt(aRow, 1).template setProperty(Graphics::Text{"%1d%", aValue});
	}

	template <class T, bool F = std::is_floating_point<T>::value>
	typename std::enable_if<F>::type setValue(unsigned aRow, T aValue)
	{
		getAt(aRow, 1).template setProperty(Graphics::Text{"%.1f", aValue});
	}

	void setValue(unsigned aRow, const Graphics::Text &aText)
	{
		getAt(aRow, 1).template setProperty(aText);
	}

	void performOp() override
	{
		purgeDequeued();
		updateOffsets();
		brUpdate();
		backgroundRectangle.render();
		run();
	}
};

}  // namespace App

#endif // INC_APP_KVGRID_HPP

//
// Lcd.hpp
//
// Created on: Feb 07, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_APP_LCD_HPP
#define CORE_INC_APP_LCD_HPP

#include "KvGrid.hpp"

namespace Lcs {

class Info;

}  // namespace Lcs

namespace Graphics {

namespace Wid {

template <unsigned short>
class Slider;

class Text;
class FillDisplay;

}  // namespace Wid

namespace Prio {

class OpQueue;

}  // namespace Prio

class Color;
class Area;
class Offset;

}  // namespace Graphics

namespace App {

class Lcd {
private:
	enum class Page {
		Main = 0,
		Active,
		Enabled,
		Amplitude,

		Max,
	} page;

	static constexpr short knPages = static_cast<short>(Page::Max);

	struct {
		Graphics::Prio::OpQueue background;
		Graphics::Prio::OpQueue kvGrid;
		struct {
			bool background = false;
			bool kvGrid = false;
		} update;
	} ren;

	Graphics::BgColor backgroundColor;
	Graphics::Wid::FillDisplay fillDisplay;
	Graphics::Wid::Slider<knPages> slider;

	Graphics::Color textColor;
	Graphics::Area textSize;  // Text size
	Graphics::Offset gridOffset;

	union {
		KvGrid<3> *kvGrid3;
		KvGrid<4> *kvGrid4;
		KvGrid<6> *kvGrid6;
	};

	struct {
		Graphics::Wid::Text pageNum;
	} widget;

	struct {
	};

private:
	void widgetPageNumFormatUpdate();

	template <unsigned Ncols, class Tvec>
	static void gridSetKv(KvGrid<Ncols> &aGrid, const char **aLabels, Tvec &&aValues)
	{
		for (unsigned i = 0; i < Ncols; ++i) {
			const char *label = aLabels != nullptr ? aLabels[i] : nullptr;

			aGrid.setValue(i, aValues[i]);

			if (label != nullptr) {
				aGrid.setKey(i, label);
			}
		}
	}

	template <unsigned Ncols>
	void gridInit(KvGrid<Ncols> **aGrid, const char *aKey, bool aUseNumSuffix)
	{
		*aGrid = new KvGrid<Ncols>(ren.kvGrid, aKey, aUseNumSuffix, &gridOffset, &backgroundColor, &Graphics::Font::kDefault, &textColor, textSize);
		(*aGrid)->setPriority(1);
	}

	template <unsigned Ncols>
	void gridDeinit(KvGrid<Ncols> **aGrid)
	{
		if (*aGrid != nullptr) {
			delete *aGrid;
			*aGrid = nullptr;
		}
	}

public:
	Lcd();
	void updateInfo(const Lcs::Info &);
	void slide(short aShift);
	void render();
};

}  // namespace App

#endif // CORE_INC_APP_LCD_HPP

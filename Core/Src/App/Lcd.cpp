//
// Lcd.cpp
//
// Created on: Feb 07, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "hagl.h"
#include "Lcs/Types.hpp"
#include "Lcs/Info.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Prio/OpQueue.hpp"
#include "Graphics/Wid/FillDisplay.hpp"
#include "Graphics/Wid/Slider.hpp"
#include "App/KvGrid.hpp"
#include "App/Lcd.hpp"

#include <cassert>

namespace App {

void Lcd::widgetPageNumFormatUpdate()
{
	static constexpr const char *kFormat = " Page %d / %d ";
	widget.pageNum.setProperty(Graphics::Text{kFormat, static_cast<int>(page) + 1, knPages});
}

Lcd::Lcd():
	page{Page::Main},
	backgroundColor{hagl_color(255, 255, 255), hagl_color(255, 255, 255)},
	fillDisplay{ren.background, &backgroundColor},
	slider{ren.background},
	textColor{hagl_color(0, 0, 0)},
	textSize{{0, 0}, {40, 15}},
	gridOffset{{5, 5}},
	kvGrid6{nullptr},
	widget{
		{ren.background, &Graphics::Font::kDefault, Graphics::BgColor{hagl_color(255, 255, 255), hagl_color(0, 0, 0)}, Graphics::Area{{{5, 130}}, {{120, 150}}}}
	}
{
	slide(0);
	widget.pageNum.setPriority(2);
	widgetPageNumFormatUpdate();
}

void Lcd::updateInfo(const Lcs::Info &aInfo)
{
	switch (page) {
		case Page::Main: {  // Position
			static const char *posLabels[] = {"x", "y", "z"};
			auto &pos = aInfo.getPos();
			for (int i = 0; i < 3; ++i) {
				kvGrid6->setKey(i, posLabels[i]);
				kvGrid6->setValue(i, pos[i]);
			}

    		static const char *eulerLabels[] = {"roll", "pitch", "yaw"};
    		static const char *kFpFormat = "%3d.%.1d"; // Формат для float
    		auto euler = aInfo.getEuler();
    		for (int i = 0; i < 3; ++i) {
        		char buffer[280]; 
        		snprintf(buffer, sizeof(buffer), kFpFormat, euler[i] / 10, euler[i] % 10); 

        		kvGrid6->setKey(i + 3, eulerLabels[i]);
        		kvGrid6->setValue(i + 3, Graphics::Text{buffer}); // Используем Graphics::Text
			}

			break;
		}

		case Page::Active: {  // Active beacons
			auto activeBeacons = aInfo.getActive();
			gridSetKv(*kvGrid4, nullptr, activeBeacons);

			break;
		}

		case Page::Enabled: {  // Enabled beacons
			auto enabledBeacons = aInfo.getEnabled();
			gridSetKv(*kvGrid4, nullptr, enabledBeacons);

			break;
		}

		case Page::Amplitude: {
			const auto &ampArr = aInfo.getAmpArr();
			gridSetKv(*kvGrid4, nullptr, ampArr);

			break;
		}

		default:
			assert(false);  // The proper handler should be written
	}

	ren.update.kvGrid = true;
}

void Lcd::slide(short aShift)
{
	// Deallocate grid
	switch (page) {
		case Page::Main:
			gridDeinit(&kvGrid6);

			break;

		case Page::Active:
		case Page::Enabled:
		case Page::Amplitude:
			gridDeinit(&kvGrid4);

			break;

		default:
			assert(false);  // The proper handler should be written
	}

	// Calculate new page index
	aShift %= knPages;
	auto fOverflow = (-aShift > static_cast<short>(page));
	page = static_cast<Page>(fOverflow ? (knPages + aShift) : (static_cast<short>(page) + aShift) % knPages);

	// Initialize new page widget
	switch (page) {
		case Page::Main:
			gridInit(&kvGrid6, "", false);

			break;

		case Page::Active:
			gridInit(&kvGrid4, "act.", true);

			break;

		case Page::Enabled:
			gridInit(&kvGrid4, "en.", true);

			break;

		case Page::Amplitude:
			gridInit(&kvGrid4, "amp ", true);

			break;

		default:
			assert(false);  // The proper handler should be written
	}

	widgetPageNumFormatUpdate();

	ren.update.background = true;
}

void Lcd::render()
{
	if (ren.update.background) {
		ren.background.run();
		ren.update.background = false;
	}

	if (ren.update.kvGrid) {
		ren.kvGrid.run();
		ren.update.kvGrid = false;
	}
}

}  // namespace App

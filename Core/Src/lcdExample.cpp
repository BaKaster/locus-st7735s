/* MIT License
 * Copyright (c) 2021, Michal Kozakiewicz, github.com/michal037
 *
 * code repository: https://github.com/michal037/driver-ST7735S
 * code version: 3.0.0
 */

#include <stdio.h> /* printf */
#include <stdlib.h> /* exit, size_t, malloc, free */
#include "st7735s.h"
#include "lcd.h"
#include "usart.h"

// hagl-related includes
#include <rgb888.h>
#include <hagl.h>
#include "fontx.h"
#include "Graphics/Types.hpp"
#include "Graphics/Ren/Text.hpp"
#include "Graphics/Ren/Rectangle.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Prio/OpQueue.hpp"
#include "Graphics/Prio/Op.hpp"
#include "Graphics/Wid/Grid.hpp"
#include "Graphics/Wid/FillDisplay.hpp"
#include "Graphics/Wid/Text.hpp"
#include "Graphics/Wid/Slider.hpp"

static Graphics::Font &sFont = Graphics::Font::kDefault;
int sliderShift = 0;

void exampleButtons()
{
	Graphics::Prio::OpQueue renderingSequence;

	Graphics::BgColor bgColor{hagl_color(255, 255, 255), hagl_color(255, 255, 255)};
	Graphics::Color textColor{hagl_color(0, 0, 0)};
	Graphics::Area area{Graphics::Display::getOrigin(), Graphics::Display::getCorner()};

	Graphics::Wid::Slider<2> slider{renderingSequence};
	Graphics::Wid::FillDisplay background{renderingSequence, &bgColor};

	Graphics::Wid::Text text1{slider[0], &sFont, &textColor, area, Graphics::Text{"There's nothing to look at"}};
	Graphics::Wid::Text text2{slider[1], &sFont, &textColor, area, Graphics::Text{"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."}};

	background.setPriority(0);
	slider.setPriority(1);

	renderingSequence.run();
	while (true) {
		while (sliderShift == 0);

		slider.shift(sliderShift);
		sliderShift = 0;

		renderingSequence.run();
	}
}

void exampleGrid()
{
	static constexpr auto kGridWidth = 2;

	using Grid = typename Graphics::Wid::Grid<4, kGridWidth>;

	Graphics::Prio::OpQueue renderingSequence;

	Graphics::BgColor bgColor{hagl_color(255, 255, 255), hagl_color(255, 255, 255)};
	Graphics::Color textColor{hagl_color(0, 0, 0)};
	Graphics::Area area{Graphics::Display::getOrigin(), {{50, 40}}};

	Graphics::Wid::FillDisplay background{renderingSequence, &bgColor};
	Grid grid{renderingSequence, Graphics::Offset{{{0, 0}}}};

	background.setPriority(0);
	grid.setPriority(1);

	Graphics::Wid::Text text1{grid, &sFont, &textColor, area, Graphics::Text{"There's nothing to look at"}};
	Graphics::Wid::Text text2{grid, &sFont, Graphics::Color{hagl_color(255, 0, 0)}, area, Graphics::Text{"But, hey, look here!"}};
	Graphics::Wid::Text text3{grid, &sFont, Graphics::Color{hagl_color(255, 0, 0)}, area, Graphics::Text{"Another text"}};

	grid.tryPlace(text1, 0, 0);
	grid.tryPlace(text2, 0, 1);
	grid.tryPlace(text3, 1, 1);

	renderingSequence.run();

	text3.getProperty<Graphics::Area>().offsetSize({{10, 0}});
	grid.tryPlace(text3, 1, 0);
	renderingSequence.run();

	grid.tryFitFill(text1, 0, 0);
	renderingSequence.run();

	grid.tryPlace(text1, 2, 1);
	renderingSequence.run();

	grid.tryFitFill(text2, 0, 1);
	renderingSequence.run();

	grid.tryFit(text3, 1, 0);
	renderingSequence.run();
}

int exampleMisc(void) {

	while (1) {
		lcd_drawFilledRectangle(0, 0, 128, 160, 255, 0, 0);
		hagl_put_pixel(60, 60, 0);
		lcd_drawFilledRectangle(0, 0, 20, 40, 0, 150, 0);
		lcd_clearScreen(200, 0, 0);
		lcd_delay(200);
		hagl_clear_screen();
		hagl_clear_screen_color(hagl_color(0, 0, 255));


		Graphics::Area area{
			Graphics::Offset{Graphics::Vec2d{{0,20}}},
			Graphics::Offset{Graphics::Vec2d{{10, 70}}}
		};

		Graphics::Ren::Text txt {
			&sFont,
			Graphics::Color{hagl_color(0, 255, 255)},
			&area,
			Graphics::Text{"wow"}
		};

		Graphics::Ren::Rectangle bg {
			Graphics::Area {
				Graphics::Offset{Graphics::Display::getOrigin()},
				Graphics::Offset{Graphics::Display::getCorner()}
			},
			Graphics::Color{hagl_color(30, 30, 50)}
		};

		bg.render();
		lcd_delay(300);

		bg.resetProperty<Graphics::Color>();
		bg.setProperty(Graphics::BgColor{hagl_color(0, 200, 200), hagl_color(200, 200, 200)});
		bg.render();

		txt.render();
		lcd_delay(300);
		txt.setProperty(Graphics::Text{"echo"});
		txt.setProperty(Graphics::Color{hagl_color(200, 0, 0)});
		txt.render();
		lcd_delay(300);
	}

	return 0;
}

extern "C" void toggleLed();

void exampleUartEcho()
{
	static constexpr auto kBufsz = 4;
	static uint8_t uart2Buffer[kBufsz];
	uint8_t nRecv = 0;
	while(1) {
		HAL_UART_Receive(&huart2, &nRecv, kBufsz, 100);
			if (nRecv > 0) {
				HAL_UART_Transmit(&huart2, uart2Buffer, nRecv, HAL_MAX_DELAY);
			toggleLed();
		}
	}
}

extern "C" int lcdExample(void)
{
//	exampleGrid();
//	exampleButtons();
	exampleUartEcho();
	return 0;
}

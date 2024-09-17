//
// Application.cpp
//
// Created on: Feb 04, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Graphics/Prio/OpQueue.hpp"
#include "App/KvGrid.hpp"
#include "Lcs/Types.hpp"
#include "Lcs/Info.hpp"
#include "Lcs/Parser.hpp"
#include "Graphics/Types.hpp"
#include "Graphics/Wid/Slider.hpp"
#include "Graphics/Wid/FillDisplay.hpp"
#include "App/Lcd.hpp"
#include "App/TimeoutMs.hpp"
#include "App/Application.hpp"

#include "stm32f0xx_hal.h"
#include "usart.h"
#include "sync.h"

namespace App {

Application::Application():
	receiveBuffer{nullptr},
	pending{0, false},
	timeout{2000, 300}
{
}

void Application::updateLcd()
{
	bool fUpdate = (timeout.display.tryUpdate() && pending.info) || (pending.slide != 0);

	if (fUpdate) {
		if (pending.slide != 0) {
			lcd.slide(pending.slide);
		}

		lcd.updateInfo(info);
		lcd.render();

		pending.info = false;
		pending.slide = 0;
	}
}

bool Application::tryParseReceived()
{
	static constexpr auto knPop = 2;
	auto nReady = cswap.countFull();
	bool res = false;
	CircularSwap::BufPtrArray bufPtrArray;

	if (nReady) {  // Non-locking precheck, optimization cutoff
		Sync::Critical critical;
		(void)critical;

		if ((nReady = cswap.countFull())) {
			bufPtrArray = cswap.popFull(nReady > knPop ? knPop : nReady);
		}
	}

	for (auto *buf : bufPtrArray) {
		auto parsingResult = parser.parse(buf->data(), buf->size());  // The buffer is expected to be filled to capacity, hence use of ::size()

		while (parsingResult.len != 0) {
			info.update(*parsingResult.message);
			res = true;
			parsingResult = parser.parse(parsingResult.data, parsingResult.len);
			pending.info = true;
		}
	}

	if (bufPtrArray.size()) {
		Sync::Critical critical;
		(void)critical;

		cswap.pushFree(bufPtrArray);
	}

	return res;
}

void Application::triggerReceiveNext()  ///< Receive next, that should be called from thread mode (user code). State-aware: may be called from a loop.
{
	if (receiveBuffer == nullptr) {
		{
			Sync::Critical critical;
			(void)critical;

			if (cswap.countFree()) {
				receiveBuffer = &cswap.popFree();
				HAL_UART_Receive_DMA(&huart2, receiveBuffer->data(), receiveBuffer->size());
			}
		}
	}
}

void Application::isrOnReceiveSuccess()  ///< that should be called from handler mode (interrupt routine)
{
	assert(receiveBuffer != nullptr);
	cswap.pushFull(*receiveBuffer);

	if (cswap.countFree()) {
		receiveBuffer = &cswap.popFree();
		HAL_UART_Receive_DMA(&huart2, receiveBuffer->data(), receiveBuffer->size());
	} else {
		receiveBuffer = nullptr;
	}
}

void Application::isrOnReceiveError()
{
	assert(receiveBuffer != nullptr);
	HAL_UART_Receive_DMA(&huart2, receiveBuffer->data(), receiveBuffer->size());
}

void Application::isrOnButtonSlide(short aSlide)
{
	if (timeout.button.tryUpdate()) {
		pending.slide = aSlide;
	}
}

}  // namespace App

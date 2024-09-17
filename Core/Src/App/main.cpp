//
// main.cpp
//
// Created on: Feb 03, 2022
//     Author: Dmitry Murashov (dmtrDOTmurashovATgmailDOTcom (gmail.com))
//

#include "Lcs/Types.hpp"
#include "Lcs/Info.hpp"
#include "Lcs/Parser.hpp"
#include "Util/CircularSwap.h"
#include "Graphics/Types.hpp"
#include "Graphics/Prio/OpQueue.hpp"
#include "Graphics/Wid/Slider.hpp"
#include "Graphics/Wid/FillDisplay.hpp"
#include "App/Lcd.hpp"
#include "App/TimeoutMs.hpp"
#include "sync.h"
#include "usart.h"
#include "App/Application.hpp"
#include "App/main.h"

static App::Application sApplication;

extern "C" void appMain()
{
	while (true) {
		sApplication.tryParseReceived();
		sApplication.updateLcd();
		sApplication.triggerReceiveNext();
	}
}

extern "C" void toggleLed();

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	sApplication.isrOnReceiveError();
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	sApplication.isrOnReceiveSuccess();
	toggleLed();
}

///
/// \brief Buttons callback
///
extern "C" void EXTI0_1_IRQHandler(void)
{
	if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_0)) {
		sApplication.isrOnButtonSlide(-1);
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
	} else if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_1)) {
		sApplication.isrOnButtonSlide(1);
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
	}
}

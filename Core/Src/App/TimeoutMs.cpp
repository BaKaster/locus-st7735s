//
// TimeoutMs.cpp
//
// Created on: Feb 08, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "App/TimeoutMs.hpp"
#include "stm32f0xx_hal.h"

namespace App {

TimeoutMs::TimeoutMs(std::uint32_t aTimeout): timeout{aTimeout}, lastTick{HAL_GetTick()}
{
}

bool TimeoutMs::tryUpdate()
{
	auto tick = HAL_GetTick();
	bool fUpdate = tick - lastTick > timeout;

	if (fUpdate) {
		lastTick = tick;
	}

	return fUpdate;
}

}  // namespace App

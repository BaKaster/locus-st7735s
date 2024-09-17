//
// TimeoutMs.hpp
//
// Created on: Feb 08, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_APP_TIMEOUTMS_HPP
#define CORE_INC_APP_TIMEOUTMS_HPP

#include <cstdint>

namespace App {

class TimeoutMs {
private:
	std::uint32_t timeout;
	std::uint32_t lastTick;
public:
	TimeoutMs (std::uint32_t aTimeout);
	bool tryUpdate();
};

}  // namespace App

#endif // CORE_INC_APP_TIMEOUTMS_HPP

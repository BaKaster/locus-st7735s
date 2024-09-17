//
// App.hpp
//
// Created on: Feb 04, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_APP_APP_HPP
#define CORE_INC_APP_APP_HPP

#include "Util/CircularSwap.h"
#include <cstdint>

namespace Lcs {

class Parser;
class Info;

}  // namespace Lcs

namespace App {

using CircularSwap = typename Util::CircularSwap<4, 128>;

class Lcd;
class TimeoutMs;

class Application {

	CircularSwap cswap;
	Lcs::Parser parser;
	Lcs::Info info;
	CircularSwap::Buffer *receiveBuffer;
	Lcd lcd;

	struct {
		short slide;
		bool info;
	} pending;

	struct {
		TimeoutMs display;  // Min. update period
		TimeoutMs button;  // Min. press period (to filter out noise)
	} timeout;

public:
	Application();
	void updateLcd();
	bool tryParseReceived();
	void triggerReceiveNext();
	void isrOnReceiveSuccess();
	void isrOnReceiveError();
	void isrOnButtonSlide(short aSlide);
};

}  // namespace App

#endif // CORE_INC_APP_APP_HPP

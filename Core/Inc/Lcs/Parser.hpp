/*
 * PioneerModuleSerial.hpp (Parser.hpp)
 *
 *  Created on: 14 dec 2018
 *      Author: kirill
 */

#ifndef CORE_INC_LCS_PARSER_HPP
#define CORE_INC_LCS_PARSER_HPP

#include <cstdint>

namespace Lcs {

union Message;

class Parser {
private:
	int parserIndex;
	int packetSize;
	std::uint8_t packet[260];
	std::uint8_t getInfoAttempt;

public:
	struct Res {
		const Message *message;
		const std::uint8_t *data;
		std::uint32_t len;
	};
	Res parse(const std::uint8_t *aData, std::uint32_t aLen);
};

}  // namespace Lcs

#endif // CORE_INC_LCS_PARSER_HPP

/*
 * PioneerModulesSerial.cpp (Parser.cpp)
 *
 *  Created on: 14 dec 2018
 *      Author: kirill
 */

#include "Lcs/Types.hpp"
#include <cstring>
#include <algorithm>
#include "Lcs/Parser.hpp"

namespace Lcs {

Parser::Res Parser::parse(const std::uint8_t *data, std::uint32_t len)
{
	uint32_t i = 0;

	while (i < len) {
		if (0 == parserIndex) {
			if (0xFE == data[i]) {
				parserIndex++;
			}
			i++;
		} else if (1 == parserIndex) {
			packet[parserIndex] = data[i];
			packetSize = data[i++] + 4;
			parserIndex++;
		} else if (parserIndex < packetSize) {
			auto l = std::min<int>(packetSize - parserIndex, len - i);
			std::memcpy(packet + parserIndex, data + i, l);
			parserIndex += l;
			i += l;
		} else {
			parserIndex = 0;
			if (data[i] == Util::FastCrc8::update(0, packet + 1, packetSize - 1)) {
				const Message *msg = reinterpret_cast<Message *>(packet);
				if (SerialSensors::EV_INFO == msg->head.event) {
					getInfoAttempt = 0;
				}
				return {msg, data + i, len - i};
			}
			i++;
		}
	}

	return {nullptr, nullptr, 0};
}

}  // namespace Lcs

/*
 * Types.hpp
 *
 *  Created on: Mar 27, 2014
 *      Author: Alexander
 */
//
// SerialSensorsMetadata.hpp
//
//  Created on: Dec 29, 2018
//      Author: andrey
//

#ifndef CORE_INC_LCS_TYPES_HPP
#define CORE_INC_LCS_TYPES_HPP

#include <cstdint>
#include "Util/FastCrc8.hpp"
#include <array>

namespace Lcs {

struct LpsTimerPayload {
	uint32_t time;
	uint32_t launchTime;
	uint8_t cmd;
	uint8_t flags;
	int16_t area[5];
};

struct OpticalFlow {
	enum class Flags : uint8_t {
		Ok = 0,
		Error = 1,
		NoData = 2,
	};
	uint8_t tofCount; //!< number of range sensors
	Flags status; //!< OF sensor status
	int16_t x; //!< X-axis OF value
	int16_t y; //!< Y-axis OF value
	uint16_t tof[5]; //!< range sensor data, mm
};

namespace SerialSensors {

// clang-format off
enum {
	EV_TELEMETRY       = 2,
	EV_BEACON_POS      = 3,
	EV_DETECTOR        = 4,
	EV_AP_INFO         = 5,
	EV_TIMER           = 18,
	EV_POWER_GOOD      = 20,
	EV_GET_INFO        = 23,
	EV_INFO            = 24,
	EV_OPTICAL_FLOW    = 25,
	EV_LED_CONTROL     = 26,
	EV_COMMAND         = 27,
	EV_BEACON_AMLITUDE = 51,
};
// clang-format on

// clang-format off
enum {
	CMD_CALIBRATE        = 0,
	CMD_CALIBRATE_GYRO   = 1,
	CMD_CALIBRATE_MOTORS = 2,
	CMD_LOG_START        = 3,
	CMD_LOG_STOP         = 4,
	CMD_REBOOT           = 5
};
// clang-format on

enum {
	FW_US_NAV = 1,
	FW_OPTICAL_FLOW = 2,
	FW_GS501_SENSOR_BOARD = 3
};

struct Header {
	uint8_t start;
	uint8_t size;
	uint8_t addr;
	uint8_t event;

	uint8_t crc() const
	{
		return Util::FastCrc8::update(0, &size, size + 3);
	}
};

struct BeaconPos {
	uint16_t active;
	int16_t beacons[4][3];
};

struct BeaconPosEvent: public Header {
	BeaconPos pl;
};

struct OpticalFlowMessage: public Header {
	OpticalFlow payload;
};

struct TelemetryEvent : public Header {
	uint32_t orientation;
	int32_t pos[3];
	int16_t vel[3];
	uint16_t voltage;
	uint8_t beacons;
	uint8_t status;
};

struct ApInfoEvent : public Header {
	uint16_t height;
	uint16_t voltage;
};

struct TimerEvent : public Header {
	LpsTimerPayload pl;
};

struct InfoEvent : public Header {
	uint16_t hwId;
	uint16_t fwId;
	uint8_t fwVerMinor;
	uint8_t fwVerMajor;
	uint8_t protoMinor;
	uint8_t protoMajor;
	uint32_t commit;
	uint16_t commitCount;
};

struct CommandEvent : public Header {
	uint8_t cmd;
	uint8_t cs;

	CommandEvent(uint8_t aCmd):
		Header{0xFE, 1, 0, EV_COMMAND},
		cmd{aCmd},
		cs{crc()}
	{
	}
};

struct PowerGoodEvent : public Header {
	uint8_t state;
};

struct BeaconAmplitude : public Header {
	std::array<uint16_t, 4> amp;
};

}  // namespace SerialSensors

union Message {
	SerialSensors::Header head;
	SerialSensors::BeaconPosEvent beaconPos;
	SerialSensors::TelemetryEvent telemetry;
	SerialSensors::TimerEvent timer;
	SerialSensors::OpticalFlowMessage flow;
	SerialSensors::InfoEvent info;
	SerialSensors::PowerGoodEvent powerGood;
	SerialSensors::BeaconAmplitude beaconAmplitude;
};

}  // namespace Lcs

#endif // CORE_INC_LCS_TYPES_HPP

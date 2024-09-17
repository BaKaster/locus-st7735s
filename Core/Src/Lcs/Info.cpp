//
// Info.cpp
//
// Created on: Feb 03, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "Lcs/Types.hpp"
#include "Lcs/Parser.hpp"
#include "Lcs/Info.hpp"

namespace Lcs {

BeaconFlags Info::getEnabled() const
{
	BeaconFlags flags;

	for (unsigned i = 0; i < knBeacons; ++i) {
		flags[i] = enabled & (1 << i);
	}

	return flags;
}

BeaconFlags Info::getActive() const
{
	BeaconFlags flags;

	for (unsigned i = 0; i < knBeacons; ++i) {
		flags[i] = active & (1 << i);
	}

	return flags;
}

Vec3d Info::getEuler() const
{
	Vec3d res;

	const struct {
		std::uint32_t roll: 11;
		std::uint32_t pitch: 10;
		std::uint32_t yaw: 11;
	} pack = *reinterpret_cast<decltype(pack) *>(&orientation);

	res[0] = pack.roll;
	res[1] = pack.pitch;
	res[2] = pack.yaw;

	return res;
}

const Vec3d &Info::getPos() const
{
	return pos;
}

void Info::update(const Message &aMessage)
{
	switch (aMessage.head.event) {
		case SerialSensors::EV_TELEMETRY:
			orientation = aMessage.telemetry.orientation;
			std::copy(aMessage.telemetry.pos, aMessage.telemetry.pos + 3, pos.begin());
			enabled = aMessage.telemetry.beacons;
			break;

		case SerialSensors::EV_BEACON_POS:
			active = aMessage.beaconPos.pl.active;
			break;

		case SerialSensors::EV_BEACON_AMLITUDE:
			ampArr = aMessage.beaconAmplitude.amp;

			break;

		default:
			break;
	}
}

const std::array<uint16_t, 4> &Info::getAmpArr() const
{
	return ampArr;
}

}  // namespace Lcs

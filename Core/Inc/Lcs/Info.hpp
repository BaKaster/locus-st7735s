//
// Info.hpp
//
// Created on: Feb 03, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_LCS_INFO_HPP
#define CORE_INC_LCS_INFO_HPP

#include <cstdint>
#include <array>

namespace Lcs {

union Message;

static constexpr std::size_t knBeacons = 4;
using Vec3d = std::array<std::uint32_t, 3>;
using BeaconFlags = std::array<bool, knBeacons>;

class Info {  ///< Further unpacks the relevant info from a received message
	std::uint32_t orientation;
	Vec3d pos;
	uint16_t active;
	uint8_t enabled;
	std::array<uint16_t, 4> ampArr;

public:
	BeaconFlags getEnabled() const;
	BeaconFlags getActive() const;
	Vec3d getEuler() const;
	const Vec3d &getPos() const;
	void update(const Message &);
	const std::array<uint16_t, 4> &getAmpArr() const;
};

}  // namespace Lcs

#endif // CORE_INC_LCS_INFO_HPP

//
// Types.hpp
//
// Created on: Jan 27, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include <type_traits>
#include <cstdint>
#include <hagl.h>
#include <vector>
#include <cstdint>
#include <array>
#include <algorithm>
#include <cstdio>

#ifndef CORE_INC_GRAPHICS_TYPES_HPP
#define CORE_INC_GRAPHICS_TYPES_HPP

namespace Graphics {

struct Color {  ///< HAGL color representation
	color_t color;
};

struct BgColor {
	color_t fg;
	color_t bg;
};

using Vec2d = typename std::array<std::int16_t, 2>;  ///< (x, y)

template <class Tvec, template <class ...> class TpOp = std::multiplies>
inline void dotProduct(Tvec &aRes, const Tvec &aLhs, const Tvec &aRhs)  ///< Hadamart multiplication (if TpOp is default) with resulting parameter
{
	using Value = typename std::remove_reference<decltype(aLhs[0])>::type;
	static_assert(std::is_arithmetic<Value>::value, "Non-arithmetic types are not supported by multiplication operator");

	std::transform(aLhs.cbegin(), aLhs.cend(), aRhs.cbegin(), aRes.begin(), TpOp<Value>{});
}

template <class Tvec, template <class ...> class TpOp = std::multiplies>
inline Tvec dotProduct(const Tvec &aLhs, const Tvec &aRhs)  ///< Hadamart multiplication (if TpOp is default)
{
	Tvec res;
	dotProduct<Tvec, TpOp>(res, aLhs, aRhs);

	return res;
}

struct Font {  ///< Encapsulation of fonts acceptible by HAGL library
	const unsigned char *font;

	Vec2d getGlyphSize();

	static Font kDefault;
};

struct Text : std::vector<char> {
	using std::vector<char>::vector;
	Text &operator =(const char *);

	template <class ...Ts>
	Text(const char *aFormat, Ts &&...aArgs): vector<char>(std::snprintf(nullptr, 0, aFormat, std::forward<Ts>(aArgs)...))
	{
		const auto kStringLen = std::snprintf(nullptr, 0, aFormat, std::forward<Ts>(aArgs)...);
		char buf[kStringLen + 1] = {0};
		sprintf(buf, aFormat, std::forward<Ts>(aArgs)...);
		std::copy(buf, buf + kStringLen, begin());
	}

	template <class ...Ts>
	void formatSet(const char *aFormat, Ts &&...aArgs)
	{
		const auto kStringLen = std::snprintf(nullptr, 0, aFormat, std::forward<Ts>(aArgs)...);
		char buf[kStringLen + 1] = {0};
		std::sprintf(buf, aFormat, std::forward<Ts>(aArgs)...);
		reserve(kStringLen);
		std::copy(buf, buf + kStringLen, begin());
	}
};

enum class Units {  ///< Units for settings clippings, offsets, etc.
	Char,  ///< Characters (width and height of a font's glyph)
	Px,  ///< Pixels
};

struct Offset {
	Vec2d coord;

	bool isValid() const;
	void fitToScreen();
};

struct Area {  ///< Same as offset, but can also be used for limiting the area of rendering
	Offset nw;
	Offset se;

	Vec2d getSize() const;
	bool checkValid() const;
	void applyOffset(const Vec2d &);
	void setOffset(const Vec2d &);
	void setSize(const Vec2d &);
	void offsetSize(const Vec2d &);  ///< Change SE coord
};

}  // namespace Graphics

#endif // CORE_INC_GRAPHICS_TYPES_HPP

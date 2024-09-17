//
// Property.hpp
//
// Created on: Jan 26, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_GRAPHICS_PROPERTY_HPP
#define CORE_INC_GRAPHICS_PROPERTY_HPP

#include <utility>

namespace Graphics {

template <class T>
using StripTp = typename std::remove_cv<
	typename std::remove_pointer<
	typename std::remove_reference<T>::type>::type>::type;

template <class T>
class PropertyHold {
	bool fHold;
	T *prop;

	void deleteIfHold()
	{
		if (fHold && prop != nullptr) {
			delete prop;
			fHold = false;
			prop = nullptr;
		}
	}

public:
	PropertyHold(): fHold{false}, prop{nullptr}
	{
	}

	PropertyHold(T *aPointer): fHold{false}, prop{aPointer}
	{
	}

	template <class ...Ts>
	PropertyHold(Ts &&...aArgs): fHold{true}, prop{new T{std::forward<Ts>(aArgs)...}}
	{
	}

	PropertyHold(const PropertyHold &aPh): fHold{false}, prop{aPh.prop}
	{
		if (aPh.prop != nullptr && aPh.fHold) {
			reset();
			set(*aPh.prop);
		}
	}

	PropertyHold(PropertyHold &&aPh): fHold{aPh.fHold}, prop{aPh.prop}
	{
		aPh.fHold = false;
	}

	PropertyHold &operator=(const PropertyHold &aPh)
	{
		if (aPh.prop == nullptr) {
			return *this;
		}

		if (aPh.fHold) {
			set(*aPh.prop);
		} else {
			set(aPh.prop);
		}
	}

	PropertyHold &operator=(PropertyHold &&aPh)
	{
		if (aPh.prop == nullptr) {
			return *this;
		}

		reset();
		aPh.prop = aPh.prop;
		aPh.fHold = nullptr;
	}

	T &operator=(T *aPtr)  ///< Set shared property.
	{
		set(aPtr);
		return *prop;
	}

	T &operator=(const T &aInstance)
	{
		set(aInstance);
		return *prop;
	}

	T &operator=(T &&aInstance)
	{
		set(aInstance);
		return *prop;
	}

	void reset()
	{
		deleteIfHold();
		prop = nullptr;
	}

	void set(T && aInstance)
	{
		if (prop == nullptr) {
			prop = new T{aInstance};
			fHold = true;
		} else {
			*prop = aInstance;
		}
	}

	void set(const T &aInstance)
	{
		if (prop == nullptr) {
			prop = new T{aInstance};
			fHold = true;
		} else {
			*prop = aInstance;
		}
	}

	void set(T *aPointer)
	{
		deleteIfHold();
		prop = aPointer;
	}

	bool checkOwn() const
	{
		return fHold;
	}

	const T &get() const
	{
		return *prop;
	}

	bool checkSet() const
	{
		return prop != nullptr;
	}

	~PropertyHold()
	{
		deleteIfHold();
	}

	T &operator*()
	{
		return *prop;
	}

	const T &operator*() const
	{
		return *prop;
	}

	T *operator->()
	{
		return prop;
	}

	const T *operator->() const
	{
		return prop;
	}

	T &get()
	{
		return *prop;
	}
};

template <class ...Ts>
struct PropertyPack : PropertyHold<Ts>... {
	template <class ...Ts2>
	PropertyPack(Ts2 &&...aArgs): PropertyHold<StripTp<Ts2>>{std::forward<Ts2>(aArgs)}...
	{
	}

	template <class T>
	PropertyHold<T> &asHold()
	{
		return static_cast<PropertyHold<StripTp<T>> &>(*this);
	}

	template <class T>
	const PropertyHold<T> &asHold() const
	{
		return static_cast<const PropertyHold<StripTp<T>> &>(*this);
	}

	template <class ...Ts2>
	bool checkOwnProperty() const
	{
		using List = bool[];
		bool f = true;
		(void)List{(f = f && asHold<Ts2>.checkOwn())...};

		return f;
	}

	template <class T, class ...Ts2>
	bool checkSetProperty() const
	{
		using List = bool[];
		bool f = asHold<StripTp<T>>().checkSet();

		if (f) {
			(void)List{(f = f && asHold<StripTp<Ts2>>().checkSet())...};
		}

		return f;
	}

	bool checkSetProperty() const
	{
		return checkSetProperty<Ts...>();
	}

	template <class T>
	void setProperty(T &&a)
	{
		asHold<StripTp<T>>() = a;
	}

	template <class T, class ...Ts2>
	void resetProperty(T &&a, Ts2 &&...aArgs)  ///< Detach asHold from a shared instance, and assign it with the new value independent of any other. Guaranteed memory reallocation
	{
		resetProperty<StripTp<T>, StripTp<Ts2>...>();
		setProperty(std::forward<T>(a), std::forward<Ts2>(aArgs)...);
	}

	template <class T, class ...Ts2>
	void resetProperty()
	{
		asHold<StripTp<T>>().reset();

		using List = int[];
		(void)List{(void(asHold<StripTp<Ts2>>().reset()), 0)...};
	}

	template <class T1, class T2, class ...Ts2>
	void setProperty(T1 &&a1, T2 &&a2, Ts2 &&...aArgs)
	{
		asHold<StripTp<T1>>() = a1;
		asHold<StripTp<T2>>() = a2;

		using List = int[];
		(void)List{(void(asHold<StripTp<Ts2>>() = aArgs), 0)...};
	}

	template <class T>
	const T &getProperty() const
	{
		return asHold<T>().get();
	}

	template <class T>
	T &getProperty()
	{
		return asHold<T>().get();
	}
};

}  // namespace Graphics

#endif // CORE_INC_GRAPHICS_PROPERTY_HPP

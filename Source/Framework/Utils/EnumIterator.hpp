//
// EnumIterator.hpp
//
// @author Roberto Cano
//

#pragma once

#include <type_traits>

#pragma region - EnumIterator
template <typename T>
class EnumIterator final
{
	static_assert(std::is_enum<T>::value, "Type must be an enumerator");
public:
	using type = typename std::underlying_type<T>::type;

	EnumIterator() = default;
	EnumIterator(T first)
	{
		_index = first;
	}
	~EnumIterator() = default;

	EnumIterator& operator=(T first)
	{
		_index = first;
	}

	bool operator<(T other_)
	{
		type index = static_cast<type>(_index);
		type other = static_cast<type>(other_);
		return index < other;
	}

	EnumIterator operator++(int dummy)
	{
		auto copy = *this;
		increment();
		return std::move(copy);
	}

	EnumIterator& operator++()
	{
		increment();
		return *this;
	}

	T operator*()
	{
		return _index;
	}

private:
	void increment()
	{
		auto value = static_cast<type>(_index);
		_index = static_cast<T>(++value);
	}
	T _index;
};

template <typename T>
class EnumBEIterator final
{
	static_assert(std::is_enum<T>::value, "Type must be an enumerator");
public:
	using type = typename std::underlying_type<T>::type;

	EnumBEIterator()
	{
		_index = T::_begin;
		_last = T::_end;
	}
	~EnumBEIterator() = default;

	operator bool()
	{
		type index = static_cast<type>(_index);
		type last = static_cast<type>(_last);
		return index < last;
	}

	EnumBEIterator operator++(int dummy)
	{
		auto copy = *this;
		increment();
		return std::move(copy);
	}

	EnumBEIterator& operator++()
	{
		increment();
		return *this;
	}

	T operator*()
	{
		return _index;
	}

private:
	void increment()
	{
		auto value = static_cast<type>(_index);
		_index = static_cast<T>(++value);
	}
	T _index;
	T _last;
};
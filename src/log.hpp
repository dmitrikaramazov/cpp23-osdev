#pragma once

#include "drivers/vga.hpp"
#include "drivers/serial.hpp"
#include "format.hpp"

namespace Log {
	inline void print(StringView str){
		VGA::print(str);
		SERIAL::print(str);
	}
	inline void println(StringView str){
		VGA::println(str);
		SERIAL::println(str);
	}
	template<typename... Args>
	inline void print(FormatString<type_identity_t<Args>...> fmt, const Args&... args) {
		VGA::print_fmt(fmt,args...);
		SERIAL::print_fmt(fmt,args...);
	}
	template<typename... Args>
	inline void println(FormatString<type_identity_t<Args>...> fmt, const Args&... args) {
		VGA::println(fmt,args...);
		SERIAL::println(fmt,args...);
	}
	template<typename... Args>
	inline void print_colored(FormatString<type_identity_t<Args>...> fmt, ColorCode color, const Args&... args) {
		VGA::print_fmt(color, fmt, args...);
		SERIAL::print_fmt(fmt,args...);
	}
}

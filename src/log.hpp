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
	template<FormatString fmt, typename... Args>
	inline void print(const Args&... args) {
		VGA::print_fmt<fmt>(args...);
		SERIAL::print_fmt<fmt>(args...);
	}
	template<FormatString fmt, typename... Args>
	inline void println(const Args&... args) {
		VGA::println<fmt>(args...);
		SERIAL::println<fmt>(args...);
	}
	template<FormatString fmt, typename... Args>
	inline void print_colored(ColorCode color, const Args&... args) {
		VGA::print_fmt<fmt>(color, args...);
		SERIAL::print_fmt<fmt>(args...);
	}
}

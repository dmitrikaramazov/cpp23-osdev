#pragma once
#include <stddef.h>
#include <stdint.h>

#include "../ports.hpp"
#include "../format.hpp"
#include "../atomic.hpp"


/*
 * VGA Writer specific structures
 */

enum Color {
	Black = 0,
	Blue,
	Green,
	Cyan,
	Red,
	Magenta,
	Brown,
	LightGray,
	DarkGray,
	LightBlue,
	LightGreen,
	LightCyan,
	LightRed,
	Pink,
	Yellow,
	White
};
class ColorCode{
private:
	uint8_t value;
public:
	constexpr ColorCode(Color fg, Color bg)
		: value((static_cast<uint8_t>(bg<<4) | static_cast<uint8_t>(fg))){}
	constexpr operator uint8_t() const{
		return value;
	}
};


class VGAWriter {
private:
	int row = 0;
	int col = 0;
	static constexpr int maxrow = 25;
	static constexpr int maxcol = 80;
	uint8_t color = 0x0F;
	volatile uint16_t* buffer = (uint16_t*)0xB8000;

	void bounds_check();
	void shift_up();
public:
	void write_char(char c, uint8_t color);
	void write_char(char c);
	template<size_t N>
    void print(const char (&str)[N]){
		print(StringView(str));
	}
	template<size_t N>
    void print(const char (&str)[N], uint8_t color){
		print(StringView(str), color);
	}
	void print(StringView str);
	void print(StringView str, uint8_t color);
	void clear();
	uint8_t get_color();
	void    set_color(uint8_t new_color);
};

namespace VGA{
	extern Spinlock vga_lock;
	extern VGAWriter vga;

	template<FormatString fmt, typename... Args>
	void print_fmt(const Args&... args){
		vga_lock.lock();
		Fmt::print_fmt<decltype(vga), fmt>(vga,args...);
		vga_lock.unlock();
	}
	template<FormatString fmt, typename... Args>
	void print_fmt(ColorCode color_code, const Args&... args){
		vga_lock.lock();
		uint8_t old_color = vga.get_color();
		vga.set_color(color_code);
		Fmt::print_fmt<decltype(vga), fmt>(vga,args...);
		vga.set_color(old_color);
		vga_lock.unlock();
	}
	template<FormatString fmt, typename... Args>
	void println(const Args&... args){
		vga_lock.lock();
		Fmt::print_fmt<decltype(vga), fmt>(vga,args...);
		vga.print("\n");
		vga_lock.unlock();
	}
	template<FormatString fmt, typename... Args>
	void println(ColorCode color_code, const Args&... args){
		vga_lock.lock();
		uint8_t old_color = vga.get_color();
		vga.set_color(color_code);
		Fmt::print_fmt<decltype(vga), fmt>(vga,args...);
		vga.set_color(old_color);
		vga.print("\n");
		vga_lock.unlock();
	}
	void print(StringView str);
	void print(StringView str, uint8_t color);
	void println(StringView str);
	void println(StringView str, uint8_t color);
	void disable_cursor();
	void clear();
}



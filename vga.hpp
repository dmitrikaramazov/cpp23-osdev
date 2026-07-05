#pragma once
#include <cstddef>
#include <cstdint>

class Spinlock {
private:
	bool locked = false;
public:
	void lock(){
		while(__atomic_test_and_set(&locked, __ATOMIC_ACQUIRE)){
			asm volatile("pause");
		}
	}
	void unlock(){
		__atomic_clear(&locked, __ATOMIC_RELEASE);
	}
};

class StringView {
private:
	const char* data_;
	size_t length_;
public:
	template<size_t N>
    constexpr StringView(const char(&str)[N]) : data_(str), length_(N-1) {}
	constexpr StringView(const char* str, size_t len) : data_(str), length_(len) {}
	char operator[](size_t index) const {
		if(index >= length_) return ' ';
		return data_[index];
	}
	size_t length() const { return length_; }
	const char* data() const { return data_; }
};


template<size_t N>
struct FormatString{
	char data[N];
	int placeholder_count = 0;
	consteval FormatString(const char(&str)[N]) {
		for(size_t i = 0; i < N; i++){
			data[i] = str[i];
			if (i < N-1 && str[i] == '{' && str[i+1] == '}'){
				placeholder_count++;
			}
		}
	}
};




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
	template<typename T, typename... Args>
	void unroll_format(StringView str, const T& value, const Args&... args) {
		for(size_t i = 0; i < str.length(); i++){
			if(i < str.length() - 1 && str[i] == '{' && str[i+1] == '}') {
				print(StringView(str.data(),i));
				this->print(value);
				StringView remainder(str.data() + i + 2, str.length() - (i+2));
				if constexpr (sizeof...(args) > 0){
					unroll_format(remainder, args...);
				} else {
					print(remainder);
				}
				return;
			}
		}
	}
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
	void print(int32_t number);
	template<FormatString fmt, typename... Args>
	void print_fmt(const Args&... args) {
		static_assert(fmt.placeholder_count == sizeof...(Args),
				"Format Error: Number of {} does not match arguments!");
		StringView view(fmt.data, sizeof(fmt.data)-1);
		if constexpr (sizeof...(Args) > 0) {
			unroll_format(view,args...);
		} else {
			print(view);
		}
	}
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
		vga.print_fmt<fmt>(args...);
		vga_lock.unlock();
	}
	template<FormatString fmt, typename... Args>
	void print_fmt(ColorCode color_code, const Args&... args){
		vga_lock.lock();
		uint8_t old_color = vga.get_color();
		vga.set_color(color_code);
		vga.print_fmt<fmt>(args...);
		vga.set_color(old_color);
		vga_lock.unlock();
	}
	template<FormatString fmt, typename... Args>
	void println(const Args&... args){
		vga_lock.lock();
		vga.print_fmt<fmt>(args...);
		vga.print("\n");
		vga_lock.unlock();
	}
	template<FormatString fmt, typename... Args>
	void println(ColorCode color_code, const Args&... args){
		vga_lock.lock();
		uint8_t old_color = vga.get_color();
		vga.set_color(color_code);
		vga.print_fmt<fmt>(args...);
		vga.print("\n");
		vga.set_color(old_color);
		vga_lock.unlock();
	}
	void print(StringView str);
	void print(StringView str, uint8_t color);
	void println(StringView str);
	void println(StringView str, uint8_t color);

	void clear();
}



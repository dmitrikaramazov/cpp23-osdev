#pragma once
#include <stddef.h>
#include <stdint.h>

#include "../format.hpp"
#include "../atomic.hpp"
#include "../ports.hpp"

class SerialWriter {
private:
	
public:
	void write_char(char c);
	void print(StringView str);
};

namespace SERIAL {
	extern Spinlock serial_lock;
	extern SerialWriter serial;

	template<FormatString fmt, typename... Args>
	void print_fmt(const Args&... args){
			serial_lock.lock();
			Fmt::print_fmt<decltype(serial), fmt>(serial,args...);
			serial_lock.unlock();
	}
	template<FormatString fmt, typename... Args>
	void println(const Args&... args){
			serial_lock.lock();
			Fmt::print_fmt<decltype(serial), fmt>(serial,args...);
			serial.write_char('\r');
			serial.write_char('\n');
			serial_lock.unlock();
	}
	void print(StringView str);
	void println(StringView str);
	
}

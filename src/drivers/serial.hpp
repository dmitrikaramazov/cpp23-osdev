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

	template<typename... Args>
	void print_fmt(FormatString<type_identity_t<Args>...> fmt, const Args&... args){
			serial_lock.lock();
			//Fmt::print_fmt<decltype(serial), fmt>(serial,args...);
			Fmt::print_fmt(serial,fmt,args...);
			serial_lock.unlock();
	}
	template<typename... Args>
	void println(FormatString<type_identity_t<Args>...> fmt, const Args&... args){
			serial_lock.lock();
			//Fmt::print_fmt<decltype(serial), fmt>(serial,args...);
			Fmt::print_fmt(serial,fmt,args...);
			serial.write_char('\r');
			serial.write_char('\n');
			serial_lock.unlock();
	}
	void print(StringView str);
	void println(StringView str);
	
}

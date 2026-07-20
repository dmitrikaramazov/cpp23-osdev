#include "serial.hpp"

// TODO: init w/ baud rate

namespace SERIAL {
	Spinlock serial_lock;
	SerialWriter serial;
	constexpr uint16_t COM1_PORT = 0X3F8;

	void print(StringView str){
		serial_lock.lock();
		serial.print(str);
		serial_lock.unlock();
	}
	void println(StringView str){
		serial_lock.lock();
		serial.print(str);
		serial.write_char('\r');
		serial.write_char('\n');
		serial_lock.unlock();
	}
}


void SerialWriter::write_char(char c){
	while((Ports::inb(SERIAL::COM1_PORT+5) & 0X20) == 0){
			// do nuttin
	}
	Ports::outb(SERIAL::COM1_PORT, static_cast<uint8_t>(c));
}
void SerialWriter::print(StringView str){
	for(size_t i = 0; i < str.length(); i++){
		write_char(str[i]);
	}
}

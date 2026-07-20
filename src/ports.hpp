#pragma once
#include <stdint.h>

namespace Ports {
	static inline void outb(uint16_t port, uint8_t data){
		asm volatile("outb %0, %1": : "a"(data), "Nd"(port));
	}
	static inline uint8_t inb(uint16_t port){
		uint8_t result;
		asm volatile("inb %1, %0" : "=a"(result): "Nd"(port));
		return result;
	}
}

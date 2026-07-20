#pragma once

#include <stdint.h>
#include <stddef.h>


enum class GateType {
	TaskGate32 		= 0x5,
	InterruptGate16 = 0x6,
	TrapGate16 		= 0x7,
	InterruptGate32	= 0xE,
	TrapGate32 		= 0xF,
};
enum class PrivilegeLevel{
	Ring0 = 0,
	Ring1 = 1,
	Ring2 = 2,
	Ring3 = 3
};
enum class InterruptType: uint8_t {
	DivisionError = 0,
	Debug = 1,
	NonMaskableInterrupt = 2,
	Breakpoint = 3,
	Overflow = 4 ,
	BoundRangeExceeded = 5,
	InvalidOpcode = 6,
	DeviceNotAvailable = 7,
	DoubleFault = 8,
	// Deprecated
	InvalidTSS = 10,
	SegmentNotPresent = 11,
	StackSegmentFault = 12,
	GeneralProtectionFault = 13,
	PageFault = 14,
	// Reserved
	X87FloatingPont = 16,
	MachineCheck = 18,
	SIMDFPException = 19,
	VirtException = 20,
	ControlProtectionException = 21,
	// Reserved 22-27
	HyperVisorInejctionException = 28,
	VMMCommunicationException = 29,
	SecurityException = 30,
	// Reserved
	TripleFault = 32,
	// Deprecated FPU
};

struct [[gnu::packed]] IDT_Options {
	uint16_t 		reserved	: 8;
	GateType 		gate_type 	: 4;
	uint16_t 		zero		: 1;
	PrivilegeLevel 	dpl			: 2;
	uint16_t 		present		: 1;
};

struct [[gnu::packed]] IDT_Entry {
	uint16_t low_offset;
	uint16_t selector;
	IDT_Options options;
	uint16_t high_offset;
};

struct [[gnu::packed]] IDT_Descriptor {
	uint16_t size;
	uint32_t offset;
};

// 6.5.1 of IA-32 manual
struct [[gnu::packed]] Interrupt_Frame {
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp; // user stack pointer
	uint32_t ss;  // user stack segment
};

class InterruptDescriptorTable {
private:
	IDT_Entry idt[256] = {};
public:
	template <typename T>
	void set_handler(InterruptType interrupt, T handler) {
		uint8_t index = static_cast<uint8_t>(interrupt);
		uint32_t address = reinterpret_cast<uint32_t>(handler);
		idt[index] = IDT_Entry {
			.low_offset = static_cast<uint16_t>(address & 0xFFFF),
			.selector = 0x08,
			.options = {
				.reserved = 0,
				.gate_type = GateType::InterruptGate32,
				.zero = 0,
				.dpl = PrivilegeLevel::Ring0,
				.present = 1
			},
			.high_offset = static_cast<uint16_t>((address >> 16) & 0xFFFF)
		};
	}
};

void init_idt();

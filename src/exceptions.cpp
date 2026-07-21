#include "exceptions.hpp"
#include "tests.hpp"


[[gnu::interrupt]]
static void breakpoint_handler(Interrupt_Frame* frame){
	Log::println("[!] EXCEPTION: #BP");
	Log::println("\tEIP: {}",(frame->eip));
	Log::println("\tCS: {}",(frame->cs));
	Log::println("\tEFLAGS: {}",(frame->eflags));
}

[[gnu::interrupt]]
static void double_fault_handler(Interrupt_Frame* frame, uint32_t error_code) {
	Log::println("[!] EXCEPTION: #DF");
	Log::println("\tError Code: {}",(error_code));
	Log::println("\tEIP: {}",(frame->eip));
	Log::println("\tCS: {}",(frame->cs));
	Log::println("\tEFLAGS: {}",(frame->eflags));
	exit_qemu(QemuExitCode::Failed);
}

static inline void load_idt(IDT_Descriptor* itdr){
	__asm__("lidt %0" :: "m"(*itdr));
}

void init_idt() {
	static InterruptDescriptorTable idt = {};
	IDT_Descriptor itdr;
	itdr.size = sizeof(InterruptDescriptorTable) - 1;
	itdr.offset = reinterpret_cast<uint32_t>(&idt);
	load_idt(&itdr);
	idt.set_handler(InterruptType::Breakpoint, &breakpoint_handler);
	idt.set_handler(InterruptType::DoubleFault, &double_fault_handler);
}

/*
 * Tests
 */
static_assert(sizeof(IDT_Options) == 2, "IDT Options must be exactly 2 bytes on x86");
TEST_CASE(idt_bit_layout_test){
	// Note with C++23 and a compatible compiler, 
	// this could be a constexpr compiletime assert
	Log::print("idt_bit_layout_test... ");
	IDT_Options opt;
	opt.reserved = 0;
	opt.gate_type = GateType::InterruptGate32;
	opt.zero = 0;
	opt.dpl = PrivilegeLevel::Ring0;
	opt.present = 1;
	uint16_t raw = *reinterpret_cast<uint16_t*>(&opt);
	if(raw != 0x8e00) {
		Log::println("[Failed]");
		exit_qemu(QemuExitCode::Failed);
	}
	Log::println("[OK]");
};
TEST_CASE(breakpoint_exception_test){
	Log::println("breakpoint_exception_test... ");
	IDT_Descriptor  current_idtr;
	current_idtr.offset = 0;
	current_idtr.size = 0;
	__asm__ volatile("sidt %0" : "=m"(current_idtr));
	if(current_idtr.size == 0 || current_idtr.offset == 0) {
		Log::println("[Failed] - No IDT loaded");
		exit_qemu(QemuExitCode::Failed);
	}
	__asm__ volatile("int $3");
	Log::println("[OK]");
}

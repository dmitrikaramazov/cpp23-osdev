#include <cstddef>
#include <cstdint>

#include "./drivers/vga.hpp"








// Multiboot header
// QEMU reads this and loads the program
struct [[gnu::packed]] MultibootHeader {
	uint32_t magic;
	uint32_t flags;
	uint32_t checksum;
};

extern "C" [[gnu::section(".multiboot"), gnu::used]]
const MultibootHeader header = {
	0x1badb002,
	0x00000000,
	static_cast<uint32_t>(-(0x1badb002 + 0x00000000))
};



extern "C" [[noreturn]] void _start() {
	VGA::clear();
	VGA::print("[*] Booting OS...\n");
	VGA::print("[i] Testing...\n", ColorCode(Blue,Green));
	for(int i = 0; i < 3; i++){
		Color fg = static_cast<Color>((i%15)+2);
		Color bg = static_cast<Color>((i%15)+3);
		VGA::print("[i]    ");
		VGA::print("Spam!\n", ColorCode(fg,bg));
	}
	VGA::print_fmt<"[*] Testing: {} {}\n">(80,81);
	VGA::print_fmt<"[*] Testing: {} {}\n">(StringView("Test"),81);
	VGA::print_fmt<"[*] Testing: {} {}\n">(82, "test2");
	VGA::print_fmt<"[*] Size of {}: {}\n">("size_t", sizeof(size_t));
	VGA::print_fmt<"[*] Size of {}: {}\n">("int*", sizeof(int*));
	VGA::println("[i] Printing line");
	VGA::println<"[x] {}">("formatted");
	VGA::println(">>> SYSTEM CRITICAL <<<",ColorCode(Red,Black));
	VGA::println<"[*] Colored {}">(ColorCode(LightRed,DarkGray),95);

	while(true){

	}
}

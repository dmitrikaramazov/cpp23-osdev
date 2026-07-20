ENTRY(_start)
SECTIONS {
	. = 1M;
	.text : {
		KEEP(*(.multiboot))
		*(.text .text.*)
	}
	.rubberducky BLOCK(4K):ALIGN(4K) {
		*(.rodata .rodata.*)
	}
	/* block&Align 4k so rwx load segment warning goes away */
	.data BLOCK(4K):ALIGN(4K) {
		*(.data .data.*)
	}
	.test_array BLOCK(4K):ALIGN(4K) {
		PROVIDE(tests_start = .);
		KEEP(*(.test_array))
		PROVIDE(tests_end = .);
	}
	.bss BLOCK(4K):ALIGN(4K) {
		*(COMMON)
		*(.bss .bss.*)
	}
}

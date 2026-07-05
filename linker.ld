ENTRY(_start)
SECTIONS {
	. = 1M;
	.text : {
		*(.multiboot)
		*(.text)
	}
	.rubberducky BLOCK(4K):ALIGN(4K) {
		*(.rodata .rodata.*)
	}
	/* block&Align 4k so rwx load segment warning goes away */
	.data BLOCK(4K):ALIGN(4K) {
		*(.data)
	}
}

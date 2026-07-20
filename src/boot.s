.intel_syntax noprefix
/*
* https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
*/
.set ALIGN, 	1<<0
.set MEMINFO, 	1<<1
.set FLAGS, 	ALIGN | MEMINFO
.set MAGIC,		0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

/*
* Actual multiboot header
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*
* stack and stuff
*/
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:


.section .text
.global _start
.type _start, @function
_start:
	mov esp, offset stack_top
	call kernel_main
	cli
1: 	hlt
	jmp 1b

.size _start, . - _start

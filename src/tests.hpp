#pragma once
#include <stdint.h>
#include "ports.hpp"
#include "log.hpp"

typedef void (*TestCase)();
#ifdef KERNEL_TEST
#define TEST_CASE(name) \
	void name(); \
	[[gnu::section(".test_array"), gnu::used]] \
	TestCase __test__##name = name; \
	void name()
#else
#define TEST_CASE(name) \
	[[gnu::always_inline, gnu::unused]] \
	inline void name()
#endif

enum class QemuExitCode : uint32_t {
	Success = 0x10,
	Failed = 0x11
};

inline void exit_qemu(QemuExitCode code){
	Ports::outb(0xF4, static_cast<uint32_t>(code));
}

extern "C" TestCase tests_start;
extern "C" TestCase tests_end;

inline void run_all_tests(){
#ifdef KERNEL_TEST
	TestCase* start = &tests_start;
	TestCase* end   = &tests_end;
	size_t count = end - start;
	Log::println("[*] Running {} tests",count);
	for(TestCase* test = start; test < end; test++){
		(*test)();
	}
	Log::println("[+] All tests passed!");
	//exit_qemu(QemuExitCode::Success);
#endif
}






## 1. Advanced Memory Management
*   **Virtual Memory:** Implement x86 paging (higher-half kernel, page tables, handling page faults).
*   **Physical Allocation:** Implement a "Buddy Allocator" for physical memory frames.
*   **Kernel Heap:** Write your own custom `malloc`/`free` or `new`/`delete` (e.g., a Slab Allocator).
*   *Employer Signal:* Trading firms obsess over memory allocation performance and theory. 

## 2. Modern C++ in a Hostile Environment
*   Continue utilizing C++23 features (like Concepts) in a freestanding environment.
*   Write your own highly optimized, exception-free, RTTI-free versions of STL containers (`std::vector`, `std::string`, `std::unique_ptr`).
*   *Employer Signal:* Shows you understand move semantics, placement `new`, and memory alignment at a fundamental hardware level.

## 3. Custom Network Stack (The "Holy Grail")
*   Write a driver for a virtual network card (like the E1000 or RTL8139 in QEMU).
*   Implement Ethernet, IPv4, and a basic UDP/TCP layer.
*   *Employer Signal:* Demonstrates deep networking and hardware integration knowledge. Massively impressive to trading and low-latency firms.

## 4. Context Switching & User Mode
*   Implement hardware interrupts (GDT and IDT).
*   Write a preemptive scheduler (e.g., Round-Robin or CFS) that uses the hardware timer to switch between threads.
*   Implement the transition to "Ring 3" (User Mode) and write system calls (syscalls).

## 5. Engineering Maturity & Production Practices
*   **Host-side Unit Testing:** Structure your code so hardware-independent logic (allocators, data structures, formatting) can be compiled into a standard Linux binary and tested with frameworks like GoogleTest or Catch2.
*   **Clean Architecture:** Implement a Hardware Abstraction Layer (HAL) to isolate x86-specific code from core kernel logic (e.g., separating `arch/x86/` from generic drivers).
*   **Documentation:** Write design docs that explain the *trade-offs* you made. Elite engineers care about *why* you chose a design.

---

## Immediate Action Items
1. **Serial Driver:** Finish the COM1 polling driver to ensure you have a reliable debugging output independent of the VGA screen.
2. **GDT & IDT:** Map out the Global and Interrupt Descriptor Tables so you can catch CPU exceptions instead of just crashing silently.
3. **Physical Memory:** Begin planning and implementing physical memory allocation.

cpu emulator might be a fun next project?


tried using the libc++ freestanding stuff but it was failin, particularly around <atomic>
needed to cross compile the sysroot stuff libc++ 
atomic apparently needs string.h which isn't included yb default? would need to copy or reimplement

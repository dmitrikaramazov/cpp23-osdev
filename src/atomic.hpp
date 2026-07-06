

class Spinlock {
private:
	bool locked = false;
public:
	void lock(){
		while(__atomic_test_and_set(&locked, __ATOMIC_ACQUIRE)){
			asm volatile("pause");
		}
	}
	void unlock(){
		__atomic_clear(&locked, __ATOMIC_RELEASE);
	}
};


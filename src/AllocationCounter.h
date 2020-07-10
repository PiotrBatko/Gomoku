#ifndef _ALLOCATION_COUNTER_H_
#define _ALLOCATION_COUNTER_H_

// This class holds count of memory heap allocations. Each "new" call increases
// counter when each "delete" and "delete[]" decreases counter.
class AllocationCounter {
public:
	static void IncrementCounter();
	static void DecrementCounter();
	static unsigned long long int GetCounter();

private:
	static unsigned long long int counter;
};

#endif

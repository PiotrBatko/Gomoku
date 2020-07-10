#include "AllocationCounter.h"

unsigned long long int AllocationCounter::counter = 0ULL;

void AllocationCounter::IncrementCounter() {
	counter++;
}

void AllocationCounter::DecrementCounter() {
	counter--;
}

unsigned long long int AllocationCounter::GetCounter() {
	return counter;
}

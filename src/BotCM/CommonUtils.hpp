#ifndef _COMMON_UTILS_H_
#define _COMMON_UTILS_H_

#include "DebugInfo.hpp"
#include "AllocationCounter.hpp"

#define NEW(ptr) \
	new ptr; \
	AllocationCounter::IncrementCounter();

#define SAFE_DELETE(ptr) \
	if (ptr) {			 \
		delete ptr;		 \
		ptr = nullptr;	 \
		AllocationCounter::DecrementCounter(); \
	} else { \
		LOG_ERROR("SAFE_DELETE(): Trying to delete nullptr pointer."); \
	}

#define SAFE_DELETE_ARRAY(ptr) \
	if (ptr) {			 \
		delete [] ptr;	 \
		ptr = nullptr;	 \
		AllocationCounter::DecrementCounter(); \
	} else { \
		LOG_ERROR("SAFE_DELETE_ARRAY(): Trying to delete nullptr pointer."); \
	}

#endif // _COMMON_UTILS_H_

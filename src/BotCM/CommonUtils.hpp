#ifndef _COMMON_UTILS_H_
#define _COMMON_UTILS_H_

#include "DebugInfo.hpp"
#include "AllocationCounter.hpp"

class Coordinates;
class Board;

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

namespace CM {

bool IsFieldOnBoardAndIsEmpty(const Coordinates& coordinates, const Board* board);
std::size_t CalculateCoordinatesDistance(const Coordinates& a, const Coordinates& b);

}

#endif // _COMMON_UTILS_H_

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <time.h>
#include <climits>

// This class represents randomizer for integer numbers. It is used Uniform Int Distribution randomizer from standard C++ library.
class Random {
	public:
		static void Initialize();

		// Randomize unsigned integer from scope <0; max - 1>.
		static unsigned int RandomizeInt(unsigned int max);
		// Randomize float from scope <0; max).
		static float RandomizeFloat(float max);
		// Randomize double from scope <0; max).
		static double RandomizeDouble(double max);
		// Randomize double from scope <min; max).
		static double RandomizeDouble(double min, double max);

	private:
		// Prevent instantiation of this class.
		Random();

		// Engine object for randomizing numbers.
		static std::default_random_engine defaultRandomEngine;
		// Randomizer object.
		static std::uniform_int_distribution<int> uniformIntDistribution;
};

#endif

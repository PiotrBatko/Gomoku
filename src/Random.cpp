#include "Random.h"

std::default_random_engine		   Random::defaultRandomEngine;
std::uniform_int_distribution<int> Random::uniformIntDistribution;

void Random::Initialize() {
	srand((unsigned int)time(NULL));
	uniformIntDistribution = std::uniform_int_distribution<int>(0, INT_MAX);
	defaultRandomEngine.seed((unsigned long)rand());
}

unsigned int Random::RandomizeInt(unsigned int max) {
	return (uniformIntDistribution(defaultRandomEngine) % max);
}

float Random::RandomizeFloat(float max) {
	unsigned int intValue = RandomizeInt(INT_MAX);
	float resultValue = ((float)intValue / INT_MAX) * max;
	return resultValue;
}

double Random::RandomizeDouble(double max) {
	unsigned int intValue = RandomizeInt(INT_MAX);
	double resultValue = ((double)intValue / INT_MAX) * max;
	return resultValue;
}

double Random::RandomizeDouble(double min, double max) {
	double scopeWidth = max - min;
	unsigned int intValue = RandomizeInt(INT_MAX);
	double resultValue = ((double)intValue / INT_MAX) * scopeWidth;
	resultValue += min;
	return resultValue;
}

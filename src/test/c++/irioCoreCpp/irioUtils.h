#pragma once

#include <random>
#include <chrono>


template <typename T>
class IntUniformDistribution{
public:
	IntUniformDistribution(T min = std::numeric_limits<T>::min(), T max =
			std::numeric_limits<T>::max()):
				gen(std::chrono::system_clock::now().time_since_epoch().count()),
				dist(min, max){}

	T getRandom() { return dist(gen); }

private:
	std::mt19937 gen;
	std::uniform_int_distribution<T> dist;
};

#pragma once

#include <array>
#include <algorithm>

class PerlinNoise {

	using PINT = std::uint_fast8_t;

	std::array<PINT, 512> p{ {} };

	PerlinNoise();
	PerlinNoise(const std::uint_fast32_t seed) {
		this->SetSeed(seed);
	}

	void SetSeed(const std::uint_fast32_t seed);

};
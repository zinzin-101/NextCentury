#pragma once
#include <random>

class Random {
	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	public:
		static void Init();
		static float Float();
		static int Int();
};
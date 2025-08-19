#pragma once
#include <intrin.h>
#include <random>




inline __m128 NRReciprocal(__m128 x) {
	__m128 a = _mm_rcp_ps(x);
	__m128 b = _mm_sub_ps(_mm_set1_ps(1.f),_mm_mul_ps(a,x));
	return _mm_add_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(b, b), b), a), a);
}

inline float NRReciprocal(float x) {
	float a = 1.f/x;
	float b = 1.f -(a * x);
	return (((b * b)* b)* a) + a;
}

inline bool caseInsensitiveSearch(const std::string& strHaystack, const std::string& strNeedle)
{
	auto it = std::search(
		strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](unsigned char ch1, unsigned char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
	);
	return (it != strHaystack.end());
}

inline uint64_t randomInt64() {
	static std::mt19937_64 gen(std::random_device{}());
	static std::uniform_int_distribution<uint64_t> dis;
	return dis(gen);
}
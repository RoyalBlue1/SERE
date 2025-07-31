#include <intrin.h>





inline __m128 NRReciprocal(__m128 x) {
	__m128 a = _mm_rcp_ps(x);
	__m128 b = _mm_sub_ps(_mm_set1_ps(1.f),_mm_mul_ps(a,x));
	return _mm_add_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(b, b), b), a), a);
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

#pragma once
#include <immintrin.h>
#include <random>
#include <format>

#ifdef _MSC_VER
typedef int16_t __int16_t;
#endif

struct Vector2 {
	float x;
	float y;
	Vector2(float ix,float iy):x(ix),y(iy){}
	Vector2():x(0.f),y(0.f){}
};

struct Vector3 {
	float x;
	float y;
	float z;
	Vector3(float ix,float iy,float iz):x(ix),y(iy),z(iz){}
	Vector3():x(0.f),y(0.f),z(0.f){}
};

struct Color {
	float red;
	float green;
	float blue;
	float alpha;
	Color(float r,float g,float b,float a):red(r),green(g),blue(b),alpha(a){}
	Color():red(1.f),green(1.f),blue(1.f),alpha(1.f){}
};

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

static inline unsigned char BitScanForwardCompat(unsigned long* index, unsigned long mask)
{
#if defined(_MSC_VER)
    return _BitScanForward(index, mask);
#else
    if (mask == 0)
        return 0;

    *index = static_cast<unsigned long>(__builtin_ctzl(mask));
    return 1;
#endif
}

static inline unsigned char BitScanForward64Compat(unsigned long* index, uint64_t mask)
{
#if defined(_MSC_VER)
    return _BitScanForward64(index, mask);
#else
    if (mask == 0)
        return 0;

    *index = static_cast<unsigned long>(__builtin_ctzll(mask));
    return 1;
#endif
}

static inline unsigned char BitScanReverseCompat(unsigned long* index, unsigned long mask)
{
#if defined(_MSC_VER)
    return _BitScanReverse(index, mask);
#else
    if (mask == 0)
        return 0;

    *index = static_cast<unsigned long>(31u - __builtin_clz(mask));
    return 1;
#endif
}

static inline unsigned char BitTestAndSet64Compat(int64_t* base, long bit)
{
#if defined(_MSC_VER)
    return _bittestandset64(base, bit);
#else
    uint64_t mask = 1ULL << bit;
    unsigned char oldBit = ((*reinterpret_cast<uint64_t*>(base) & mask) != 0);
    *reinterpret_cast<uint64_t*>(base) |= mask;
    return oldBit;
#endif
}


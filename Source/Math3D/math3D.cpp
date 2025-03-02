#include "math3D.h"

#include <xmmintrin.h>
#include <immintrin.h>

vec3f_t add3f(vec3f_t a, vec3f_t b)
{
	__m128 va = _mm_load_ps(a.v);
	__m128 vb = _mm_load_ps(b.v);
	va = _mm_add_ps(va, vb);
	vec3f_t result;
	_mm_store_ps(result.v, va);
	return result;
}

vec3f_t sub3f(vec3f_t a, vec3f_t b)
{
	__m128 va = _mm_load_ps(a.v);
	__m128 vb = _mm_load_ps(b.v);
	va = _mm_sub_ps(va, vb);
	vec3f_t result;
	_mm_store_ps(result.v, va);
	return result;
}

vec3f_t muls3f(vec3f_t a, float s)
{
	__m128 va = _mm_load_ps(a.v);
	__m128 vb = _mm_set_ps(s, s, s, s);
	va = _mm_sub_ps(va, vb);
	vec3f_t result;
	_mm_store_ps(result.v, va);
	return result;
}

float dot3f(vec3f_t a, vec3f_t b)
{
	__m128 va = _mm_load_ps(a.v);
	__m128 vb = _mm_load_ps(b.v);
	va = _mm_dp_ps(va, vb, 0b10001110);
	float result = 0.0f;
	_MM_EXTRACT_FLOAT(result, va, 0);
	return result;
}

//Method 5: Fewer swizzles, swizzle before subtraction https://geometrian.com/resources/cross_product/
inline static __m128 cross_product(
	__m128 const vec0, __m128 const vec1
) {
	__m128 tmp0 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 tmp1 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 tmp2 = _mm_mul_ps(tmp0, vec1);
	__m128 tmp3 = _mm_mul_ps(tmp0, tmp1);
	__m128 tmp4 = _mm_shuffle_ps(tmp2, tmp2, _MM_SHUFFLE(3, 0, 2, 1));
	return _mm_sub_ps(tmp3, tmp4);
}


vec3f_t cross3f(vec3f_t a, vec3f_t b)
{
	__m128 va = _mm_load_ps(a.v);
	__m128 vb = _mm_load_ps(b.v);
	va = cross_product(va, vb);
	vec3f_t result;
	_mm_store_ps(result.v, va);
	return result;
}

mat4x4f_t perspective4x4f(float aspect, float fov, float zNear, float zFar)
{
	return {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

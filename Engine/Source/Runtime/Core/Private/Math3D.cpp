#include "Math3D.h"
#include <xmmintrin.h>
#include <intrin.h>

namespace MarkTech
{
	Vector3 AddV3(Vector3 a, Vector3 b)
	{
		Vector3 result = {};
		__m128 vA = _mm_set_ps(0.0f, a.z, a.y, a.x);
		__m128 vB = _mm_set_ps(0.0f, b.z, b.y, b.x);
		__m128 c = _mm_add_ps(vA, vB);
		
		alignas(16) float memory[4] = {};
		_mm_store_ps(memory, c);
		result.x = memory[0];
		result.y = memory[1];
		result.z = memory[2];
		return result;
	}

	Vector3 SubtractV3(Vector3 a, Vector3 b)
	{
		Vector3 result = {};
		__m128 vA = _mm_set_ps(0.0f, a.z, a.y, a.x);
		__m128 vB = _mm_set_ps(0.0f, b.z, b.y, b.x);
		__m128 c = _mm_sub_ps(vA, vB);

		alignas(16) float memory[4] = {};
		_mm_store_ps(memory, c);
		result.x = memory[0];
		result.y = memory[1];
		result.z = memory[2];
		return result;
	}

	Vector3 MultiplyV3(Vector3 a, float scalar)
	{
		Vector3 result = {};
		__m128 vA = _mm_set_ps(0.0f, a.z, a.y, a.x);
		__m128 vB = _mm_set_ps(scalar, scalar, scalar, scalar);
		__m128 c = _mm_mul_ps(vA, vB);

		alignas(16) float memory[4] = {};
		_mm_store_ps(memory, c);
		result.x = memory[0];
		result.y = memory[1];
		result.z = memory[2];
		return result;
	}

	float MagnitudeV3(Vector3 a)
	{
		__m128 v = _mm_set_ps(0.0f, a.z, a.y, a.x);
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v, v, 0x71)));;
	}

	Vector3 NormalizeV3(Vector3 a)
	{
		Vector3 result = {};
		__m128 v = _mm_set_ps(0.0f, a.z, a.y, a.x);
		__m128 norm = _mm_sqrt_ps(_mm_dp_ps(v, v, 0x7F));
		__m128 vresult = _mm_div_ps(v, norm);

		alignas(16) float memory[4] = {};
		_mm_store_ps(memory, vresult);
		result.x = memory[0];
		result.y = memory[1];
		result.z = memory[2];
		return result;
	}

	float DotV3(Vector3 a, Vector3 b)
	{
		__m128 v1 = _mm_set_ps(0.0f, a.z, a.y, a.x);
		__m128 v2 = _mm_set_ps(0.0f, b.z, b.y, b.x);
		__m128 result = _mm_dp_ps(v1, v2, 0x7F);
		alignas(16) float memory[4] = {};
		_mm_store_ps(memory, result);
		return memory[0];
	}

	Vector3 CrossV3(Vector3 a, Vector3 b)
	{
		Vector3 result = {};

		__m128 v1 = _mm_set_ps(0.0f, a.z, a.y, a.x);
		__m128 v2 = _mm_set_ps(0.0f, b.z, b.y, b.x);

		__m128 tmp0 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 tmp1 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2));
		__m128 tmp2 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2));
		__m128 tmp3 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 tmp4 = _mm_sub_ps(_mm_mul_ps(tmp0, tmp1), _mm_mul_ps(tmp2, tmp3));

		alignas(16) float memory[4] = {};
		_mm_store_ps(memory, tmp4);
		result.x = memory[0];
		result.y = memory[1];
		result.z = memory[2];

		return result;
	}

	Vector4 AddV4(Vector4 a, Vector4 b)
	{
		Vector4 result = {};
		__m128 vA = _mm_set_ps(a.w, a.z, a.y, a.x);
		__m128 vB = _mm_set_ps(b.w, b.z, b.y, b.x);
		__m128 c = _mm_add_ps(vA, vB);

		alignas(16) float memory[4] = {};
		_mm_store_ps(memory, c);
		result.x = memory[0];
		result.y = memory[1];
		result.z = memory[2];
		result.w = memory[3];
		return result;
	}

	Vector4 SubtractV4(Vector4 a, Vector4 b)
	{
		Vector4 result = {};
		__m128 vA = _mm_set_ps(a.w, a.z, a.y, a.x);
		__m128 vB = _mm_set_ps(b.w, b.z, b.y, b.x);
		__m128 c = _mm_sub_ps(vA, vB);
		
		alignas(16) float memory[4] = {};
		_mm_store_ps(memory, c);
		result.x = memory[0];
		result.y = memory[1];
		result.z = memory[2];
		result.w = memory[3];
		return result;
	}

	Vector4 MultiplyV4(Vector4 a, float scalar)
	{
		Vector4 result = {};
		__m128 vA = _mm_set_ps(a.w, a.z, a.y, a.x);
		__m128 vB = _mm_set_ps(scalar, scalar, scalar, scalar);
		__m128 c = _mm_mul_ps(vA, vB);
		
		alignas(16) float memory[4] = {};
		_mm_store_ps(memory, c);
		result.x = memory[0];
		result.y = memory[1];
		result.z = memory[2];
		result.w = memory[3];
		return result;
	}

	float MagnitudeV4(Vector4 a)
	{
		__m128 v = _mm_set_ps(a.w, a.z, a.y, a.x);
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(v, v, 0x71)));;
	}

	Vector4 NormalizeV4(Vector4 a)
	{
		Vector4 result = {};
		__m128 v = _mm_set_ps(a.w, a.z, a.y, a.x);
		__m128 norm = _mm_sqrt_ps(_mm_dp_ps(v, v, 0x7F));
		__m128 vresult = _mm_div_ps(v, norm);

		alignas(16) float memory[4] = {};
		_mm_store_ps(memory, vresult);
		result.x = memory[0];
		result.y = memory[1];
		result.z = memory[2];
		result.w = memory[3];
		return result;
	}
}
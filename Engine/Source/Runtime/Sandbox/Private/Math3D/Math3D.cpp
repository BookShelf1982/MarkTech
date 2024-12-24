#include "Math3D.h"

#include <xmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

#define MATRIX4X4_INDEX(row, col) ((col * 4) + row)

namespace MarkTech
{
#ifdef MT_DOUBLE_PRECISION
#else
	Vector2 MultiplyByScalarV2(const Vector2& a, Number s)
	{
		Vector2 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.x, a.y);
		__m128 vs = _mm_set_ps(s, s, s, s);
		va = _mm_mul_ps(va, vs);
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		return v;
	}

	Vector2 AddV2(const Vector2& a, const Vector2& b)
	{
		Vector2 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.x, a.y);
		__m128 vb = _mm_set_ps(b.x, b.y, b.x, b.y);
		va = _mm_add_ps(va, vb);
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		return v;
	}

	Vector2 SubtractV2(const Vector2& a, const Vector2& b)
	{
		Vector2 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.x, a.y);
		__m128 vb = _mm_set_ps(b.x, b.y, b.x, b.y);
		va = _mm_sub_ps(va, vb);
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		return v;
	}

	Number MagnitudeV2(const Vector2& a)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.x, a.y);
		va = _mm_sqrt_ps(_mm_dp_ps(va, va, 0xCC));
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);;
		return result.f;
	}

	Number SquaredMagnitudeV2(const Vector2& a)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.x, a.y);
		va = _mm_dp_ps(va, va, 0xFF);
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);;
		return result.f;
	}

	Vector2 NormalizeV2(const Vector2& a)
	{
		Vector2 norm;
		__m128 va = _mm_set_ps(a.x, a.y, a.x, a.y);
		va = _mm_div_ps(va, _mm_sqrt_ps(_mm_dp_ps(va, va, 0xCC)));
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		norm.x = result[3];
		norm.y = result[2];
		return norm;
	}

	Number DotProductV2(const Vector2& a, const Vector2& b)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.x, a.y);
		__m128 vb = _mm_set_ps(b.x, b.y, b.x, b.y);
		va = _mm_dp_ps(va, vb, 0xC8);
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);;
		return result.f;
	}

	Vector3 MultiplyByScalarV3(const Vector3& a, Number s)
	{
		Vector3 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.y);
		__m128 vs = _mm_set_ps(s, s, s, s);
		va = _mm_mul_ps(va, vs);
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		v.z = result[1];
		return v;
	}

	Vector3 AddV3(const Vector3& a, const Vector3& b)
	{
		Vector3 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.y);
		__m128 vb = _mm_set_ps(b.x, b.y, b.z, b.y);
		va = _mm_add_ps(va, vb);
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		v.z = result[1];
		return v;
	}

	Vector3 SubtractV3(const Vector3& a, const Vector3& b)
	{
		Vector3 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.x);
		__m128 vb = _mm_set_ps(b.x, b.y, b.z, b.x);
		va = _mm_sub_ps(va, vb);
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		v.z = result[1];
		return v;
	}

	Number MagnitudeV3(const Vector3& a)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.x);
		va = _mm_sqrt_ps(_mm_dp_ps(va, va, 0xEE));
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);
		return result.f;
	}

	Number SquaredMagnitudeV3(const Vector3& a)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.x);
		va = _mm_dp_ps(va, va, 0xEE);
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);
		return result.f;
	}

	Vector3 NormalizeV3(const Vector3& a)
	{
		Vector3 v;
		__m128 va = _mm_set_ps(a.x, a.y, a.x, a.y);
		va = _mm_div_ps(va, _mm_sqrt_ps(_mm_dp_ps(va, va, 0xEE)));
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		v.z = result[1];
		return v;
	}

	Number DotProductV3(const Vector3& a, const Vector3& b)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.x, a.y);
		__m128 vb = _mm_set_ps(b.x, b.y, b.x, b.y);
		va = _mm_dp_ps(va, vb, 0xEE);
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);;
		return result.f;
	}

	// function from: https://geometrian.com/programming/tutorials/cross-product/index.php
	inline static __m128 cross_product(__m128 const& vec0, __m128 const& vec1) {
		__m128 tmp0 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 tmp1 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 1, 0, 2));
		__m128 tmp2 = _mm_mul_ps(tmp0, vec1);
		__m128 tmp3 = _mm_mul_ps(tmp0, tmp1);
		__m128 tmp4 = _mm_shuffle_ps(tmp2, tmp2, _MM_SHUFFLE(3, 0, 2, 1));
		return _mm_sub_ps(tmp3, tmp4);
	}

	Vector3 CrossProductV3(const Vector3& a, const Vector3& b)
	{
		Vector3 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.x);
		__m128 vb = _mm_set_ps(b.x, b.y, b.z, b.x);

		va = cross_product(va, vb);

		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		v.z = result[1];
		return v;
	}

	Vector3 LerpV3(const Vector3& a, const Vector3& b, Number s)
	{
		Vector3 v = {};
		float weight = 1.0f - s;
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.x);
		__m128 vb = _mm_set_ps(b.x, b.y, b.z, b.x);
		__m128 vsa = _mm_set_ps(weight, weight, weight, weight);
		__m128 vsb = _mm_set_ps(s, s, s, s);
		va = _mm_add_ps(_mm_mul_ps(vsa, va), _mm_mul_ps(vsb, vb));

		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		v.z = result[1];
		return v;
	}

	Vector4 MultiplyByScalarV4(const Vector4& a, Number s)
	{
		Vector4 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.w);
		__m128 vs = _mm_set_ps(s, s, s, s);
		va = _mm_mul_ps(va, vs);
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		v.z = result[1];
		v.w = result[0];
		return v;
	}

	Vector4 AddV4(const Vector4& a, const Vector4& b)
	{
		Vector4 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.w);
		__m128 vb = _mm_set_ps(b.x, b.y, b.z, b.w);
		va = _mm_add_ps(va, vb);
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		v.z = result[1];
		v.w = result[0];
		return v;
	}

	Vector4 SubtractV4(const Vector4& a, const Vector4& b)
	{
		Vector4 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.w);
		__m128 vb = _mm_set_ps(b.x, b.y, b.z, b.w);
		va = _mm_sub_ps(va, vb);
		alignas(16) Number result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[3];
		v.y = result[2];
		v.z = result[1];
		return v;
	}

	Matrix4x4 Identity4x4()
	{
		return { 
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	Matrix4x4 Transpose4x4(const Matrix4x4& a)
	{
		Matrix4x4 M = {};
		__m128 Mr1 = _mm_set_ps(a.m11, a.m12, a.m13, a.m14);
		__m128 Mr2 = _mm_set_ps(a.m21, a.m22, a.m23, a.m24);
		__m128 Mr3 = _mm_set_ps(a.m31, a.m32, a.m33, a.m34);
		__m128 Mr4 = _mm_set_ps(a.m41, a.m42, a.m43, a.m44);
		_MM_TRANSPOSE4_PS(Mr1, Mr2, Mr3, Mr4);
		alignas(16) Number result[16];
		_mm_store_ps(result, Mr4);
		_mm_store_ps(result + 4, Mr3);
		_mm_store_ps(result + 8, Mr2);
		_mm_store_ps(result + 12, Mr1);
		for (U32 i = 0; i < 16; i++)
			((float*)&M)[i] = result[i];

		return M;
	}

	Matrix4x4 Multiply4x4(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 M = {};

		for (U32 i = 0; i < 4; i++)
		{
			for (U32 j = 0; j < 4; j++)
			{
				__m128 aRow = _mm_set_ps(
					((float*)&a)[MATRIX4X4_INDEX(i, 0)],
					((float*)&a)[MATRIX4X4_INDEX(i, 1)],
					((float*)&a)[MATRIX4X4_INDEX(i, 2)],
					((float*)&a)[MATRIX4X4_INDEX(i, 3)]
				);

				__m128 bCol = _mm_set_ps(
					((float*)&b)[MATRIX4X4_INDEX(0, j)],
					((float*)&b)[MATRIX4X4_INDEX(1, j)],
					((float*)&b)[MATRIX4X4_INDEX(2, j)],
					((float*)&b)[MATRIX4X4_INDEX(3, j)]
				);

				_MM_EXTRACT_FLOAT(((float*)&M)[MATRIX4X4_INDEX(i, j)], _mm_dp_ps(aRow, bCol, 0xFF), 3);
			}
		}

		return M;
	}
	Matrix4x4 Traslation4x4(Number tx, Number ty, Number tz)
	{
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			tx, ty, tz, 1.0f
		};
	}

	Matrix4x4 Scale4x4(Number sx, Number sy, Number sz)
	{
		return {
			sx, 0.0f, 0.0f, 0.0f,
			0.0f, sy, 0.0f, 0.0f,
			0.0f, 0.0f, sz, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
#endif
}
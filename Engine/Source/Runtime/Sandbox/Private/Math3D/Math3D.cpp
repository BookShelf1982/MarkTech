#include "Math3D.h"

#include <xmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>
#include <math.h>

#define MATRIX4X4_INDEX(row, col) ((col * 4) + row)
#define MATRIX3X3_INDEX(row, col) ((col * 3) + row)

namespace MarkTech
{
	float ToRadians(float degree)
	{
		return degree * (PI / 180);
	}

	float ToDegrees(float radian)
	{
		return radian * (180 / PI);
	}

	Vector2 MultiplyByScalarV2(const Vector2& a, float s)
	{
		Vector2 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.y, a.x);
		__m128 vs = _mm_set_ps(s, s, s, s);
		va = _mm_mul_ps(va, vs);
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[1];
		return v;
	}

	Vector2 AddV2(const Vector2& a, const Vector2& b)
	{
		Vector2 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.y, a.x);
		__m128 vb = _mm_set_ps(b.x, b.y, b.y, b.x);
		va = _mm_add_ps(va, vb);
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[1];
		return v;
	}

	Vector2 SubtractV2(const Vector2& a, const Vector2& b)
	{
		Vector2 v = {};
		__m128 va = _mm_set_ps(a.x, a.y, a.y, a.x);
		__m128 vb = _mm_set_ps(b.x, b.y, b.y, b.x);
		va = _mm_sub_ps(va, vb);
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[1];
		return v;
	}

	float MagnitudeV2(const Vector2& a)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.y, a.x);
		va = _mm_sqrt_ps(_mm_dp_ps(va, va, 0xCC));
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);;
		return result.f;
	}

	float SquaredMagnitudeV2(const Vector2& a)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.y, a.x);
		va = _mm_dp_ps(va, va, 0xFF);
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);;
		return result.f;
	}

	Vector2 NormalizeV2(const Vector2& a)
	{
		if (a.x == 0.0f && a.y == 0.0f)
			return a;

		Vector2 norm;
		__m128 va = _mm_set_ps(a.y, a.x, a.y, a.x);
		va = _mm_div_ps(va, _mm_sqrt_ps(_mm_dp_ps(va, va, 0x33)));
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		norm.x = result[0];
		norm.y = result[1];
		return norm;
	}

	float DotProductV2(const Vector2& a, const Vector2& b)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.y, a.x);
		__m128 vb = _mm_set_ps(b.x, b.y, b.y, b.x);
		va = _mm_dp_ps(va, vb, 0xC8);
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);;
		return result.f;
	}

	Vector2 LerpV2(const Vector2& a, const Vector2& b, float s)
	{
		Vector2 v = {};
		float weight = 1.0f - s;
		__m128 va = _mm_set_ps(a.y, a.x, a.y, a.x);
		__m128 vb = _mm_set_ps(b.y, b.x, b.y, b.x);
		__m128 vsa = _mm_set_ps(weight, weight, weight, weight);
		__m128 vsb = _mm_set_ps(s, s, s, s);
		va = _mm_add_ps(_mm_mul_ps(vsa, va), _mm_mul_ps(vsb, vb));

		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[1];
		return v;
	}

	Vector3 MultiplyByScalarV3(const Vector3& a, float s)
	{
		Vector3 v = {};
		__m128 va = _mm_set_ps(a.x, a.z, a.y, a.x);
		__m128 vs = _mm_set_ps(s, s, s, s);
		va = _mm_mul_ps(va, vs);
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[1];
		v.z = result[2];
		return v;
	}

	Vector3 AddV3(const Vector3& a, const Vector3& b)
	{
		Vector3 v = {};
		__m128 va = _mm_set_ps(a.x, a.z, a.y, a.x);
		__m128 vb = _mm_set_ps(b.x, b.z, b.y, b.x);
		va = _mm_add_ps(va, vb);
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[1];
		v.z = result[2];
		return v;
	}

	Vector3 SubtractV3(const Vector3& a, const Vector3& b)
	{
		Vector3 v = {};
		__m128 va = _mm_set_ps(a.x, a.z, a.y, a.x);
		__m128 vb = _mm_set_ps(b.x, b.z, b.y, b.x);
		va = _mm_sub_ps(va, vb);
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[1];
		v.z = result[2];
		return v;
	}

	float MagnitudeV3(const Vector3& a)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.x);
		va = _mm_sqrt_ps(_mm_dp_ps(va, va, 0x77));
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);
		return result.f;
	}

	float SquaredMagnitudeV3(const Vector3& a)
	{
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.x);
		va = _mm_dp_ps(va, va, 0x77);
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);
		return result.f;
	}

	Vector3 NormalizeV3(const Vector3& a)
	{
		Vector3 v;
		__m128 va = _mm_set_ps(a.x, a.z, a.y, a.x);
		va = _mm_div_ps(va, _mm_sqrt_ps(_mm_dp_ps(va, va, 0x77)));
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[1];
		v.z = result[2];
		return v;
	}

	float DotProductV3(const Vector3& a, const Vector3& b)
	{
		__m128 va = _mm_set_ps(a.x, a.z, a.y, a.x);
		__m128 vb = _mm_set_ps(b.x, b.z, b.y, b.x);
		va = _mm_dp_ps(va, vb, 0x77);
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
		__m128 va = _mm_set_ps(a.x, a.z, a.y, a.x);
		__m128 vb = _mm_set_ps(b.x, b.z, b.y, b.x);

		va = cross_product(va, vb);

		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[1];
		v.z = result[2];
		return v;
	}

	Vector3 LerpV3(const Vector3& a, const Vector3& b, float s)
	{
		Vector3 v = {};
		float weight = 1.0f - s;
		__m128 va = _mm_set_ps(a.x, a.y, a.z, a.x);
		__m128 vb = _mm_set_ps(b.x, b.y, b.z, b.x);
		__m128 vsa = _mm_set_ps(weight, weight, weight, weight);
		__m128 vsb = _mm_set_ps(s, s, s, s);
		va = _mm_add_ps(_mm_mul_ps(vsa, va), _mm_mul_ps(vsb, vb));

		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[2];
		v.z = result[1];
		return v;
	}

	Vector4 MultiplyByScalarV4(const Vector4& a, float s)
	{
		Vector4 v = {};
		__m128 va = _mm_set_ps(a.w, a.z, a.y, a.x);
		__m128 vs = _mm_set_ps(s, s, s, s);
		va = _mm_mul_ps(va, vs);
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[2];
		v.z = result[1];
		v.w = result[0];
		return v;
	}

	Vector4 AddV4(const Vector4& a, const Vector4& b)
	{
		Vector4 v = {};
		__m128 va = _mm_set_ps(a.w, a.z, a.y, a.x);
		__m128 vb = _mm_set_ps(b.w, b.z, b.y, b.x);
		va = _mm_add_ps(va, vb);
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
		v.y = result[2];
		v.z = result[1];
		v.w = result[3];
		return v;
	}

	Vector4 SubtractV4(const Vector4& a, const Vector4& b)
	{
		Vector4 v = {};
		__m128 va = _mm_set_ps(a.w, a.z, a.y, a.x);
		__m128 vb = _mm_set_ps(b.w, b.z, b.y, b.x);
		va = _mm_sub_ps(va, vb);
		alignas(16) float result[4] = {};
		_mm_store_ps(result, va);
		v.x = result[0];
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
		return {
			a.m11, a.m21, a.m31, a.m41,
			a.m12, a.m22, a.m32, a.m42,
			a.m13, a.m23, a.m33, a.m43,
			a.m14, a.m24, a.m34, a.m44
		};
	}

	Matrix4x4 Multiply4x4(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 M = {};

		for (U32 i = 0; i < 4; i++)
		{
			for (U32 j = 0; j < 4; j++)
			{
				__m128 aRow = _mm_set_ps(
					((float*)&a)[MATRIX4X4_INDEX(0, i)],
					((float*)&a)[MATRIX4X4_INDEX(1, i)],
					((float*)&a)[MATRIX4X4_INDEX(2, i)],
					((float*)&a)[MATRIX4X4_INDEX(3, i)]
				);

				__m128 bCol = _mm_set_ps(
					((float*)&b)[MATRIX4X4_INDEX(j, 0)],
					((float*)&b)[MATRIX4X4_INDEX(j, 1)],
					((float*)&b)[MATRIX4X4_INDEX(j, 2)],
					((float*)&b)[MATRIX4X4_INDEX(j, 3)]
				);

				_MM_EXTRACT_FLOAT(((float*)&M)[MATRIX4X4_INDEX(j, i)], _mm_dp_ps(aRow, bCol, 0x77), 3);
			}
		}

		return M;
	}

	Matrix4x4 Translate4x4(float tx, float ty, float tz)
	{
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			tx, ty, tz, 1.0f
		};
	}

	Matrix4x4 RatateZAxis4x4(float angle)
	{
		float sine = sinf(angle);
		float cosine = cosf(angle);
		return {
			cosine, -sine, 0.0f, 0.0f,
			sine, cosine, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	Matrix4x4 Scale4x4(float sx, float sy, float sz)
	{
		return {
			sx, 0.0f, 0.0f, 0.0f,
			0.0f, sy, 0.0f, 0.0f,
			0.0f, 0.0f, sz, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	Matrix4x4 LookTo4x4(Vector3 pos, Vector3 up, Vector3 lookAt)
	{
		Vector3 zaxis = NormalizeV3(SubtractV3(pos, lookAt));
		Vector3 xaxis = NormalizeV3(CrossProductV3(up, zaxis));
		Vector3 yaxis = CrossProductV3(zaxis, xaxis);

		return {
			xaxis.x, yaxis.x, zaxis.x, 0.0f,
			xaxis.y, yaxis.y, zaxis.y, 0.0f,
			xaxis.z, yaxis.z, zaxis.z, 0.0f,
			-DotProductV3(xaxis, pos), -DotProductV3(yaxis, pos), -DotProductV3(zaxis, pos), 1.0f,
		};
	}

	Matrix4x4 OrthoProjection4x4(float l, float r, float t, float b, float n, float f)
	{

		return {
			2/(r - l), 0.0f, 0.0f, -(r + l) / (r - l),
			0.0f, 2 / (t - b), 0.0f, -(t + b) / (t - b),
			0.0f, 0.0f, -2 / (f - n), -(f + n) / (f - n),
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	Matrix4x4 operator*(Matrix4x4 a, Matrix4x4 b)
	{
		return Multiply4x4(a, b);
	}

	Matrix3x3 Identity3x3()
	{
		return {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
	}

	Matrix3x3 Transpose3x3(const Matrix3x3& a)
	{
		return {
			a.m11, a.m21, a.m31,
			a.m12, a.m22, a.m32,
			a.m13, a.m23, a.m33
		};
	}

	Matrix3x3 Multiply3x3(const Matrix3x3& a, const Matrix3x3& b)
	{
		Matrix3x3 M = {};

		for (U32 i = 0; i < 3; i++)
		{
			for (U32 j = 0; j < 3; j++)
			{
				__m128 aRow = _mm_set_ps(
					((float*)&a)[MATRIX3X3_INDEX(0, i)],
					((float*)&a)[MATRIX3X3_INDEX(1, i)],
					((float*)&a)[MATRIX3X3_INDEX(2, i)],
					0.0f
				);

				__m128 bCol = _mm_set_ps(
					((float*)&b)[MATRIX3X3_INDEX(j, 0)],
					((float*)&b)[MATRIX3X3_INDEX(j, 1)],
					((float*)&b)[MATRIX3X3_INDEX(j, 2)],
					0.0f
				);

				_MM_EXTRACT_FLOAT(((float*)&M)[MATRIX3X3_INDEX(j, i)], _mm_dp_ps(aRow, bCol, 0xFF), 0);
			}
		}

		return M;
	}

	Matrix3x3 Translate3x3(float tx, float ty)
	{
		return {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			tx, ty, 1.0f
		};
	}

	Matrix3x3 Scale3x3(float sx, float sy)
	{
		return {
			sx, 0.0f, 0.0f,
			0.0f, sy, 0.0f,
			0.0f, 0.0f, 1.0f
		};
	}

	Matrix3x3 OrthoProjection3x3(float l, float r, float b, float t)
	{
		return {
			2.0f / (r - l), 0.0f, 0.0f,
			0.0f, 2.0f / (t - b), 0.0f,
			-(r + l) / (r - l), -(t + b) / (t - b), 1.0f
		};
	}

	Matrix3x3 operator*(Matrix3x3 a, Matrix3x3 b)
	{
		return Multiply3x3(a, b);
	}
}
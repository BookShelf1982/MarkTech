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
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vs = _mm_set_ps(s, s, s, s);
		va = _mm_mul_ps(va, vs);
		_mm_store_ps((float*)&v, va);
		return v;
	}

	Vector2 AddV2(const Vector2& a, const Vector2& b)
	{
		Vector2 v = {};
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
		va = _mm_add_ps(va, vb);
		_mm_store_ps((float*)&v, va);
		return v;
	}

	Vector2 SubtractV2(const Vector2& a, const Vector2& b)
	{
		Vector2 v = {};
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
		va = _mm_sub_ps(va, vb);
		_mm_store_ps((float*)&v, va);
		return v;
	}

	float MagnitudeV2(const Vector2& a)
	{
		__m128 va = _mm_load_ps((float*)&a);
		va = _mm_sqrt_ps(_mm_dp_ps(va, va, 0b00110001));
		
		float result = 0.0f;
		_MM_EXTRACT_FLOAT(result, va, 0);
		return result;
	}

	float SquaredMagnitudeV2(const Vector2& a)
	{
		__m128 va = _mm_load_ps((float*)&a);
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
		__m128 va = _mm_load_ps((float*)&a);
		va = _mm_div_ps(va, _mm_sqrt_ps(_mm_dp_ps(va, va, 0xFF)));
		_mm_store_ps((float*)&norm, va);
		return norm;
	}

	float DotProductV2(const Vector2& a, const Vector2& b)
	{
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
		va = _mm_dp_ps(va, vb, 0xC8);
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);;
		return result.f;
	}

	Vector2 LerpV2(const Vector2& a, const Vector2& b, float s)
	{
		Vector2 v = {};
		float weight = 1.0f - s;
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
		__m128 vsa = _mm_set_ps(weight, weight, weight, weight);
		__m128 vsb = _mm_set_ps(s, s, s, s);
		va = _mm_add_ps(_mm_mul_ps(vsa, va), _mm_mul_ps(vsb, vb));

		_mm_store_ps((float*)&v, va);
		return v;
	}

	Vector2 operator*(const Vector2& a, float s)
	{
		return MultiplyByScalarV2(a, s);
	}

	Vector3 MultiplyByScalarV3(const Vector3& a, float s)
	{
		Vector3 v = {};
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vs = _mm_set_ps(s, s, s, s);
		va = _mm_mul_ps(va, vs);
		_mm_store_ps((float*)&v, va);
		return v;
	}

	Vector3 AddV3(const Vector3& a, const Vector3& b)
	{
		Vector3 v = {};
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
		va = _mm_add_ps(va, vb);
		_mm_store_ps((float*)&v, va);
		return v;
	}

	Vector3 SubtractV3(const Vector3& a, const Vector3& b)
	{
		Vector3 v = {};
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
		va = _mm_sub_ps(va, vb);
		_mm_store_ps((float*)&v, va);
		return v;
	}

	float MagnitudeV3(const Vector3& a)
	{
		__m128 va = _mm_load_ps((float*)&a);
		va = _mm_sqrt_ps(_mm_dp_ps(va, va, 0x77));
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);
		return result.f;
	}

	float SquaredMagnitudeV3(const Vector3& a)
	{
		__m128 va = _mm_load_ps((float*)&a);
		va = _mm_dp_ps(va, va, 0x77);
		I32F32 result;
		result.i = _mm_extract_ps(va, 3);
		return result.f;
	}

	Vector3 NormalizeV3(const Vector3& a)
	{
		Vector3 v;
		__m128 va = _mm_load_ps((float*)&a);
		va = _mm_div_ps(va, _mm_sqrt_ps(_mm_dp_ps(va, va, 0x77)));
		_mm_store_ps((float*)&v, va);
		return v;
	}

	float DotProductV3(const Vector3& a, const Vector3& b)
	{
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
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
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
		va = cross_product(va, vb);

		_mm_store_ps((float*)&v, va);
		return v;
	}

	Vector3 LerpV3(const Vector3& a, const Vector3& b, float s)
	{
		Vector3 v = {};
		float weight = 1.0f - s;
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
		__m128 vsa = _mm_set_ps(weight, weight, weight, weight);
		__m128 vsb = _mm_set_ps(s, s, s, s);
		va = _mm_add_ps(_mm_mul_ps(vsa, va), _mm_mul_ps(vsb, vb));

		_mm_store_ps((float*)&v, va);
		return v;
	}

	Vector4 MultiplyByScalarV4(const Vector4& a, float s)
	{
		Vector4 v = {};
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vs = _mm_set_ps(s, s, s, s);
		va = _mm_mul_ps(va, vs);

		_mm_store_ps((float*)&v, va);
		return v;
	}

	Vector4 AddV4(const Vector4& a, const Vector4& b)
	{
		Vector4 v = {};
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
		va = _mm_add_ps(va, vb);

		_mm_store_ps((float*)&v, va);
		return v;
	}

	Vector4 SubtractV4(const Vector4& a, const Vector4& b)
	{
		Vector4 v = {};
		__m128 va = _mm_load_ps((float*)&a);
		__m128 vb = _mm_load_ps((float*)&b);
		va = _mm_sub_ps(va, vb);

		_mm_store_ps((float*)&v, va);
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
	
	/*Matrix4x4 OLDMultiply4x4(const Matrix4x4& a, const Matrix4x4& b)
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
	}*/

	Matrix4x4 Multiply4x4(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 M = {};
		__m128 aRows[4];
		for (U32 i = 0; i < 4; i++)
			aRows[i] = _mm_load_ps((float*)&a + (4 * i));

		_MM_TRANSPOSE4_PS(aRows[0], aRows[1], aRows[2], aRows[3]);

		__m128 bColumns[4];
		for (U32 i = 0; i < 4; i++)
			bColumns[i] = _mm_load_ps((float*)&b + (4 * i));

		for (U32 i = 0; i < 4; i++)
		{
			for (U32 j = 0; j < 4; j++)
			{
				__m128 dp = _mm_dp_ps(aRows[i], bColumns[j], 0b11110001);
				_MM_EXTRACT_FLOAT(((float*)&M)[(i * 4) + j], dp, 0);
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
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f
		};
	}

	Matrix3x3 Transpose3x3(const Matrix3x3& a)
	{
		return {
			a.m11, a.m21, a.m31, 0.0f,
			a.m12, a.m22, a.m32, 0.0f,
			a.m13, a.m23, a.m33, 0.0f
		};
	}

	Matrix3x3 Multiply3x3(const Matrix3x3& a, const Matrix3x3& b)
	{
		Matrix3x3 M = {};
		Matrix3x3 Mt = Transpose3x3(a);

		__m128 aRows[3];
		for (U32 i = 0; i < 3; i++)
			aRows[i] = _mm_load_ps((float*)&Mt + (4 * i));

		__m128 bColumns[3];
		for (U32 i = 0; i < 3; i++)
			bColumns[i] = _mm_load_ps((float*)&b + (4 * i));

		for (U32 i = 0; i < 3; i++)
		{
			for (U32 j = 0; j < 3; j++)
			{
				__m128 dp = _mm_dp_ps(aRows[j], bColumns[i], 0b01111111);
				_MM_EXTRACT_FLOAT(((float*)&M)[(i * 4) + j], dp, 0);
			}
		}

		return M;
	}

	Matrix3x3 Translate3x3(float tx, float ty)
	{
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			tx, ty, 1.0f, 0.0f
		};
	}

	Matrix3x3 Scale3x3(float sx, float sy)
	{
		return {
			sx, 0.0f, 0.0f, 0.0f,
			0.0f, sy, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f
		};
	}

	Matrix3x3 OrthoProjection3x3(float l, float r, float b, float t)
	{
		return {
			2.0f / (r - l), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / (t - b), 0.0f, 0.0f,
			-(r + l) / (r - l), -(t + b) / (t - b), 1.0f, 0.0f
		};
	}

	Matrix3x3 operator*(Matrix3x3 a, Matrix3x3 b)
	{
		return Multiply3x3(a, b);
	}
}
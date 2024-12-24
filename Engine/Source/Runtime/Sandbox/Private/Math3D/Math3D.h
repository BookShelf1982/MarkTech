#pragma once
#include "PrimitiveTypes\intalias.h"

namespace MarkTech
{
#ifdef MT_DOUBLE_PRECISION
	typedef double Number;
#else
	typedef float Number;
#endif

	struct Vector2
	{
		Number x, y;
	};

	Vector2 MultiplyByScalarV2(const Vector2& a, Number s);
	Vector2 AddV2(const Vector2& a, const Vector2& b);
	Vector2 SubtractV2(const Vector2& a, const Vector2& b);
	Number MagnitudeV2(const Vector2& a);
	Number SquaredMagnitudeV2(const Vector2& a);
	Vector2 NormalizeV2(const Vector2& a);
	Number DotProductV2(const Vector2& a, const Vector2& b);

	struct Vector3
	{
		Number x, y, z;
	};

	Vector3 MultiplyByScalarV3(const Vector3& a, Number s);
	Vector3 AddV3(const Vector3& a, const Vector3& b);
	Vector3 SubtractV3(const Vector3& a, const Vector3& b);
	Number MagnitudeV3(const Vector3& a);
	Number SquaredMagnitudeV3(const Vector3& a);
	Vector3 NormalizeV3(const Vector3& a);
	Number DotProductV3(const Vector3& a, const Vector3& b);
	Vector3 CrossProductV3(const Vector3& a, const Vector3& b);
	Vector3 LerpV3(const Vector3& a, const Vector3& b, Number s);

	struct Vector4
	{
		float x, y, z, w;
	};

	Vector4 MultiplyByScalarV4(const Vector4& a, Number s);
	Vector4 AddV4(const Vector4& a, const Vector4& b);
	Vector4 SubtractV4(const Vector4& a, const Vector4& b);
	Number MagnitudeV4(const Vector4& a);
	Number SquaredMagnitudeV4(const Vector4& a);
	Vector4 NormalizeV4(const Vector4& a);
	Number DotProductV4(const Vector4& a, const Vector4& b);
	Vector4 CrossProductV4(const Vector4& a, const Vector4& b);
	Vector4 LerpV4(const Vector4& a, const Vector4& b, Number s);

	struct Matrix4x4
	{
		Number m11, m12, m13, m14;
		Number m21, m22, m23, m24;
		Number m31, m32, m33, m34;
		Number m41, m42, m43, m44;
	};

	Matrix4x4 Identity4x4();
	Matrix4x4 Transpose4x4(const Matrix4x4& a);
	Matrix4x4 Multiply4x4(const Matrix4x4& a, const Matrix4x4& b);
	Matrix4x4 Traslation4x4(Number tx, Number ty, Number tz);
	Matrix4x4 Scale4x4(Number sx, Number sy, Number sz);
	
	struct Matrix3x3
	{
		Number m11, m12, m13;
		Number m21, m22, m23;
		Number m31, m32, m33;
	};
}
#pragma once
#include "PrimitiveTypes\intalias.h"

namespace MarkTech
{
#define PI 3.14159265358979323846f
	float ToRadians(float degree);
	float ToDegrees(float radian);

	struct alignas(16) Vector2
	{
		float x, y;
		float pad[2];
	};

	Vector2 MultiplyByScalarV2(const Vector2& a, float s);
	Vector2 AddV2(const Vector2& a, const Vector2& b);
	Vector2 SubtractV2(const Vector2& a, const Vector2& b);
	float MagnitudeV2(const Vector2& a);
	float SquaredMagnitudeV2(const Vector2& a);
	Vector2 NormalizeV2(const Vector2& a);
	float DotProductV2(const Vector2& a, const Vector2& b);
	Vector2 LerpV2(const Vector2& a, const Vector2& b, float s);
	Vector2 operator*(const Vector2& a, float s);

	struct alignas(16) Vector3
	{
		float x, y, z, pad;
	};

	Vector3 MultiplyByScalarV3(const Vector3& a, float s);
	Vector3 AddV3(const Vector3& a, const Vector3& b);
	Vector3 SubtractV3(const Vector3& a, const Vector3& b);
	float MagnitudeV3(const Vector3& a);
	float SquaredMagnitudeV3(const Vector3& a);
	Vector3 NormalizeV3(const Vector3& a);
	float DotProductV3(const Vector3& a, const Vector3& b);
	Vector3 CrossProductV3(const Vector3& a, const Vector3& b);
	Vector3 LerpV3(const Vector3& a, const Vector3& b, float s);

	struct alignas(16) Vector4
	{
		float x, y, z, w;
	};

	Vector4 MultiplyByScalarV4(const Vector4& a, float s);
	Vector4 AddV4(const Vector4& a, const Vector4& b);
	Vector4 SubtractV4(const Vector4& a, const Vector4& b);
	float MagnitudeV4(const Vector4& a);
	float SquaredMagnitudeV4(const Vector4& a);
	Vector4 NormalizeV4(const Vector4& a);
	float DotProductV4(const Vector4& a, const Vector4& b);
	Vector4 CrossProductV4(const Vector4& a, const Vector4& b);
	Vector4 LerpV4(const Vector4& a, const Vector4& b, float s);

	// NOTE: These matrices will be stored in collum major notation
	struct alignas(16) Matrix4x4
	{
		float m11, m12, m13, m14;
		float m21, m22, m23, m24;
		float m31, m32, m33, m34;
		float m41, m42, m43, m44;
	};

	Matrix4x4 Identity4x4();
	Matrix4x4 Transpose4x4(const Matrix4x4& a);
	Matrix4x4 Multiply4x4(const Matrix4x4& a, const Matrix4x4& b);
	Matrix4x4 Translate4x4(float tx, float ty, float tz);
	Matrix4x4 RatateZAxis4x4(float angle);
	Matrix4x4 Scale4x4(float sx, float sy, float sz);
	Matrix4x4 LookTo4x4(Vector3 pos, Vector3 up, Vector3 look);
	Matrix4x4 OrthoProjection4x4(float l, float r, float t, float b, float n, float f);
	Matrix4x4 operator*(Matrix4x4 a, Matrix4x4 b);
	
	struct alignas(16) Matrix3x3
	{
		float m11, m12, m13, pad14;
		float m21, m22, m23, pad24;
		float m31, m32, m33, pad34;
	};

	Matrix3x3 Identity3x3();
	Matrix3x3 Transpose3x3(const Matrix3x3& a);
	Matrix3x3 Multiply3x3(const Matrix3x3& a, const Matrix3x3& b);
	Matrix3x3 Translate3x3(float tx, float ty);
	Matrix3x3 RatateZAxis3x3(float angle);
	Matrix3x3 Scale3x3(float sx, float sy);
	Matrix3x3 OrthoProjection3x3(float l, float r, float b, float t);
	Matrix3x3 operator*(Matrix3x3 a, Matrix3x3 b);
}
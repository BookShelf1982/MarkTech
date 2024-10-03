#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	struct Vector2
	{
		F32 x, y;
	};

	struct Vector3
	{
		F32 x, y, z;
	};

	struct Vector4
	{
		F32 x, y, z, w;
	};

	struct Matrix4x4
	{
		F32 m11, m12, m13, m14;
		F32 m21, m22, m23, m24;
		F32 m31, m32, m33, m34;
		F32 m41, m42, m43, m44;
	};

	struct Matrix4x3
	{
		F32 m11, m12, m13, m14;
		F32 m21, m22, m23, m24;
		F32 m31, m32, m33, m34;
	};

	Vector3 AddV3(Vector3 a, Vector3 b);
	Vector3 SubtractV3(Vector3 a, Vector3 b);
	Vector3 MultiplyV3(Vector3 a, float scalar);
	float MagnitudeV3(Vector3 a);
	Vector3 NormalizeV3(Vector3 a);
	float DotV3(Vector3 a, Vector3 b);
	Vector3 CrossV3(Vector3 a, Vector3 b);

	Vector4 AddV4(Vector4 a, Vector4 b);
	Vector4 SubtractV4(Vector4 a, Vector4 b);
	Vector4 MultiplyV4(Vector4 a, float scalar);
	float MagnitudeV4(Vector4 a);
	Vector4 NormalizeV4(Vector4 a);
}
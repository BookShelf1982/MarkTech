#pragma once
#include "Core.h"
#include <stdint.h>
#include <math.h>
#include <xmmintrin.h>
#include <DirectXMath.h>

namespace MarkTech
{
	struct MARKTECH_API MVector2
	{
		float x, y;

		MVector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

		// Bool Equals Operator Overload
		bool operator==(const MVector2& other) const;

		// Bool Not Equals Operator Overload
		bool operator!=(const MVector2& other) const;

		// Addition Operator Overload
		MVector2 operator+(const MVector2& right) const;

		// Subtraction Operator Overload
		MVector2 operator-(const MVector2& right) const;

		// Float Multiplication Operator Overload
		MVector2 operator*(const float& other) const;

		// Float Multiplication Operator Overload
		MVector2 operator/(const float& f) const;
	};

	struct MARKTECH_API MVector3
	{
		float x, y, z;

		MVector3();

		MVector3(float x, float y, float z);

		// Bool Equals Operator Overload
		bool operator==(const MVector3& other) const;

		// Bool Not Equals Operator Overload
		bool operator!=(const MVector3& other) const;

		// Addition Operator Overload
		MVector3 operator+(const MVector3& right) const;

		// Subtraction Operator Overload
		MVector3 operator-(const MVector3& right) const;

		// Float Multiplication Operator Overload
		MVector3 operator*(const float& other) const;

		// Float Multiplication Operator Overload
		MVector3 operator/(const float& f) const;
	};

	float DotProductVector3(const MVector3& a, const MVector3& b);

	MVector3 CrossProductVector3(const MVector3& a, const MVector3& b);

	MVector3 UnitVector3(const MVector3& a);

	float MagnitudeVector3(const MVector3& a);

	MVector3 NegateVector3(const MVector3& v);

	struct MARKTECH_API MVector4
	{
		float x, y, z, w;

		MVector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {}

		// Bool Equals Operator Overload
		bool operator==(const MVector4& other) const;

		// Bool Not Equals Operator Overload
		bool operator!=(const MVector4& other) const;

		// Addition Operator Overload
		MVector4 operator+(const MVector4& right) const;

		// Subtraction Operator Overload
		MVector4 operator-(const MVector4& right) const;

		// Float Multiplication Operator Overload
		MVector4 operator*(const float& other) const;

		// Float Division Operator Overload
		MVector4 operator/(const float& other) const;
	};

	struct MARKTECH_API MRotator
	{
		float Pitch, Yaw, Roll;

		MRotator(float Pitch = 0.0f, float Yaw = 0.0f, float Roll = 0.0f) : Pitch(Pitch), Yaw(Yaw), Roll(Roll) {}

		// Bool Equals Operator Overload
		bool operator==(const MRotator& other) const;

		// Bool Not Equals Operator Overload
		bool operator!=(const MRotator& other) const;

		// Addition Operator Overload
		MRotator operator+(const MRotator& right) const;

		// Subtraction Operator Overload
		MRotator operator-(const MRotator& right) const;

		// Float Multiplication Operator Overload
		MRotator operator*(const float& other) const;

		// Float Division Operator Overload
		MRotator operator/(const float& other) const;
	};

	struct MARKTECH_API MMatrix4x4
	{
		float nums[16];

		MMatrix4x4();
		MMatrix4x4(float n1, float n2,float n3,float n4,float n5,float n6,float n7,float n8,float n9,float n10,float n11,float n12,float n13,float n14,float n15,float n16);

		MMatrix4x4 operator*(const MMatrix4x4& right);

		MVector4 operator*(const MVector4& right);
	};

	MMatrix4x4 MatrixMultiplyByMatrix4x4(const MMatrix4x4& a, const MMatrix4x4& b);
	MMatrix4x4 LookAt(const MVector3& eyePos, const MVector3& lookDir, const MVector3& up);
	MMatrix4x4 OrthographicProjection(float right, float left, float bottom, float top, float nearClip, float farClip);
	MMatrix4x4 PrespectiveProjection(float right, float left, float bottom, float top, float nearClip, float farClip);
	MMatrix4x4 PerspectiveTransformation(float nearClip, float farClip);
	MMatrix4x4 Translate(const float& xoffset, const float& yoffset, const float& zoffset);
	MMatrix4x4 Transpose(const MMatrix4x4& m);
	MMatrix4x4 IdentityMatrix();
	MMatrix4x4 RotateXAxis(float angle);
	MMatrix4x4 RotateYAxis(float angle);
	MMatrix4x4 RotateZAxis(float angle);
	MVector4 MatrixMultiplyByVector4(const MMatrix4x4& a, const MVector4& b);

	struct MARKTECH_API MTransform
	{
		MVector3 Position, Scale;
		MRotator Rotation;

		MVector3 Forward, Right;

		MTransform(MVector3 Position = MVector3(), MRotator Rotation = MRotator(), MVector3 Scale = MVector3()) : Position(Position), Rotation(Rotation), Scale(Scale), 
			Forward(MVector3()), Right(MVector3()) {}
	};
}
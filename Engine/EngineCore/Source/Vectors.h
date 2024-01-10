#pragma once
#include "Core.h"
#include <DirectXMath.h>

struct  MVector2
{
	float x, y;

	MVector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct  MVector3
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

struct  MVector4
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

struct  MRotator
{
	float Pitch, Yaw, Roll;

	MRotator(float Pitch = 0.0f, float Yaw = 0.0f, float Roll = 0.0f) : Pitch(Pitch), Yaw(Yaw), Roll(Roll) {}

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

	// Float Division Operator Overload
	MVector3 operator/(const float& other) const;
};

struct MTransform
{
	MVector3 Position, Scale;
	MRotator Rotation;

	MTransform(MVector3 Position = MVector3(), MRotator Rotation = MRotator(), MVector3 Scale = MVector3()) : Position(Position), Rotation(Rotation), Scale(Scale) {}
};
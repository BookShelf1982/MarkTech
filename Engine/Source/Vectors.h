#pragma once
#include <string>
#include <sstream>
struct MVector2
{
	float x, y;

	MVector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct MVector3
{
	float x, y, z;

	MVector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

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

	std::string ToString();
};

struct MVector4
{
	float x, y, z, w;

	MVector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {}
};

struct MTransform
{
	MVector3 Position, Rotation, Scale;

	MTransform(MVector3 Position = MVector3(), MVector3 Rotation = MVector3(), MVector3 Scale = MVector3()) : Position(Position), Rotation(Rotation), Scale(Scale) {}
};
#include "Vectors.h"
#include <math.h>
namespace MarkTech
{
	bool MVector3::operator==(const MVector3& other) const
	{
		return (this->x == other.x && this->y == other.y && this->z == other.z);
	}

	bool MVector3::operator!=(const MVector3& other) const
	{
		return !(this->x == other.z && this->y == other.y && this->z == other.z);
	}

	MVector3 MVector3::operator+(const MVector3& right) const
	{
		return MVector3(this->x + right.x, this->y + right.y, this->z + right.z);
	}

	MVector3 MVector3::operator-(const MVector3& right) const
	{
		return MVector3(this->x - right.x, this->y - right.y, this->z - right.z);
	}

	MVector3 MVector3::operator*(const float& other) const
	{
		return MVector3(this->x * other, this->y * other, this->z * other);
	}

	MVector3 MVector3::operator/(const float& other) const
	{
		return MVector3(this->x / other, this->y / other, this->z / other);
	}

	// Vector3 DotProduct
	float DotV3(const MVector3 a, const MVector3 b)
	{
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}

	// Vector3 Cross Product
	MVector3 CrossV3(const MVector3 a, const MVector3 b)
	{
		return MVector3(a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	// Vector3 Magnitude Calculation
	float MagnitudeV3(const MVector3 in)
	{
		return (sqrtf(powf(in.x, 2) + powf(in.y, 2) + powf(in.z, 2)));
	}

	// Angle between 2 Vector3 Objects
	float AngleBetweenV3(const MVector3 a, const MVector3 b)
	{
		float angle = DotV3(a, b);
		angle /= (MagnitudeV3(a) * MagnitudeV3(b));
		return angle = acosf(angle);
	}
}
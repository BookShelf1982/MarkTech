#include "Vectors.h"
#include <math.h>
namespace MarkTech
{
	MVector3::MVector3()
		: x(0.0f), y(0.0f), z(0.0f)
	{
	}

	MVector3::MVector3(float x, float y, float z) 
		: x(x), y(y), z(z) 
	{
	}

	bool MVector3::operator==(const MVector3& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	bool MVector3::operator!=(const MVector3& other) const
	{
		return !(x == other.z && y == other.y && z == other.z);
	}

	MVector3 MVector3::operator+(const MVector3& right) const
	{
		return MVector3(x + right.x, y + right.y, z + right.z);
	}

	MVector3 MVector3::operator-(const MVector3& right) const
	{
		return MVector3(x - right.x, y - right.y, z - right.z);
	}

	MVector3 MVector3::operator*(const float& other) const
	{
		return MVector3(x * other, y * other, z * other);
	}

	MVector3 MVector3::operator/(const float& other) const
	{
		return MVector3(x / other, y / other, z / other);
	}

	float DotProductVector3(const MVector3& a, const MVector3& b)
	{
		return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
	}

	MVector3 CrossProductVector3(const MVector3& a, const MVector3& b)
	{
		return MVector3(a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	MVector3 UnitVector3(const MVector3& a)
	{
		return a / MagnitudeVector3(a);
	}

	float MagnitudeVector3(const MVector3& a)
	{
		return sqrtf(a.x*a.x + a.y*a.y + a.z*a.z);
	}

	MVector3 NegateVector3(const MVector3& v)
	{
		return MVector3((-1*v.x), (-1*v.y), (-1*v.z));
	}

	MMatrix4x4 MatrixMultiplyByMatrix4x4(const MMatrix4x4& a, const MMatrix4x4& b)
	{
		return MMatrix4x4(
			(a.nums[0] * b.nums[0]) + (a.nums[1] * b.nums[4]) + (a.nums[2] * b.nums[8]) + (a.nums[3] * b.nums[12]),//1x1
			(a.nums[0] * b.nums[1]) + (a.nums[1] * b.nums[5]) + (a.nums[2] * b.nums[9]) + (a.nums[3] * b.nums[13]),//1x2
			(a.nums[0] * b.nums[2]) + (a.nums[1] * b.nums[6]) + (a.nums[2] * b.nums[10]) + (a.nums[3] * b.nums[14]),//1x3
			(a.nums[0] * b.nums[3]) + (a.nums[1] * b.nums[7]) + (a.nums[2] * b.nums[11]) + (a.nums[3] * b.nums[15]),//1x4
			(a.nums[4] * b.nums[0]) + (a.nums[5] * b.nums[4]) + (a.nums[6] * b.nums[8]) + (a.nums[7] * b.nums[12]),//2x1
			(a.nums[4] * b.nums[1]) + (a.nums[5] * b.nums[5]) + (a.nums[6] * b.nums[9]) + (a.nums[7] * b.nums[13]),//2x2
			(a.nums[4] * b.nums[2]) + (a.nums[5] * b.nums[6]) + (a.nums[6] * b.nums[10]) + (a.nums[7] * b.nums[14]),//2x3
			(a.nums[4] * b.nums[3]) + (a.nums[5] * b.nums[7]) + (a.nums[6] * b.nums[11]) + (a.nums[7] * b.nums[15]),//2x4
			(a.nums[8] * b.nums[0]) + (a.nums[9] * b.nums[4]) + (a.nums[10] * b.nums[8]) + (a.nums[11] * b.nums[12]),//3x1
			(a.nums[8] * b.nums[1]) + (a.nums[9] * b.nums[5]) + (a.nums[10] * b.nums[9]) + (a.nums[11] * b.nums[13]),//3x2
			(a.nums[8] * b.nums[2]) + (a.nums[9] * b.nums[6]) + (a.nums[10] * b.nums[10]) + (a.nums[11] * b.nums[14]),//3x3
			(a.nums[8] * b.nums[3]) + (a.nums[9] * b.nums[7]) + (a.nums[10] * b.nums[11]) + (a.nums[11] * b.nums[15]),//3x4
			(a.nums[12] * b.nums[0]) + (a.nums[13] * b.nums[4]) + (a.nums[14] * b.nums[8]) + (a.nums[15] * b.nums[12]),//4x1
			(a.nums[12] * b.nums[1]) + (a.nums[13] * b.nums[5]) + (a.nums[14] * b.nums[9]) + (a.nums[15] * b.nums[13]),//4x2
			(a.nums[12] * b.nums[2]) + (a.nums[13] * b.nums[6]) + (a.nums[14] * b.nums[10]) + (a.nums[15] * b.nums[14]),//4x3
			(a.nums[12] * b.nums[3]) + (a.nums[13] * b.nums[7]) + (a.nums[14] * b.nums[11]) + (a.nums[15] * b.nums[15])//4x4
		);
	}

	MMatrix4x4 LookAt(const MVector3& eyePos, const MVector3& lookDir, const MVector3& up)
	{
		/*	XMVECTOR R2 = XMVector3Normalize(EyeDirection);

			XMVECTOR R0 = XMVector3Cross(UpDirection, R2);
			R0 = XMVector3Normalize(R0);

			XMVECTOR R1 = XMVector3Cross(R2, R0);

			XMVECTOR NegEyePosition = XMVectorNegate(EyePosition);

			XMVECTOR D0 = XMVector3Dot(R0, NegEyePosition);
			XMVECTOR D1 = XMVector3Dot(R1, NegEyePosition);
			XMVECTOR D2 = XMVector3Dot(R2, NegEyePosition);

			XMMATRIX M;
			M.r[0] = XMVectorSelect(D0, R0, g_XMSelect1110.v);
			M.r[1] = XMVectorSelect(D1, R1, g_XMSelect1110.v);
			M.r[2] = XMVectorSelect(D2, R2, g_XMSelect1110.v);
			M.r[3] = g_XMIdentityR3.v;

			M = XMMatrixTranspose(M);

			return M;
		*/


		MVector3 zaxis = UnitVector3(eyePos - lookDir);
		MVector3 xaxis = CrossProductVector3(up, zaxis);
		xaxis = UnitVector3(xaxis);
		MVector3 yaxis = CrossProductVector3(zaxis, xaxis);
		MVector3 NegEyePos = NegateVector3(eyePos);
		MMatrix4x4 M;
		M.nums[0] = xaxis.x;
		M.nums[1] = yaxis.x;
		M.nums[2] = zaxis.x;
		M.nums[3] = DotProductVector3(xaxis, NegEyePos);
		M.nums[4] = xaxis.y;
		M.nums[5] = yaxis.y;
		M.nums[6] = zaxis.y;
		M.nums[7] = DotProductVector3(yaxis, NegEyePos);
		M.nums[8] = xaxis.z;
		M.nums[9] = yaxis.z;
		M.nums[10] = zaxis.z;
		M.nums[11] = DotProductVector3(zaxis, NegEyePos);
		M.nums[15] = 1.0f;
		MMatrix4x4 result = Transpose(M);
		return result;
	}

	MMatrix4x4 OrthographicProjection(float right, float left, float bottom, float top, float nearClip, float farClip)
	{
		MMatrix4x4 M;
		M.nums[0] = 2 / right - left;
		M.nums[5] = 2 / top - bottom;
		M.nums[10] = 2 / farClip - nearClip;
		M.nums[15] = 1.0f;
		M.nums[3] = -1 * ((right + left) / (right - left));
		M.nums[7] = -1 * ((top + bottom) / (top - bottom));
		M.nums[11] = -1 * ((farClip + nearClip) / (farClip - nearClip));

		return M;
	}

	MMatrix4x4 PrespectiveProjection(float right, float left, float bottom, float top, float nearClip, float farClip)
	{
		MMatrix4x4 Perspective = PerspectiveTransformation(nearClip, farClip);
		MMatrix4x4 Orthograph = OrthographicProjection(right, left, bottom, top, nearClip, farClip);

		return Perspective * Orthograph;
	}

	MMatrix4x4 PerspectiveTransformation(float nearClip, float farClip)
	{
		MMatrix4x4 M;
		M.nums[0] = nearClip;
		M.nums[5] = nearClip;
		M.nums[10] = nearClip - farClip;
		M.nums[11] = -farClip * nearClip;
		M.nums[14] = 1.0f;

		return M;
	}

	MMatrix4x4 Translate(const float& xoffset, const float& yoffset, const float& zoffset)
	{
		MMatrix4x4 M;
		M.nums[0] = 1.0f;
		M.nums[5] = 1.0f;
		M.nums[10] = 1.0f;
		M.nums[15] = 1.0f;
		M.nums[3] = xoffset;
		M.nums[7] = yoffset;
		M.nums[11] = zoffset;

		return M;
	}

	MMatrix4x4 Transpose(const MMatrix4x4& m)
	{
		MMatrix4x4 M = m;
		M.nums[1] = m.nums[4];
		M.nums[2] = m.nums[8];
		M.nums[3] = m.nums[12];
		M.nums[4] = m.nums[1];
		M.nums[6] = m.nums[9];
		M.nums[7] = m.nums[13];
		M.nums[8] = m.nums[2];
		M.nums[9] = m.nums[6];
		M.nums[11] = m.nums[14];
		M.nums[12] = m.nums[3];
		M.nums[13] = m.nums[7];
		M.nums[14] = m.nums[11];

		return M;
	}

	MMatrix4x4 IdentityMatrix()
	{
		return MMatrix4x4(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	MMatrix4x4 RotateXAxis(float angle)
	{
		float sin, cos;
		sin = sinf(angle);
		cos = cosf(angle);

		MMatrix4x4 M;
		M.nums[0] = 1.0f;
		M.nums[5] = cos;
		M.nums[6] = sin;
		M.nums[9] = -sin;
		M.nums[10] = cos;
		M.nums[15] = 1.0f;

		return M;
	}

	MMatrix4x4 RotateYAxis(float angle)
	{
		float sin, cos;
		sin = sinf(angle);
		cos = cosf(angle);

		MMatrix4x4 M;
		M.nums[0] = cos;
		M.nums[2] = -sin;
		M.nums[5] = 1.0f;
		M.nums[8] = sin;
		M.nums[10] = cos;
		M.nums[15] = 1.0f;

		return M;
	}

	MMatrix4x4 RotateZAxis(float angle)
	{
		float sin, cos;
		sin = sinf(angle);
		cos = cosf(angle);

		MMatrix4x4 M;
		M.nums[0] = cos;
		M.nums[1] = sin;
		M.nums[4] = -sin;
		M.nums[5] = cos;
		M.nums[10] = 1.0f;
		M.nums[15] = 1.0f;

		return M;
	}

	MVector4 MatrixMultiplyByVector4(const MMatrix4x4& a, const MVector4& b)
	{
		return MVector4((a.nums[0] * b.x) + (a.nums[1] * b.y) + (a.nums[2] * b.z) + (a.nums[3] * b.w),
			(a.nums[4] * b.x) + (a.nums[5] * b.y) + (a.nums[6] * b.z) + (a.nums[7] * b.w),
			(a.nums[8] * b.x) + (a.nums[9] * b.y) + (a.nums[10] * b.z) + (a.nums[11] * b.w),
			(a.nums[12] * b.x) + (a.nums[13] * b.y) + (a.nums[14] * b.z) + (a.nums[15] * b.w));
	}

	MMatrix4x4::MMatrix4x4()
		:nums{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f}
	{
	}

	MMatrix4x4::MMatrix4x4(float n1, float n2, float n3, float n4, float n5, float n6, float n7, float n8, float n9, float n10, float n11, float n12, float n13, float n14, float n15, float n16)
		:nums{ n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16 }
	{
	}

	MMatrix4x4 MMatrix4x4::operator*(const MMatrix4x4& right)
	{
		return MatrixMultiplyByMatrix4x4(*this, right);
	}

	MVector4 MMatrix4x4::operator*(const MVector4& right)
	{
		return MatrixMultiplyByVector4(*this, right);
	}
}
//====================================================================================================
//	File Name:		Math.h
//	Created By:		Matt Stinnen
//====================================================================================================
#include "precompiled.h"
#include "Math.h"

namespace MAGE3D
{
namespace Math
{

//====================================================================================================
// consts
//====================================================================================================

static const float kEpsilon		= 0.0000000001f;
static const float kPi			= 3.1415926535;
static const float kRootTwo		= 1.4142135623;
static const float kDegToRad	= kPi / 180.0f;
static const float kRadToDeg	= 180.0f / kPi;	


bool Math::IsZero(float f)
{
	if(f > kEpsilon || f < -kEpsilon)
	{
		return false;
	}

	return true;
}

float Math::Min(float a, float b)
{
	if(a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
float Math::Max(float a, float b)
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
float Math::Clamp(float f, float min, float max)
{
	if(f < min)
	{
		f = min;
	}
	else if(f > max)
	{
		f = max;
	}

	return f;
}
float Math::Floor(float f)
{
	return floor(f);
}
float Math::Length(const Vector3& v)
{
	return v.Magnitude();
}
float Math::LengthSq(const Vector3& v)
{
	return v.MagnitudeSqrd();
}

Vector3 Math::Normalize(const Vector3& v)
{
	Vector3 temp;
	temp = v;
	temp.Normalize();
	return temp;
}

float Math::Dot(const Vector3& a, const Vector3& b)
{
	return a.DotProduct(b);
}
Vector3 Math::Cross(const Vector3& a, const Vector3& b) 
{

	return a.CrossProduct(b);
}
float Math::Distance(const Vector3& a, const Vector3& b)
{
	return sqrt(pow((a.x + b.x),2) + pow((a.y + b.y),2) + pow((a.z + b.z),2));
}
float Math::DistanceSq(const Vector3& a, const Vector3& b)
{
	return (pow((a.x + b.x),2) + pow((a.y + b.y),2) + pow((a.z + b.z),2));
}

Vector3 Math::TransformNormal(const Matrix4& m, const Vector3& v)
{
	return Vector3
	(
		(v.x * m._11) + (v.y * m._21) + (v.z * m._31),
		(v.x * m._12) + (v.y * m._22) + (v.z * m._32),
		(v.x * m._13) + (v.y * m._23) + (v.z * m._33)
	);
}
Vector3 Math::TransformCoord(const Matrix4& m, const Vector3& v)
{
	// transform (xyz1) and project to w=1
	float x = (v.x * m._11) + (v.y * m._21) + (v.z * m._31) + m._41;
	float y = (v.x * m._12) + (v.y * m._22) + (v.z * m._32) + m._42;
	float z = (v.x * m._13) + (v.y * m._23) + (v.z * m._33) + m._43;
	float w = (v.x * m._14) + (v.y * m._24) + (v.z * m._34) + m._44;
	return Vector3(x/w, y/w, z/w);
}

}// namespace math
}// namespace mage3d


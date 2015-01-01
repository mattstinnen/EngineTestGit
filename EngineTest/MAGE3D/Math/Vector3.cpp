//====================================================================================================
//	File Name:		Vector3.cpp
//	Created By:		Matt Stinnen
//	Description:	A cpp for a 3 component Vector class
//====================================================================================================

//====================================================================================================
// includes
//====================================================================================================
#include "Precompiled.h"
#include "vector3.h"
#include <limits>

using namespace MAGE3D;
using namespace Math;

static float sEpsilon = std::numeric_limits<float>::epsilon();

//====================================================================================================
// definitions
//====================================================================================================


const Vector3 Vector3::XAxis(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::YAxis(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::ZAxis(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::XZero(0.0f, 0.0f, 0.0f);


Vector3::Vector3()
:	x(0.0f)
,	y(0.0f)
,	z(0.0f)
{
	// empty
}

//----------------------------------------------------------------------------------------------------

Vector3::Vector3(const Vector3& v)
:	x(v.x)
,	y(v.y)
,	z(v.z)
{
	// empty
}

//----------------------------------------------------------------------------------------------------

Vector3::Vector3(float X, float Y, float Z)
:	x(X)
,	y(Y)
,	z(Z)
{
	// empty
}

//----------------------------------------------------------------------------------------------------

Vector3::~Vector3()
{
	// empty
}

//----------------------------------------------------------------------------------------------------

Vector3 Vector3::operator+(const Vector3& v)
{
	Vector3 newVec;

	newVec.x = x + v.x;
	newVec.y = y + v.y;
	newVec.z = z + v.z;

	return newVec;
}

//----------------------------------------------------------------------------------------------------

Vector3 Vector3::operator-(const Vector3& v)
{
	Vector3 newVec;

	newVec.x = x - v.x;
	newVec.y = y - v.y;
	newVec.z = z - v.z;

	return newVec;
}

//----------------------------------------------------------------------------------------------------

Vector3 Vector3::operator *(float f)
{
	Vector3 newVec;

	newVec.x = x * f;
	newVec.y = y * f;
	newVec.z = z * f;

	return newVec;
}

//----------------------------------------------------------------------------------------------------

Vector3 Vector3::operator /(float f)
{	
	Vector3 newVec;
	
	float n = 1/f;

	if(f >= sEpsilon)
	{	
		newVec.x = x * n;
		newVec.y = y * n;
		newVec.z = z * n;
	}
	return newVec;
}

//----------------------------------------------------------------------------------------------------

float Vector3::Magnitude() const
{
	return sqrt((x * x) + (y * y) + (z * z)); 
}

//----------------------------------------------------------------------------------------------------

float Vector3::MagnitudeSqrd() const
{
	return ((x * x) + (y * y) + (z * z)); 
}

//----------------------------------------------------------------------------------------------------

void Vector3::Normalize()
{
	float m = Magnitude();
	
	if(m >= sEpsilon)
	{
		float invMag = 1/m;
		
		x *= invMag;
		y *= invMag;
		z *= invMag;
	}
}

//----------------------------------------------------------------------------------------------------

float Vector3::DotProduct(const Vector3& v) const
{
	return ((x * v.x) + (y * v.y) + (z * v.z));
}

//----------------------------------------------------------------------------------------------------

Vector3 Vector3::CrossProduct(const Vector3& v) const
{
	Vector3 CpV;

	CpV.x = ((y* v.z) - (v.y * z));
	CpV.y = ((z* v.x) - (v.z * x));
	CpV.z = ((x* v.y) - (v.x * y));
	
	return CpV;
}

//----------------------------------------------------------------------------------------------------

Vector3 Vector3::operator-()
{
	return this->operator *(-1);
}

Vector3 Vector3::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}
Vector3& Vector3::operator+=(const Vector3& v)
{
	Vector3 newVec;

	newVec.x = x + v.x;
	newVec.y = y + v.y;
	newVec.z = z + v.z;

	*this = newVec;
	return *this;
}
Vector3 Vector3::operator-=(const Vector3& v)
{
	Vector3 temp;
	temp.x = this->x - v.x;
	temp.y = this->y - v.y;
	temp.z = this->z - v.z;

	*this = temp;

	return *this;
}
Vector3 Vector3::operator*=(float f)
{
	Vector3 temp;
	temp.x = this->x * f;
	temp.y = this->y * f;
	temp.z = this->z * f;

	*this = temp;

	return *this;
}
Vector3 Vector3::operator/=(float f)
{
	float inF = 1/f;
	Vector3 temp;
	temp.x = this->x * inF;
	temp.y = this->y * inF;
	temp.z = this->z * inF;

	*this = temp;

	return *this;
}

bool Vector3::operator==(const Vector3& v)
{
	if((x == v.x) && (y == v.y) && (z == v.z))
	{
		return true;
	}
	return false;
}
bool Vector3::operator!=(const Vector3& v)
{
	return !(this->operator ==(v));
}
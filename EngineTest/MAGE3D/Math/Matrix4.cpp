#include "precompiled.h"
#include "Math.h"
#include "matrix4.h"
#include "Vector3.h"

//====================================================================================================
//	File Name:		Matrix4.cpp
//	Created By:		Matt Stinnen
//====================================================================================================
namespace MAGE3D
{
namespace Math
{

//----------------------------------------------------------------------------------------------------

const Matrix4 Matrix4::Identity
(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

Matrix4::Matrix4()
{
	this->_11 = 1.0f;
	this->_12 = 0.0f;
	this->_13 = 0.0f;
	this->_14 = 0.0f;

	this->_21 = 0.0f;
	this->_22 = 1.0f;
	this->_23 = 0.0f;
	this->_24 = 0.0f;

	this->_31 = 0.0f;
	this->_32 = 0.0f;
	this->_33 = 1.0f;
	this->_34 = 0.0f;
	
	this->_41 = 0.0f;
	this->_42 = 0.0f;
	this->_43 = 0.0f;
	this->_44 = 1.0f;
}

//----------------------------------------------------------------------------------------------------

Matrix4::Matrix4(float _11, float _12, float _13, float  _14,
		float _21, float _22, float _23, float  _24,
		float _31, float _32, float _33, float  _34,
		float _41, float _42, float _43, float  _44)
{
	this->_11 = _11;
	this->_12 = _12;
	this->_13 = _13;
	this->_14 = _14;

	this->_21 = _21;
	this->_22 = _22;
	this->_23 = _23;
	this->_24 = _24;

	this->_31 = _31;
	this->_32 = _32;
	this->_33 = _33;
	this->_34 = _34;
	
	this->_41 = _41;
	this->_42 = _42;
	this->_43 = _43;
	this->_44 = _44;
}

//----------------------------------------------------------------------------------------------------

Matrix4::Matrix4(const Matrix4& m)
{
	this->_11 = m._11;
	this->_12 = m._12;
	this->_13 = m._13;
	this->_14 = m._14;

	this->_21 = m._21;
	this->_22 = m._22;
	this->_23 = m._23;
	this->_24 = m._24;

	this->_31 = m._31;
	this->_32 = m._32;
	this->_33 = m._33;
	this->_34 = m._34;
	
	this->_41 = m._41;
	this->_42 = m._42;
	this->_43 = m._43;
	this->_44 = m._44;
}

//----------------------------------------------------------------------------------------------------

Matrix4::Matrix4(const float matrixArray[4][4])
{
	this->_11 = matrixArray[0][0];
	this->_12 = matrixArray[0][1];
	this->_13 = matrixArray[0][2];
	this->_14 = matrixArray[0][3];

	this->_21 = matrixArray[1][0];
	this->_22 = matrixArray[1][1];
	this->_23 = matrixArray[1][2];
	this->_24 = matrixArray[1][3];

	this->_31 = matrixArray[2][0];
	this->_32 = matrixArray[2][1];
	this->_33 = matrixArray[2][2];
	this->_34 = matrixArray[2][3];
	
	this->_41 = matrixArray[3][0];
	this->_42 = matrixArray[3][1];
	this->_43 = matrixArray[3][2];
	this->_44 = matrixArray[3][3];
}

//----------------------------------------------------------------------------------------------------

Matrix4::~Matrix4()
{

}

//----------------------------------------------------------------------------------------------------


void Matrix4::SetIdentity()
{
	this->operator =(Identity);
}

//----------------------------------------------------------------------------------------------------

void Matrix4::SetTranslate(float x, float y, float z)
{
	this->_41 += x;
	this->_42 += y;
	this->_43 += z;
}

//----------------------------------------------------------------------------------------------------

void Matrix4::SetRotateX(float angle)
{
	Matrix4 xTemp;
	xTemp.SetIdentity();
	xTemp._22 = cos(angle);
	xTemp._33 = cos(angle);
	xTemp._23 = sin(angle);
	xTemp._32 = -sin(angle);
	*this = xTemp;
	
}

//----------------------------------------------------------------------------------------------------

void Matrix4::SetRotateY(float angle)
{
	Matrix4 yTemp;
	yTemp.SetIdentity();
	yTemp._11 = cos(angle);
	yTemp._33 = cos(angle);
	yTemp._13 = -sin(angle);
	yTemp._31 = sin(angle);
	*this = yTemp;
}

//----------------------------------------------------------------------------------------------------

void Matrix4::SetRotateZ(float angle)
{
	Matrix4 zTemp;
	zTemp.SetIdentity();
	zTemp._11 = cos(angle);
	zTemp._22 = cos(angle);
	zTemp._12 = sin(angle);
	zTemp._21 = -sin(angle);
	*this = zTemp;
}

//----------------------------------------------------------------------------------------------------

void Matrix4::SetRotateAxis(const Vector3& v, float angle)
{
	const Vector3 axis = Math::Normalize(v);
	const float c = cos(angle);
	const float s = sin(angle);
	const float x = axis.x;
	const float y = axis.y;
	const float z = axis.z;

	_11 = x * x * (1 - c) + c;
	_21 = x * y * (1 - c) - (z * s);
	_31 = x * z * (1 - c) + (y * s);
	_41 = 0.0f;
	  
	_12 = y * x * (1 - c) + (z * s);
	_22 = y * y * (1 - c) + c;
	_32 = y * z * (1 - c) - (x * s);
	_42 = 0.0f;
	  
	_13 = z * x * (1 - c) - (y * s);
	_23 = z * y * (1 - c) + (x * s);
	_33 = z * z * (1 - c) + c;
	_43 = 0.0f;
	  
	_14 = 0.0f;
	_24 = 0.0f;
	_34 = 0.0f;
	_44 = 1.0f;
}

//----------------------------------------------------------------------------------------------------

void Matrix4::SetScale(float x, float y, float z)
{
	this->_11 *= x;
	this->_22 *= y;
	this->_33 *= z;
}

//----------------------------------------------------------------------------------------------------
//
//
Matrix4 Matrix4::operator*(float scalar) const
{
	Matrix4 temp;
	temp._11 = this->_11 * scalar;
	temp._12 = this->_12 * scalar;
	temp._13 = this->_13 * scalar;
	temp._14 = this->_14 * scalar;

	temp._21 = this->_21 * scalar;
	temp._22 = this->_22 * scalar;
	temp._23 = this->_23 * scalar;
	temp._24 = this->_24 * scalar;

	temp._31 = this->_31 * scalar;
	temp._32 = this->_32 * scalar;
	temp._33 = this->_33 * scalar;
	temp._34 = this->_34 * scalar;

	temp._41 = this->_41 * scalar;
	temp._42 = this->_42 * scalar;
	temp._43 = this->_43 * scalar;
	temp._44 = this->_44 * scalar;

	return temp;
}
//----------------------------------------------------------------------------------------------------
//
//
Matrix4& Matrix4::operator*=(float scalar) 
{
	Matrix4 temp;
	temp._11 = this->_11 * scalar;
	temp._12 = this->_12 * scalar;
	temp._13 = this->_13 * scalar;
	temp._14 = this->_14 * scalar;

	temp._21 = this->_21 * scalar;
	temp._22 = this->_22 * scalar;
	temp._23 = this->_23 * scalar;
	temp._24 = this->_24 * scalar;

	temp._31 = this->_31 * scalar;
	temp._32 = this->_32 * scalar;
	temp._33 = this->_33 * scalar;
	temp._34 = this->_34 * scalar;

	temp._41 = this->_41 * scalar;
	temp._42 = this->_42 * scalar;
	temp._43 = this->_43 * scalar;
	temp._44 = this->_44 * scalar;

	*this = temp;
	return *this;
}
//----------------------------------------------------------------------------------------------------


const Matrix4& Matrix4::operator= (const Matrix4& m)
{
	this->_11 = m._11;
	this->_12 = m._12;
	this->_13 = m._13;
	this->_14 = m._14;

	this->_21 = m._21;
	this->_22 = m._22;
	this->_23 = m._23;
	this->_24 = m._24;

	this->_31 = m._31;
	this->_32 = m._32;
	this->_33 = m._33;
	this->_34 = m._34;
	
	this->_41 = m._41;
	this->_42 = m._42;
	this->_43 = m._43;
	this->_44 = m._44;
	return *this;
}

//----------------------------------------------------------------------------------------------------
Matrix4 Matrix4::operator*(const Matrix4& m) const
{
	Matrix4 multMat;
	multMat._11 = (_11 * m._11) + (_12 * m._21) + (_13 * m._31) + (_14 * m._41);
	multMat._12 = (_11 * m._12) + (_12 * m._22) + (_13 * m._32) + (_14 * m._42);
	multMat._13 = (_11 * m._13) + (_12 * m._23) + (_13 * m._33) + (_14 * m._43);
	multMat._14 = (_11 * m._14) + (_12 * m._24) + (_13 * m._34) + (_14 * m._44);

	multMat._21 = (_21 * m._11) + (_22 * m._21) + (_23 * m._31) + (_24 * m._41);
	multMat._22 = (_21 * m._12) + (_22 * m._22) + (_23 * m._32) + (_24 * m._42);
	multMat._23 = (_21 * m._13) + (_22 * m._23) + (_23 * m._33) + (_24 * m._43);
	multMat._24 = (_21 * m._14) + (_22 * m._24) + (_23 * m._34) + (_24 * m._44);

	multMat._31 = (_31 * m._11) + (_32 * m._21) + (_33 * m._31) + (_34 * m._41);
	multMat._32 = (_31 * m._12) + (_32 * m._22) + (_33 * m._32) + (_34 * m._42);
	multMat._33 = (_31 * m._13) + (_32 * m._23) + (_33 * m._33) + (_34 * m._43);
	multMat._34 = (_31 * m._14) + (_32 * m._24) + (_33 * m._34) + (_34 * m._44);

	multMat._41 = (_41 * m._11) + (_42 * m._21) + (_43 * m._31) + (_44 * m._41);
	multMat._42 = (_41 * m._12) + (_42 * m._22) + (_43 * m._32) + (_44 * m._42);
	multMat._43 = (_41 * m._13) + (_42 * m._23) + (_43 * m._33) + (_44 * m._43);
	multMat._44 = (_41 * m._14) + (_42 * m._24) + (_43 * m._34) + (_44 * m._44);

	return multMat;
}
//----------------------------------------------------------------------------------------------------
Matrix4& Matrix4::operator*=(const Matrix4& m)
{
	*this = this->operator *(m);

	return *this;
}





}	//	namespace Math
}	//	namespace MAGE3D

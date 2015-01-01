#ifndef MATH_H
#define MATH_H

//====================================================================================================
//	File Name:		Math.h
//	Created By:		Matt Stinnen
//	Description:	A header to emcompas all of the math files (a smaller version of the common.h for math)
//====================================================================================================
#include "Matrix4.h"
#include "Vector3.h"

namespace MAGE3D
{
namespace Math
{

//====================================================================================================
// consts
//====================================================================================================

extern const float kEpsilon;
extern const float kPi;
extern const float kRootTwo;
extern const float kDegToRad;
extern const float kRadToDeg;

//====================================================================================================
// helper functions
//====================================================================================================

bool IsZero(float f);

float Min(float a, float b);
float Max(float a, float b);
float Clamp(float f, float min, float max);
float Floor(float f);


float Length(const Vector3& v);
float LengthSq(const Vector3& v);

Vector3 Normalize(const Vector3& v);

float Dot(const Vector3& a, const Vector3& b);
Vector3 Cross(const Vector3& a, const Vector3& b);

float Distance(const Vector3& a, const Vector3& b);
float DistanceSq(const Vector3& a, const Vector3& b);

Vector3 TransformNormal(const Matrix4& m, const Vector3& v);
Vector3 TransformCoord(const Matrix4& m, const Vector3& v);

}// namespace math
}// namespace mage3d

#endif // MATH_H
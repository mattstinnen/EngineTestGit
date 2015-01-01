#ifndef VECTOR3_H
#define VECTOR3_H

//====================================================================================================
//	File Name:		Vector3.h
//	Created By:		Matt Stinnen
//	Description:	A Header for a 3 component Vector class
//====================================================================================================
namespace MAGE3D
{
namespace Math
{

//----------------------------------------------------------------------------------------------------

struct Vector3
{
	// basic vectors

	static const Vector3 XAxis;
	static const Vector3 YAxis;
	static const Vector3 ZAxis;
	static const Vector3 XZero;


	Vector3();
	Vector3(const Vector3& v);
	Vector3(float X, float Y, float Z);
	~Vector3();

	bool IsZero();

	void Normalize();

	float Magnitude() const;
	float MagnitudeSqrd() const;
	float DotProduct(const Vector3& v) const;

	void Set(float _x, float _y, float _z) {x = _x; y = _y; z = _z;}

	Vector3 CrossProduct(const Vector3& v) const;

	Vector3 operator-();
	Vector3 operator+(const Vector3& v);
	Vector3 operator-(const Vector3& v);
	Vector3 operator*(float f);
	Vector3 operator/(float f);

	Vector3 operator=(const Vector3& v);// write
	Vector3& operator+=(const Vector3& v);// write
	Vector3 operator-=(const Vector3& v);// write
	Vector3 operator*=(float f);// write
	Vector3 operator/=(float f);// write

	bool operator==(const Vector3& v);// write
	bool operator!=(const Vector3& v);// write

	

	union
	{
		struct
		{
			float x, y, z;
		};
		float mValues[3];
	};
};

//----------------------------------------------------------------------------------------------------

}	//	namespace Math
}	//	namespace MAGE3D
#endif //#ifndef VECTOR3_H
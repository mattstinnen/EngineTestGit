#ifndef MATRIX4_H
#define MATRIX4_H

//====================================================================================================
//	File Name:		Matrix4.h
//	Created By:		Matt Stinnen
//	Description:	A Header for a 4x4 matrix class
//====================================================================================================
namespace MAGE3D
{
namespace Math
{

//----------------------------------------------------------------------------------------------------
struct Vector3;
struct Matrix4
{
	// basic matrix
	static const Matrix4 Identity;
	Matrix4();
	Matrix4(float _11, float _12, float _13, float  _14,
			float _21, float _22, float _23, float  _24,
			float _31, float _32, float _33, float  _34,
			float _41, float _42, float _43, float  _44);
	Matrix4(const Matrix4& m);
	Matrix4(const float matrixArray[4][4]);
	~Matrix4();

	union
	{
		float mElements[4][4];
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
	};

	void SetIdentity();
	void SetTranslate(float x, float y, float z);
	void SetRotateX(float angle);
	void SetRotateY(float angle);
	void SetRotateZ(float angle);
	void SetRotateAxis(const Vector3& v, float angle);
	void SetScale(float x, float y, float z);

	Matrix4 operator*(float scalar) const;
	Matrix4& operator*=(float scalar);
	Matrix4 operator*(const Matrix4& m) const;
	Matrix4& operator*=(const Matrix4& m);

	const Matrix4& operator= (const Matrix4& m);

};

//----------------------------------------------------------------------------------------------------

}	//	namespace Math
}	//	namespace MAGE3D
#endif //#ifndef Matrix4_H
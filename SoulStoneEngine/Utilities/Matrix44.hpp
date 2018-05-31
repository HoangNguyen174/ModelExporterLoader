#ifndef MATRIX4x4_H
#define MATRIX4x4_H
#include "Vector3.h"
#include "MathUtilities.hpp"

class Matrix44
{
	public:
		float m_matrix[16];

	public:
		Matrix44();
		Matrix44(const float other[16]);
		Matrix44 operator+(const Matrix44& other);
		Matrix44 operator+=(const Matrix44& other);
		Matrix44 operator-(const Matrix44& other);
		Matrix44 operator-=(const Matrix44& other);
		Matrix44 operator*(const Matrix44& other);
		void operator=(const Matrix44& other);
		bool operator==(const Matrix44& other);
		Vector3 TransformPoint(const Vector3& vector);
		Vector3 TransformDirection( const Vector3& vector );
		Matrix44 TransformMatrix(const Matrix44& matrix );
		static Matrix44 CancelTranslation( const Matrix44& matrix );
		static Matrix44 CreateOrthoMatrix(float left, float right, float bottom, float top, float near, float far);
		static Matrix44 CreatePerspectiveMatrix(float fovY,float aspect,float zNear,float zFar);
		static Matrix44 CreateTranslationMatrix(const Vector3& translation);
		static Matrix44 CreateRotationAboutAxisDegrees(const Vector3& rotationAxis, float rotationDegree);
		static Matrix44 CreateUniformScaleMatrix(float scale);
		static Matrix44 CreateIdentity();
};

#endif
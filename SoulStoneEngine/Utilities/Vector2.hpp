#ifndef VECTOR2_H
#define VECTOR2_H
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "MathUtilities.hpp"
#include <string>
#include <iostream>

class Vector2
{
	public:
		float x;
		float y;

	public:
		Vector2()
		{
			x = 0.f;
			y = 0.f;
		}

		Vector2(float x,float y)
		{
			this->x = x;
			this->y = y;
		}

		Vector2(const Vector2& source)
		{
			x = source.x;
			y = source.y;
		}

		~Vector2() {}

		Vector2 Normalize()
		{
			Vector2 temp;
			float length = CalcLength();
			temp.x = x / length;
			temp.y = y / length;
			return temp;
		}

		void SetYawRad(float rad)
		{
			x = cos(rad);
			y = sin(rad);
		}

		void SetYawDeg(float deg)
		{
			x = cos(MathUtilities::DegToRad(deg));
			y = sin(MathUtilities::DegToRad(deg)); 
		}

		void SetLengthYawRad(float length,float rad)
		{
			x = length * cos(rad);
			y = length * sin(rad);
		}

		void SetLengthYawDeg(float length,float deg)
		{
			x = length * cos(MathUtilities::DegToRad(deg));
			y = length * sin(MathUtilities::DegToRad(deg));
		}

		void SetLength(float length)
		{
			float theta = atan2(y,x);
			x = length * cos(theta);
			y = length * sin(theta);
		}

		void Scale(float scale)
		{
			x *= scale;
			y *= scale;
		}

		void ScaleNonUniform(float xScale,float yScale)
		{
			x *= xScale;
			y *= yScale;
		}

		static Vector2 MultiplyComponents(const Vector2& vec1, const Vector2& vec2)
		{
			Vector2 result;
			result.x = vec1.x * vec2.x;
			result.y = vec1.y * vec2.y;
			return result;
		}

		void RotateDegree(float deg)
		{
			float tempX;
			tempX = x;
			x = x * cos(MathUtilities::DegToRad(deg)) - y * sin(MathUtilities::DegToRad(deg));
			y = tempX * sin(MathUtilities::DegToRad(deg)) + y * cos(MathUtilities::DegToRad(deg));
		}

		void RotateRad(float rad)
		{
			float temp_x;
			temp_x = x;
			x = x * cos(rad) - y * sin(rad);
			y = temp_x * sin(rad) + y * cos(rad);
		}

		void Rotate90Deg()
		{
			float temp;
			temp = x;
			x = -y;
			y = temp;
		}

		void RotateMinus90Deg()
		{
			float temp;
			temp = this->x;
			x = y;
			y = -temp;
		}

		void Reverse()
		{
			x = -x;
			y = -y;
		}

		float CalcLength()
		{
			return ( sqrt( x * x + y * y ) );
		}

		float CalcLengthSquare()
		{
			return ( x * x + y * y );
		}

		float CalcYaw()
		{
			return atan2( y, x);
		}

		float CalcYawDeg()
		{
			return MathUtilities::RadToDeg( atan2( y, x ) );
		}

		float DotProduct(const Vector2& other)
		{
			return ( x * other.x + y * other.y );
		}

		static float DotProduct(const Vector2& vec1,const Vector2& vec2)
		{
			return ( vec1.x * vec2.x + vec1.y * vec2.y );
		}

		Vector2 operator=(const Vector2& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}

		Vector2 operator+=(const Vector2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		Vector2 operator-=(const Vector2& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		Vector2 operator*=(float scale)
		{
			x *= scale;
			y *= scale;
			return *this;
		}

		void operator/=(float scale)
		{
			if(scale == 0)
			{
				cout << "Divide by zero.\n";
				exit(0);
			}
			x /= scale;
			y /= scale;
		}

		Vector2 operator+(const Vector2& other)
		{
			Vector2 result;
			result.x = x + other.x;
			result.y = y + other.y;
			return result;
		}

		Vector2 operator-(const Vector2& other)
		{
			Vector2 result;
			result.x = x - other.x;
			result.y = y - other.y;
			return result;
		}

		Vector2 operator-()
		{
			Vector2 result;
			result.x = -x;
			result.y = -y;
			return result;
		}

		friend Vector2 operator*(const Vector2& other,float scale)
		{
			Vector2 result;
			result.x = other.x * scale;
			result.y = other.y * scale;
			return result;
		}

		friend Vector2 operator/(const Vector2& other,float scale)
		{
			Vector2 result;
			if(scale == 0)
			{
				cout << "Divide by zero.\n";
				exit(0);
			}
			result.x = other.x / scale;
			result.y = other.y / scale;
			return result;
		}

		friend bool operator==(const Vector2& vec1,const Vector2& vec2)
		{
			return ( ( vec1.x == vec2.x ) && ( vec1.y == vec2.y ) );
		}

		friend bool operator!=(const Vector2& vec1,const Vector2& vec2)
		{
			return ( ( vec1.x != vec2.x ) || ( vec1.y != vec2.y ) );
		}

		friend Vector2 operator-(const Vector2& vec1, const Vector2& vec2)
		{
			Vector2 temp;
			temp.x = vec1.x - vec2.x;
			temp.y = vec1.y - vec2.y;
			return temp;
		}

		friend Vector2 operator*(float scale,const Vector2& vec)
		{
			Vector2 result;
			result.x = scale * vec.x;
			result.y = scale * vec.y;
			return result;
		}

		void PrintVector()
		{
			cout << "X: " << x << "  Y: " << y << endl;
		}
};
#endif
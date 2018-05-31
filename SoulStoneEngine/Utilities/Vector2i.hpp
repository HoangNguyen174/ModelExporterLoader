#ifndef	VECTOR2INT_H
#define VECTOR2INT_H
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "MathUtilities.hpp"
#include <string>
#include <iostream>

class Vector2i
{
	public:
		int x;
		int y;

	public:
		Vector2i()
		{
			x = 0;
			y = 0;
		}

		Vector2i(int x,int y)
		{
			this->x = x;
			this->y = y;
		}

		Vector2i(const Vector2i& source)
		{
			x = source.x;
			y = source.y;
		}

		~Vector2i() {}

		float CalcLength()
		{
			return ( sqrt( static_cast<float>( x * x + y * y ) ) );
		}

		int CalcLengthSquare()
		{
			return ( x * x + y * y );
		}

		void Scale(int scale)
		{
			x *= scale;
			y *= scale;
		}

		void ScaleNonUniform(int xScale,int yScale)
		{
			x *= xScale;
			y *= yScale;
		}

		void Rotate90Deg()
		{
			int temp;
			temp = x;
			x = -y;
			y = temp;
		}

		void RotateMinus90Deg()
		{
			int temp;
			temp = this->x;
			x = y;
			y = -temp;
		}

		void Reverse()
		{
			x = -x;
			y = -y;
		}

		static Vector2i MultiplyComponents(const Vector2i& vec1, const Vector2i& vec2)
		{
			Vector2i result;
			result.x = vec1.x * vec2.x;
			result.y = vec1.y * vec2.y;
			return result;
		}

		int DotProduct(const Vector2i& other)
		{
			return ( x * other.x + y * other.y );
		}

		Vector2i operator=(const Vector2i& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}

		Vector2i operator+=(const Vector2i& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		Vector2i operator-=(const Vector2i& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		Vector2i operator+(const Vector2i& other)
		{
			Vector2i result;
			result.x = x + other.x;
			result.y = y + other.y;
			return result;
		}

		Vector2i operator-(const Vector2i& other)
		{
			Vector2i result;
			result.x = x - other.x;
			result.y = y - other.y;
			return result;
		}

		Vector2i operator-()
		{
			Vector2i result;
			result.x = -x;
			result.y = -y;
			return result;
		}

		friend bool operator==(const Vector2i& vec1,const Vector2i& vec2)
		{
			return ( ( vec1.x == vec2.x ) && ( vec1.y == vec2.y ) );
		}

		friend bool operator!=(const Vector2i& vec1,const Vector2i& vec2)
		{
			return ( ( vec1.x != vec2.x ) || ( vec1.y != vec2.y ) );
		}

		friend bool operator<(const Vector2i& v1, const Vector2i& v2)
		{
			if(v1.x < v2.x)
			{
				return true;
			}
			else if(v1.x > v2.x)
			{
				return false;
			}
			else 
			{
				if(v1.y < v2.y)
					return true;
				else
					return false;
			}
		}

		void PrintVector()
		{
			cout << "X: " << x << "  Y: " << y << endl;
		}
};
#endif
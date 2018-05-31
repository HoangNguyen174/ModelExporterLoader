#ifndef VECTOR3_HEADER
#define VECTOR3_HEADER
#include <math.h>
#include "MathUtilities.hpp"

class Vector3
{
	public:
		float x;
		float y;
		float z;

	public:
		Vector3()
		{
			x = 0.f;
			y = 0.f;
			z = 0.f;
		}

		Vector3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector3(const Vector3& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}

		~Vector3() {}

		Vector3 Normalize()
		{
			Vector3 temp;
			float length = CalcLength();
			temp.x = x/length;
			temp.y = y/length;
			temp.z = z/length;
			return temp;
		}

		float CalcLength()
		{
			return ( sqrt( x * x + y * y + z * z ) ) ;
		}

		float CalcLengthSquare()
		{
			return ( x * x + y * y + z * z );
		}

		float DotProduct(const Vector3& other)
		{
			return ( x * other.x + y * other.y + z * other.z );
		}

		Vector3 CrossProduct(const Vector3& vec1)
		{
			Vector3 normal;
			normal.x = y * vec1.z - vec1.y * z;
			normal.y = z * vec1.x - vec1.z * x;
			normal.z = x * vec1.y - vec1.x * y;
			return normal;
		}

		void Scale(float scale)
		{
			x *= scale;
			y *= scale;
			z *= scale;
		}

		void ScaleNonUniform(float xScale, float yScale, float zScale)
		{
			x *= xScale;
			y *= yScale;
			z *= zScale;
		}

		void Reverse()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		Vector3 operator=(const Vector3& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		Vector3 operator-(const Vector3& other)
		{
			Vector3 temp;
			temp.x = x - other.x;
			temp.y = y - other.y;
			temp.z = z - other.z;
			return temp;
		}

		Vector3 operator+(const Vector3& other)
		{
			Vector3 temp;
			temp.x = x + other.x;
			temp.y = y + other.y;
			temp.z = z + other.z;
			return temp;
		}

		Vector3 operator+=(const Vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		void operator-=(const Vector3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		Vector3 operator*(const float scale)
		{
			Vector3 temp;
			temp.x = x * scale;
			temp.y = y * scale;
			temp.z = z * scale;
			return temp;
		}

		Vector3 operator*=(const float scale)
		{
			Vector3 temp(x,y,z);
			temp.x *= scale;
			temp.y *= scale;
			temp.z *= scale;
			return temp;
		}

		Vector3 operator-()
		{
			Vector3 temp(-x,-y,-z);
			return temp;
		}

		friend Vector3 operator-(const Vector3& v1, const Vector3& v2)
		{
			Vector3 temp;
			temp.x = v1.x - v2.x;
			temp.y = v1.y - v2.y;
			temp.z = v1.z - v2.z;
			return temp;
		}

		friend bool operator==(const Vector3& v1,const Vector3& v2)
		{
			if(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z )
				return true;
			return false;
		}

		friend bool operator!=(const Vector3& v1,const Vector3& v2)
		{
			if(v1.x != v2.x || v1.y != v2.y || v1.z != v2.z )
				return true;
			return false;
		}

		friend bool operator<(const Vector3& v1,const Vector3& v2)
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
				else if(v1.y > v2.y)
					return false;
				else
				{
					if(v1.z > v2.z)
						return true;
					else
						return false;
				}
			}
		}

		friend Vector3 operator*(const float& scale, const Vector3& v1)
		{
			Vector3 temp;
			temp.x = scale * v1.x;
			temp.y = scale * v1.y;
			temp.z = scale * v1.z;
			return temp;
		}
};
#endif
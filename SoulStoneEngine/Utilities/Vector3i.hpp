#ifndef VECTOR3I_HEADER
#define VECTOR3I_HEADER
#include <math.h>

class Vector3i
{	
	public:
		int x;
		int y;
		int z;

	public:
		Vector3i()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		Vector3i(int x, int y, int z)
		{
			x = x;
			y = y;
			z = z;
		}

		Vector3i(const Vector3i& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}

		~Vector3i() {}

		float CalcLength()
		{
			return ( sqrt( static_cast<float> ( x * x + y * y + z * z ) ) );
		}

		int CalcLengthSquare()
		{
			return ( x * x + y * y + z * z );
		}

		int DotProduct(const Vector3i& other)
		{
			return ( x * other.x + y * other.y + z * other.z );
		}

		void Scale(int scale)
		{
			x *= scale;
			y *= scale;
			z *= scale;
		}

		void ScaleNonUniform(int xScale, int yScale, int zScale)
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

		Vector3i operator=(const Vector3i& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		Vector3i operator-(const Vector3i& other)
		{
			Vector3i temp;
			temp.x = x - other.x;
			temp.y = y - other.y;
			temp.z = z - other.z;
			return temp;
		}

		Vector3i operator+(const Vector3i& other)
		{
			Vector3i temp;
			temp.x = x + other.x;
			temp.y = y + other.y;
			temp.z = z + other.z;
			return temp;
		}

		Vector3i operator+=(const Vector3i& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		void operator-=(const Vector3i& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		Vector3i operator*(const int scale)
		{
			Vector3i temp;
			temp.x = x*scale;
			temp.y = y*scale;
			temp.z = z*scale;
			return temp;
		}

		Vector3i operator*=(const int scale)
		{
			Vector3i temp(x,y,z);
			temp.x *= scale;
			temp.y *= scale;
			temp.z *= scale;
		}

		Vector3i operator-()
		{
			Vector3i temp;
			temp.x = -x;
			temp.y = -y;
			temp.z = -z;
			return temp;
		}

		friend bool operator==(const Vector3i& v1,const Vector3i& v2)
		{
			if(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z )
				return true;
			return false;
		}

		friend bool operator!=(const Vector3i& v1,const Vector3i& v2)
		{
			if(v1.x != v2.x || v1.y != v2.y || v1.z != v2.z )
				return true;
			return false;
		}

		friend Vector3i operator*(const int scale, const Vector3i& v1)
		{
			Vector3i temp;
			temp.x = scale * v1.x;
			temp.y = scale * v1.y;
			temp.z = scale * v1.z;
		}
};
#endif
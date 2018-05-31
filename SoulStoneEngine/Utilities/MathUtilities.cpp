#include "MathUtilities.hpp"

//empty constructor
MathUtilities::MathUtilities()
{
}

MathUtilities::~MathUtilities()
{
}

//Usage: get the nearest int fo a given float
//Parameter: number that is going to be rounded
//Output: nearest integer of given number
int MathUtilities::GetNearestInt(float num)
{
	int temp;
	//if decimal part is larger than 0.5, the number is round up
	if(abs(num - (int)num) >= 0.5)
	{
		if(num > 0)
			temp = static_cast<int>(ceil(num));
		else
			temp = static_cast<int>(floor(num));
	}
	//if decimal part is smaller than 0.5, the number is round down
	else 
	{
		if(num > 0)
			temp = static_cast<int>(floor(num));
		else
			temp = static_cast<int>(ceil(num));
	}

	return temp;
}

//Usage: get largest number between two numbers
//Parameter: two arbitrary numbers
//Output: the largest number of two given numbers
float MathUtilities::Max2(float num1, float num2)
{
	if(num1 > num2)
		return num1;
	else 
		return num2;
}

//Usage: get largest number of three numbers
//Parameter: three arbitrary numbers
//Output: the largest number of three given numbers
float MathUtilities::Max3(float num1,float num2,float num3)
{
	float temp_max = Max2(num1,num2);
	
	temp_max = Max2(temp_max,num3);

	return temp_max;
}

//Usage: get largest number of four numbers
//Parameter: four arbitrary numbers
//Output: the largest number of four given numbers
float MathUtilities::Max4(float num1,float num2,float num3,float num4)
{
	float temp_max = Max3(num1,num2,num3);

	temp_max = Max2(temp_max,num4);

	return temp_max;
}

//Usage: get smallest number of two numbers
//Parameter: two arbitrary numbers
//Output: the smallest number of two given numbers
float MathUtilities::Min2(float num1,float num2)
{
	if(num1 < num2)
		return num1;
	else
		return num2;
}

//Usage: get smallest number of three numbers
//Parameter: three arbitrary numbers
//Output: the smallest number of three given numbers
float MathUtilities::Min3(float num1,float num2,float num3)
{
	float temp_min = Min2(num1,num2);
	temp_min = Min2(temp_min,num3);

	return temp_min;
}

//Usage: get smallest number of four numbers
//Parameter: four arbitrary numbers
//Output: the smallest number of four given numbers
float MathUtilities::Min4(float num1,float num2,float num3,float num4)
{
	float temp_min = Min3(num1,num2,num3);
	temp_min = Min2(temp_min,num4);

	return temp_min;
}

/*
//Usage: get smallest number from list of numbers
//Parameter: list of arbitrary numbers
//Output: the smallest number from list of numbers
template<typename T>
float MathUtilities::GetMin(vector<T> list)
{
	return *min_element(list.begin(),list.end());
}

//Usage: get largest number from list of numbers
//Parameter: list of arbitrary numbers
//Output: the largest number from list of numbers
template<typename T>
float MathUtilities::GetMin(vector<T> list)
{
	return *max_element(list.begin(),list.end());
}*/

//Usage: Convert degree to radian
//Parameter: magnitude of angle in degree
//Output: magnitude of angle in radian
float MathUtilities::DegToRad(float degree)
{
	return (degree * PI)/180;
}

//Usage: Convert radian to degree
//Parameter: magnitude of angle in radian
//Output: magnitude of angle in degree
float MathUtilities::RadToDeg(float radian)
{
	return (radian * 180)/PI;
}

//Usage: interpolate number from given range and fraction
//Parameter: lower bound, upper bound, and fraction
//Output: interpolate number accoring to given range and fraction
float MathUtilities::Interpolate(float start,float end,float fraction)
{
	if(start > end)
	{
		ErrorMessage("Lower bound is bigger than upper bound!\n");
		return 0;
	}
	return ((end - start) * fraction) + start;
}

//Usage: clamp a number in given range
//Parameter: lower bound, upper bound, and number that will be clamped
//Output: upper bound if number > upper bound
//		  lower bound if number < lower bound
//		  number if lower bound < number < upper bound
float MathUtilities::Clamp(float lower,float upper,float num)
{
	if(lower > upper)
	{
		ErrorMessage("Lower bound is bigger than upper bound!\n");
		return 0;
	}
	if(num > upper)
		return upper;
	else if(num < lower)
		return lower;
	else return num;
}

//Usage: calculate displacement of two angles
//Parameter: magnitude of two angles in degree
//Output: displacement of two angles in degree
float MathUtilities::AngularDisplacement(float destinationDegrees, float startDegrees)
{
	float displacementDegrees = destinationDegrees - startDegrees;
	while(displacementDegrees > 180.f)
		displacementDegrees -= 360.f;
	while(displacementDegrees < - 180)
		displacementDegrees += 360.f;

	return displacementDegrees;
}

//Usage: calculate displacement angle
//Parameter: angular speed per second and elapsed time
//Output: displacement angles in degree
float MathUtilities::AngularDistance(float anglePerSec, float elapsedTime)
{
	return  anglePerSec * elapsedTime;
};

//Usage: check if given number is power of 2
//Parameter: integer
//Output: true if number is power of 2
//		  false if number is not power of 2
bool MathUtilities::IsPowerOfTwo(int num)
{
	if(num != 0 && (num & (num - 1)) == 0)
		return true;
	else return false;
};

//Usage: generate error message
//Parameter: error message
//Output: error message
void MathUtilities::ErrorMessage(char *mess)
{
	printf("Error occurs: %s\n",mess);
}

void MathUtilities::SetBit(unsigned int& bitflags, unsigned int bit)
{
	bitflags |= bit;
}

void MathUtilities::ClearBit(unsigned int& bitflags, unsigned int bit)
{
	bitflags &= ( ~bit );
}

bool MathUtilities::IsBitSet(const unsigned int bitflags, unsigned int bit)
{
	return ( ( bitflags & bit ) != 0 );
}

bool MathUtilities::CompareTwoFloatNumbers(float A, float B, float maxRelativeError )
{
	if (A == B)
		return true;
	float relativeError;
	if (fabs(B) > fabs(A))
		relativeError = fabs((A - B) / B);
	else
		relativeError = fabs((A - B) / A);
	if (relativeError <= maxRelativeError)
		return true;
	return false;
}
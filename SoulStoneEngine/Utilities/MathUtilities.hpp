#ifndef MATH_H
#define MATH_H
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;

const float PI = 3.14159265358979323846f;
const float EPSILON = 0.0000001f; // in case of comparing 2 float numbers

class MathUtilities
{
	private:
		static void ErrorMessage(char *message);

	public:
		MathUtilities();//default constructor
		~MathUtilities();//destructor
		static int GetNearestInt(float num);
		static float Max2(float num1,float num2);
		static float Max3(float num1,float num2,float num3);
		static float Max4(float num1,float num2,float num3,float num4);
		static float Min2(float num1,float num2);
		static float Min3(float num1,float num2,float num3);
		static float Min4(float num1,float num2,float num3,float num4);
		template<typename T>
		static float GetMin(vector<T> list)
		{
			return *min_element(list.begin(),list.end());
		}

		template<typename T>
		static float GetMax(vector<T> list)
		{
			return *max_element(list.begin(),list.end());
		}

		static float Clamp(float num1, float num2,float num);
		static float DegToRad(float degree);
		static float RadToDeg(float radian);
		static bool IsPowerOfTwo(int num);
		static float Interpolate(float start,float end,float fraction);	
		static float AngularDisplacement(float degree1, float degree2);
		static float AngularDisplace(float anglePerSec, float elapsedTime);
		static float AngularDistance(float anglePerSec, float elapsedTime);	
		static void SetBit(unsigned int& bitflags, unsigned int bit);
		static void ClearBit(unsigned int& bitflags, unsigned int bit);
		static bool IsBitSet(const unsigned int bitflags, unsigned int bit);
		static bool CompareTwoFloatNumbers(float a, float b, float maxUlps );
		static int GetRandomNumber(int nLow, int nHigh)
		{
			return (rand() % (nHigh - nLow + 1)) + nLow;
		}
		
};
#endif
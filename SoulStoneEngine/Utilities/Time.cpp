//---------------------------------------------------------------------------
#include "Time.hpp"
#include <Windows.h>


//---------------------------------------------------------------------------
double g_secondsPerCount = 0.0;


//---------------------------------------------------------------------------
void InitializeTime()
{
	if( g_secondsPerCount == 0.0 )
	{
		LARGE_INTEGER countsPerSecond;
		QueryPerformanceFrequency( &countsPerSecond );
		g_secondsPerCount = 1.0 / static_cast< double >( countsPerSecond.QuadPart );
	}
}


//---------------------------------------------------------------------------
double GetCurrentTimeSeconds()
{
	LARGE_INTEGER performanceCount;
	QueryPerformanceCounter( &performanceCount );

	double currentSeconds = static_cast< double >( performanceCount.QuadPart ) * g_secondsPerCount;
	return currentSeconds;
}



#pragma once

#include <math.h>

/** Simple dB gain to decimal converter,
	Not used in the end but could be useful 
	to future developers. */

class GainToDecimalConverter
{

public:

	double convertGainToDecimal(double gain)
	{
		// The equation for obtaining voltage ratio from dB is
		// V2 / V1 = 10 ^ (A / 20)
		// Available From: http://www.mogami.com/e/cad/db.html

		return pow(10, (gain / 20));
	}

private:

};
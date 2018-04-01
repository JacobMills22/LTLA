#pragma once

// Formula taken from https://stackoverflow.com/questions/929103/convert-a-number-range-to-another-range-maintaining-ratio

/** Simple class used to convert a value
	from a given range to a new range. */

template <typename ValueType>
class RangeRemapper
{

public:

	ValueType convert(ValueType valueToConvert, ValueType oldMax, ValueType oldMin, ValueType newMin, ValueType newMax)
	{
		return (((valueToConvert - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin;
	}

private:

};
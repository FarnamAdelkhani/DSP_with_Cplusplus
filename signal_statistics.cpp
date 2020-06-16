#include <cmath>

//Calculate Signal Mean:
double signal_statistics::calc_signal_mean()
{
	double mean = 0.0;

	//Sum up all input values
	for (int i = 0; i < signal_length; i++)
	{
		mean = mean + input_array[i];
	}

	//Divide by total length
	mean = mean / signal_length;

	return mean;
}
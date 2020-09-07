#include "signal_statistics.hpp"

#include <cmath>
#include <Eigen/Dense>

void find_cpu_endianness()
{
	int x = 1;

	//Get the address, convert it to char pointer, and then dereference.
	bool littleEndian = (*(char*)(&x) == 1) ? true : false;

	//Break from program if 0x01 is not at lowest mem address
	assert(littleEndian);
};

signal_statistics::signal_statistics(double* _source_signal, int _signal_length)
{
	source_signal = _source_signal;
	signal_length = _signal_length;
};

double signal_statistics::calc_signal_mean()
{
	double _mean = 0.0;

	//accumulate into local variable (_mean)
	for (int i = 0; i < signal_length; i++)
	{
		_mean = _mean + source_signal[i];
	}

	//cast sig_length to double
	_mean = _mean / (double)signal_length;

	return _mean;
}

double signal_statistics::calc_signal_variance()
{
	double variance = 0.0;
	double signal_mean = calc_signal_mean();

	for (int i = 0; i < signal_length; i++)
	{
		variance = variance + pow((source_signal[i] - signal_mean), 2);
	}

	variance = variance / ((double)signal_length - 1);
	return variance;
};

double signal_statistics::calc_signal_std() 
{

	double sig_variance = calc_signal_variance();

	//Standard deviation is the square root of the variance.
	double std = sqrt(sig_variance);

	return std;
}

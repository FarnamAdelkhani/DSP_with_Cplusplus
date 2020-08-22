#include <cmath>
#include "signal_statistics.h"

#include <Eigen/Dense>

signal_statistics::signal_statistics(double* _source_signal, int _signal_length)
{
	source_signal = _source_signal;
	signal_length = _signal_length;
};


double signal_statistics::calc_signal_mean()
{
	double mean;

	//Sum up all source signal values
	for (int i = 0; i < signal_length; i++)
	{
		mean = mean + source_signal[i];
	}

	//Divide by total signal length
	mean = mean / signal_length;

	return mean;
}

double signal_statistics::calc_signal_variance()
{
	double variance;
	double signal_mean = calc_signal_mean();

	for (int i = 0; i < signal_length; i++)
	{
		variance = variance + pow((source_signal[i] - signal_mean), 2);
	}

	variance = variance / (signal_length - 1);
	return variance;

};

double signal_statistics::calc_signal_std() 
{

	double sig_variance = calc_signal_variance();

	//Standard deviation is the square root of the variance.
	double std = sqrt(sig_variance);

	return std;
}
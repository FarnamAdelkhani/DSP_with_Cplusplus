#include <cmath>
#include "signal_statistics.h"

/**
 * @brief Pointer to signal source array.
 *
 * @param[in] _source_signal Pointer to signal source array.
 * @param[in] _signal_length Length of course array.
 *
 * @return none
 */
signal_statistics::signal_statistics(double* _source_signal, int _signal_length)
{
	source_signal = _source_signal;
	signal_length = _signal_length;
};


/**
 * @brief Calculate signal mean.
 *
 * @param[in] _source_signal Pointer to signal source array.
 * @param[in] _signal_length Length of course array.
 *
 * @return Signal mean
 */
double signal_statistics::calc_signal_mean()
{
	double mean = 0.0;

	//Sum up all source signal values
	for (int i = 0; i < signal_length; i++)
	{
		mean = mean + source_signal[i];
	}

	//Divide by total signal length
	mean = mean / signal_length;

	return mean;
}
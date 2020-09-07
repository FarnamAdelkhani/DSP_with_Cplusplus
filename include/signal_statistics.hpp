#ifndef SIGNAL_STATISTICS_H
#define SIGNAL_STATISTICS_H

/// @brief Function to determine if processor is set to little or big endianness.
	///        Higher memory   ----->
	///    [ 0x01 | 0x00 | 0x00 | 0x00 ] <- Little endian
	///    [ 0x00 | 0x00 | 0x00 | 0x01 ] <- Big endian
	/// This function checks to see if the int 1 is stored in the lowest memory address.
void find_cpu_endianness();


class signal_statistics 
{
private:

	double* source_signal;
	int signal_length;

public:

	/// @brief Pointer to signal source array.
	///
	/// @param _source_signal Pointer to signal source array.
	/// @param _signal_length Length of course array.
	signal_statistics(double* _source_signal, int _signal_length);

	/// @brief Calculate signal mean.
	/// @return Signal mean
	double calc_signal_mean();

	/// @brief Calculate the variance signal.
	/// @return Signal variance as a double
	double calc_signal_variance();

	/// @brief Standard deviation is calculated as the square root of 
	///        variance by figuring out the variation between each data 
	///        point relative to the mean. If the points are further from 
	///        the mean, there is a higher deviation within the signal; if 
	///        they are closer to the mean, there is a lower deviation.
	/// @return Standard deviation as double.
	double calc_signal_std();

};

#endif
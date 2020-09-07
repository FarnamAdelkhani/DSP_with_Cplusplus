#include <vector>
#include <string>
#include <math.h>

///TODO Update the DOXY comments


/// @brief 
/// @param channelData 
/// @param numChannels 
/// @param numSamples 
void audioCallback(float** channelData, int numChannels, int numSamples);

/// @brief 
/// @param channelData 
/// @param numChannels 
/// @param numSamples 
void simple_audioCallback(float** channelData, int numChannels, int numSamples);

/// @brief Basic convolution function.
/// @param impulse_response          kernel
/// @param input_array               input signal
/// @param output_array              output signal
/// @param input_signal_length       length of input
/// @param impulse_response_length   length of filter kernel
void convolution(
	double* impulse_response,
	double* input_array,
	double* output_array,
	int input_signal_length,
	int impulse_response_length );

/// @brief 
/// @param A 
/// @param B 
/// @return 
bool compare_4bytes(const FOUR_BYTES A, const FOUR_BYTES B);

/// @brief 
/// @param input_signal 
/// @param ChunkIDtoFind 
/// @return 
int find_chunk(std::istream& input_signal, const FOUR_BYTES ChunkIDtoFind);

/// @brief 
/// @param input_signal 
/// @param sample_rate 
/// @param num_samples 
/// @param num_channels 
/// @param bits_per_sample 
/// @return 
int readWavHeader(std::istream& input_signal, int& sample_rate, int& num_samples, short& num_channels, short& bits_per_sample);


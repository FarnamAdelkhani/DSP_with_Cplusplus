// A Flexible Digital Signal Processing Pipeline

// Two golden rules for audio code:
// 1 - Never cause audio dropouts.
//     -> Meaning, do not fail to write the audio buffer in time.
// 2 - Don't do anything where you cannot predict the time it will take.
//     -> Do not call anything that blocks or waits.

#include <iostream>
#include <fstream>
#include <thread>
#include <cmath>

#include "signal_statistics.hpp"
#include "input_signals_ascii.hpp"
#include "audio_algorithms.hpp"

#include <SDL.h> //Simple direct-media layer

#define FILTER_LENGTH 30
#define SIGNAL_LENGTH 360

//Sample .WAV input signals
#define WAV_FILE_PATH "./sample_wav_files/equinox_48KHz_15sec.wav"

//Sample ASCII input signals
extern double low_pass_filter[FILTER_LENGTH];
extern double violin_sample_44KHz[SIGNAL_LENGTH];

//Kernels and ouputs
double output_signal[FILTER_LENGTH + SIGNAL_LENGTH];
double filter[FILTER_LENGTH];

int main(int argc, char**)
{
	//Print thread ID
	std::cout << "Main started, thread id=" << std::this_thread::get_id() << std::endl;

	std::ofstream input, filter, output;

	input.open("_input.dat");
	filter.open("_filter.dat");
	output.open("_output.dat");  //Output from convolution sum algorithm



	convolution(
		low_pass_filter,
		violin_sample_44KHz,
		output_signal,
		SIGNAL_LENGTH,
		FILTER_LENGTH);


	for (int i = 0; i < SIGNAL_LENGTH + FILTER_LENGTH; i++)
	{
		output << output_signal[i] << std::endl;

		if (i < FILTER_LENGTH)
		{
			filter << low_pass_filter[i] << std::endl;
		}

		if (i < SIGNAL_LENGTH)
		{
			input << violin_sample_44KHz[i] << std::endl;
		}
	}
	
	// *********************************************************************************
    /// -------------------- Signal Statistics Calculations: --------------------------- 
	
	/// Functions to find signal mean, variance and standard deviation
	double signal_Mean, signal_Variance, signal_STD;

	//Set 'signal_statistics' class constructors to input signal and size of that signal
	signal_statistics* prog_signal_stats = new signal_statistics(&violin_sample_44KHz[0], 
		sizeof( violin_sample_44KHz ) / sizeof(violin_sample_44KHz[0]) );

	signal_Mean = prog_signal_stats->calc_signal_mean();
	signal_Variance = prog_signal_stats->calc_signal_variance();
	signal_STD = prog_signal_stats->calc_signal_std();

	std::cout << "- Signal Statistics -" << std::endl;
	std::cout << "Signal Mean = " << signal_Mean << std::endl;
	std::cout << "Signal Variance = " << signal_Variance << std::endl;
	std::cout << "Signal Standard Deviation = " << signal_STD << std::endl;

	// *********************************************************************************

	input.close();
	filter.close();
	output.close();

	return 0;
}
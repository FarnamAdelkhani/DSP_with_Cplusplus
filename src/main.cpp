// A Flexible Digital Signal Processing Pipeline

#include <iostream>
#include <fstream>
#include <thread>
#include <cmath>

#define FILTER_LENGTH 30
#define SIGNAL_LENGTH 360

double output_signal[FILTER_LENGTH + SIGNAL_LENGTH];
double filter[FILTER_LENGTH];

extern double violin_sample_44KHz[360];
extern double low_pass_filter[30];

//Identifier
void convolution(double* impulse_response, double* input_array, double* output_array, int input_signal_length, int impulse_response_length);

int main(int argc, char**)
{
	//Print thread ID
	std::cout << "Main started, thread id=" << std::this_thread::get_id() << std::endl;

	std::ofstream input, filter, output;

	input.open("_input.dat");
	filter.open("_filter.dat");
	output.open("_output.dat");

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

	input.close();
	filter.close();
	output.close();

	return 0;

}
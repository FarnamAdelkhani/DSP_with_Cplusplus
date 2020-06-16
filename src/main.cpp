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

//Basic Convolution:
void convolution(
	double* impulse_response,    //kernel
	double* input_array,         //input signal
	double* output_array,        //output signal
	int input_signal_length,     //length of input
	int impulse_response_length) //length of filter kernel
{
	//Fill the signal with 0s
	int i, j;
	for (int i = 0; i < (input_signal_length + impulse_response_length); i++)
	{
		output_array[i] = 0;
	}

	//Convolution:
	for (i = 0; i < input_signal_length; i++)
	{
		for (j = 0; j < impulse_response_length; j++)
		{
			//Convolution Sum Equation:
			output_array[i + j] = output_array[i + j] + input_array[i] * impulse_response[j];
		}
	}
}

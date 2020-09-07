#include "audio_algorithms.hpp"
#include "signal_statistics.hpp"

#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <algorithm>

#include <ranges> //C++ 20 specific

#include <SDL.h>  //Simple direct-media layer

#include <Eigen/Dense>

#define M_PI   3.141592653589793238

typedef char FOUR_BYTES[4];

//Simple audio callback:
void simple_audioCallback(float** channelData, int numChannels, int numSamples)
{
	//Loop channels + samples
	for (int channel = 0; channel < numChannels; ++channel)
		for (int sample = 0; sample < numSamples; ++sample)

			//Can perform some complex DSP work here:
			channelData[channel][sample] = 0.0f;
}

//Robust audio callback:
void audioCallback(
		float** inputChannelData,   //Audio input from sound card
		float** outputChannelData,  //Audio output to the sound card
		int numInputChannels,
		int numOutputChannels,
		int numFrames)
{
	//TODO: Implement the more robust method
}


//Convolution function for a linear time-invariant signal
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

	//Convolve...
	for (i = 0; i < input_signal_length; i++)
	{
		for (j = 0; j < impulse_response_length; j++)
		{
			//Convolution Sum Equation:
			// y[n] = x[n] * h[n]
			output_array[i + j] = output_array[i + j] + input_array[i] * impulse_response[j];
		}
	}
}

///NOTE Experimental - might delete soon
//Play a .wav file with SDL library
void play_wav_file()
{
	SDL_Init(SDL_INIT_AUDIO);

	SDL_AudioSpec wav_spec;
	Uint32 wav_length;
	Uint8* wav_buffer;

	//Load the .WAV
	if (SDL_LoadWAV("test.wav", &wav_spec, &wav_buffer, &wav_length) == NULL) 
	{
		fprintf(stderr, "Could not open test.wav: %s\n",s SDL_GetError());
	}
	else 
	{
		/* Do stuff with the WAV data, and then... */
		SDL_FreeWAV(wav_buffer);
	}
};

/// TODO Write this function
void read_wav_file()
{

};

//If 4-byte inputs A & B are equal, return true
bool compare_4bytes(const FOUR_BYTES A, const FOUR_BYTES B)
{
	//'strncmp' returns 0 if first 4 chars in a and b compare equal
	return (strncmp(A, B, 4) == 0);
};

int find_chunk(std::istream &input_signal, const FOUR_BYTES ChunkIDtoFind)
{
	int position = 0;
	const int TOTAL_CHUNKS = 20;

	//While we haven't reached the end of the file
	while (position < TOTAL_CHUNKS && !input_signal.eof())
	{

	}
};

int readWavHeader (
			std::istream &input_signal,
			int          &sample_rate,
			int          &num_samples,
			short        &num_channels,
			short        &bits_per_sample )
{
	//Check if processor is little or big endian
	find_cpu_endianness();

	//	The canonical WAVE format starts with the RIFF header:
	//
	// -------------------------------------------------------------------------
	// 0     4 bytes - ChunkID   
	// -------------------------------------------------------------------------
	// Contains the letters "RIFF" in ASCII for (0x52494646 big-endian form).
	// -------------------------------------------------------------------------
	FOUR_BYTES ChunkID;
	input_signal.read(ChunkID, sizeof(ChunkID));
	//Use assert to break if false
	assert(input_signal.gcount() == sizeof(ChunkID));
	assert(compare_4bytes("RIFF", ChunkID));

	// -------------------------------------------------------------------------
	//  4     4 bytes - ChunkSize 
	// -------------------------------------------------------------------------
	// 36 + SubChunk2Size, or more precisely:
	//	4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	// This is the size of the rest of the chunk
	// following this number.  This is the size of the
	// entire file in bytes minus 8 bytes for the
	// two fields not included in this count: ChunkID and ChunkSize.
	// -------------------------------------------------------------------------
	int chunkSize;
	input_signal.read((char*)&chunkSize, sizeof(chunkSize));
	assert(input_signal.gcount() == sizeof(ChunkID));

	// -------------------------------------------------------------------------
	//  8     4 bytes - Format           
	// -------------------------------------------------------------------------
	// Contains the letters "WAVE" (0x57415645 big-endian form).
	// -------------------------------------------------------------------------
	input_signal.read(ChunkID, sizeof(ChunkID));
	assert(input_signal.gcount() == sizeof(ChunkID));
	assert(compare_4bytes("WAVE", ChunkID));

	//'tellg' returns input position indicator of the current associated streambuf object
	double input_signal_position = input_signal.tellg();

	// ------------------------------------------------------
	// The "fmt " subchunk describes the sound data's format (12-35):
	// ------------------------------------------------------

	//Find current position on the input signal
	input_signal.seekg(input_signal_position);
	
	// -------------------------------------------------------------------------
	//  12     4 bytes - Subchunk1ID
	// -------------------------------------------------------------------------
	// Contains the letters "fmt " (0x666d7420 big - endian form).
	// -------------------------------------------------------------------------

	//'find_chunk' function used to pickup at 'input_signal_position'
	///TODO Create this function
	int fmtChunkSize = find_chunk(input_signal, "fmt ");

	// -------------------------------------------------------------------------
	//  16     4 bytes - Subchunk1Size    
	// -------------------------------------------------------------------------
	// 16 for PCM.This is the size of the rest of the Subchunk which 
	// follows this number.
	// -------------------------------------------------------------------------
	//TODO Read chunk size

	// -------------------------------------------------------------------------
	//  20     2 bytes - AudioFormat
	// -------------------------------------------------------------------------
	// PCM = 1 (i.e. Linear quantization) 
	// Values other than 1 indicate some form of compression.
	// -------------------------------------------------------------------------
	short audio_format;
	input_signal.read((char*)&audio_format, sizeof(audio_format));

	// -------------------------------------------------------------------------
	//  22     2 bytes - NumChannels      
	// -------------------------------------------------------------------------
	//  Mono = 1, Stereo = 2, etc.
	// -------------------------------------------------------------------------
	input_signal.read((char*)&num_channels, sizeof(num_channels));

	// -------------------------------------------------------------------------
	//	24     4 - SampleRate
	// -------------------------------------------------------------------------
	//  8000, 44100, etc.
	// -------------------------------------------------------------------------
	input_signal.read((char*)&sample_rate, sizeof(sample_rate));

	// -------------------------------------------------------------------------
	//	28     4 - ByteRate         
	// -------------------------------------------------------------------------
	//  == SampleRate * NumChannels * BitsPerSample/8
	//  int byteRate = sampleRate * numChannels * bitsPerSample/8;
	// -------------------------------------------------------------------------
	int byteRate;
	input_signal.read((char*)&byteRate, sizeof(byteRate));

	// -------------------------------------------------------------------------
	//	32     2 - BlockAlign      
	// -------------------------------------------------------------------------
	//  == NumChannels * BitsPerSample/8
	//  The number of bytes for one sample including all channels. 
	//  I wonder what happens when this number isn't an integer?
	// -------------------------------------------------------------------------
	short block_align;
	input_signal.read((char*)&block_align, sizeof(block_align));

	// -------------------------------------------------------------------------
	//	34     2 - BitsPerSample    8 bits = 8, 16 bits = 16, etc.
	//		   2 - ExtraParamSize   if PCM, then doesn't exist
	//		   X - ExtraParams      space for extra parameters
	// -------------------------------------------------------------------------
	input_signal.read((char*)&bits_per_sample, sizeof(bits_per_sample));

	// -------------------------------------------------------------------------
	// The "data" subchunk (36 - 44):
	// -------------------------------------------------------------------------
	input_signal.seekg(input_signal_position);
	int chunk_size = find_chunk(input_signal, "data");

	num_samples = chunk_size / block_align;

	return num_samples;
}

///TODO Implement the volume knob using std::atomic for threading
/// 
//// Simple volume knob implementation using atomic:
//class Synthesizer
//{
//public:
//
//	Synthesizer() : level(1.0f) {}
//
//	//GUI thread: get some callback from mouse event, then level is changed
//	void levelChanged(float newValue)
//	{
//		//<atomic>
//		level.store = newValue;
//	}
//
//private:
//	//Audio thread:
//	void audioCallback(float* buffer,
//		int numSamples) noexcept
//	{
//		for (int i = 0; i < numSamples; ++i)
//			//<atomic>
//			buffer[i] = level.load * getNextAudioSample();
//	}
//	//<atomic>
//	std::atomic<float> level;
//};
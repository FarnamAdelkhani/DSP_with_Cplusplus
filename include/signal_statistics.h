#ifndef SIGNAL_STATISTICS_H
#define SIGNAL_STATISTICS_H

class signal_statistics 
{
private:

	double* source_signal;
	int signal_length;

public:
	signal_statistics(double* _source_signal, int _signal_length);
	double calc_signal_mean();

};

#endif
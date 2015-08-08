#ifndef JOB_H
#define JOB_H

#include <string>

using namespace std;

class Job
{
public:
	string _job_name;
	int _cpus_required;
	int _duration;
	int _duration_remaining;
	int _starting_tick;
	int _ending_tick;
	
	//Getters and setters are not really important for a program of this small size and simplicity
	
	Job(string job_name, int cpus_required, int duration)
	{
		_job_name = job_name;
		_cpus_required = cpus_required;
		_duration = duration;
		_duration_remaining = duration;
	}
};

#endif // !JOB_H

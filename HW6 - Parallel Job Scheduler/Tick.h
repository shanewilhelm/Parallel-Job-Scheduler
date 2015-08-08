#ifndef TICK_H
#define TICK_H

#include <queue>
#include <vector>
#include <fstream>
#include <memory>
#include <sstream>
#include "Job.h"

struct jobCompare
{
	bool operator()(const shared_ptr<Job> &j1, const shared_ptr<Job> &j2)
	{ //Sort duration from low to high
		return (j1->_duration > j2->_duration);
	}
};

void tick(priority_queue<shared_ptr<Job>, vector<shared_ptr<Job>>, jobCompare> &job_queue, vector<shared_ptr<Job>> &active_jobs, ifstream &jobs_file, ofstream &results_file, ofstream &output_file);

#endif // !TICK_H

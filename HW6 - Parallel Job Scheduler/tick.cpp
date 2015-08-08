#include "Tick.h"

extern int MAX_CPUS;
extern int RESOURCE_POOL;
extern int CURRENT_TICK;

//Add job from file if there is a job to add
void addJob(priority_queue<shared_ptr<Job>, vector<shared_ptr<Job>>, jobCompare> &job_queue, ifstream &jobs_file, ofstream &output_file)
{
	string line, jobName, cpusRequired, duration;
	shared_ptr<Job> job;

	output_file << "Processing jobs file..." << endl;

	//Extract jobName, cpusRequired, and duration information from the line
	getline(jobs_file, line);
	stringstream ss(line);
	ss >> jobName >> cpusRequired >> duration;

	output_file << "The next job in the list is: " << jobName << "." << endl;

	//Only add a job if the line actually contained a job. 
	if (jobName != "" && jobName != "NULL" && jobName != "null")
	{
		output_file << "Adding job to jobs queue..." << endl << endl;
		shared_ptr<Job> job{ new Job(jobName, stoi(cpusRequired), stoi(duration)) };
		job->_starting_tick = CURRENT_TICK;
		job_queue.push(job);

		if (job->_cpus_required > MAX_CPUS)
		{
			throw exception("Max number of CPUs exceeded");
		}
		
		//One job per tick
		return;
	}
}

void addToActiveJobs(priority_queue<shared_ptr<Job>, vector<shared_ptr<Job>>, jobCompare> &job_queue, vector<shared_ptr<Job>> &active_jobs, ofstream &output_file)
{
	shared_ptr<Job> currentJob;

	output_file << "Processing jobs queue..." << endl;
	output_file << "Current number of scheuled jobs: " << job_queue.size() << '.' << endl;

	if (job_queue.empty())
	{
		output_file << "Job queue empty." << endl << endl;
		return;
	}
	if (RESOURCE_POOL == 0)
	{
		output_file << "No CPUs available. Waiting for resources..." << endl << endl;
		return;
	}

	while (!job_queue.empty())
	{ //Add as many jobs as we have available CPUs for. 
		currentJob = job_queue.top();
		if (currentJob->_cpus_required <= RESOURCE_POOL)
		{ //We have enough available CPUs for the next job!
			output_file << "Adding highest priority job:" << endl;
			output_file << currentJob->_job_name << "\t" << currentJob->_cpus_required << "\t" << currentJob->_duration << endl;
			job_queue.pop();
			active_jobs.push_back(currentJob);
			RESOURCE_POOL -= currentJob->_cpus_required;
		}
		else
		{
			output_file << "Not enough CPUs available. (Requested: " << currentJob->_cpus_required << " Available: " << RESOURCE_POOL << "). ";
			output_file << "Waiting for resources..." << endl << endl;
			return;
		}
	}

	output_file << "Job queue empty." << endl << endl;
	return;
}

void executeJobs(vector<shared_ptr<Job>> &active_jobs, ofstream &output_file)
{
	output_file << "Running active jobs..." << endl;
	for (int i = 0; i < active_jobs.size(); ++i)
	{ //Do 1 tick worth of work on each active_job
		--(active_jobs[i]->_duration_remaining);
	}
}

void jobToCsv(shared_ptr<Job> job, ofstream &results_file)
{
	stringstream ss;
	int calculatedDuration = (job->_ending_tick - job->_starting_tick);

	ss << '"' << job->_job_name << "\",\"" << job->_starting_tick << "\",\"" << job->_ending_tick << "\",\"" << calculatedDuration << "\"";
	results_file << ss.str() << endl;
}

void removeJobs(vector<shared_ptr<Job>> &active_jobs, ofstream &results_file, ofstream &output_file)
{
	for (vector<shared_ptr<Job>>::iterator iter = active_jobs.begin(); iter != active_jobs.end(); )
	{
		if ((*iter)->_duration_remaining == 0)
		{ //If all work for job is done
			output_file << "Job " << (*iter)->_job_name << " is complete. Freeing resources..." << endl;
			RESOURCE_POOL += (*iter)->_cpus_required;
			(*iter)->_ending_tick = CURRENT_TICK;

			jobToCsv((*iter), results_file);
			iter = active_jobs.erase(iter); //Erase returns the next iter in the series
		}
		else
		{
			++iter;
		}
	}

	output_file << endl << endl;
	return;
}

void tick(priority_queue<shared_ptr<Job>, vector<shared_ptr<Job>>, jobCompare> &job_queue, vector<shared_ptr<Job>> &active_jobs, ifstream &jobs_file, ofstream &results_file, ofstream &output_file)
{
	output_file << "*** Tick: " << CURRENT_TICK << " ***" << endl;
	addJob(job_queue, jobs_file, output_file);
	addToActiveJobs(job_queue, active_jobs, output_file);
	executeJobs(active_jobs, output_file);
	removeJobs(active_jobs, results_file, output_file);
}
#include <fstream>
#include <iostream>
#include "Tick.h"

//I know, global variable, BOO! But it is the cleanest way to implement my solution.
int MAX_CPUS = 0;
int RESOURCE_POOL = 0;
int CURRENT_TICK = 1;

using namespace std;


int main(void)
{
	priority_queue<shared_ptr<Job>, vector<shared_ptr<Job>>, jobCompare> jobQueue;
	vector<shared_ptr<Job>> activeJobs;
	ofstream outputFile("myoutput.txt"), resultsFile("myresults.csv");
	ifstream jobsFile;
	string line;

	cout << "Please enter the Jobs file: ";
	getline(cin, line);
	jobsFile.open(line);
	if (!jobsFile.is_open())
	{
		cout << "Could not open " << line << ". Exiting..." << endl;
		return 1;
	}

	getline(jobsFile, line); //line with MAX_CPU information
	MAX_CPUS = stoi(line);
	RESOURCE_POOL = MAX_CPUS;

	//Header line for results csv
	resultsFile << "\"Job Name\",\"Starting Tick\",\"Ending Tick\",\"Duration\"" << endl;

	do
	{
		tick(jobQueue, activeJobs, jobsFile, resultsFile, outputFile);
		++CURRENT_TICK;
	} while (!jobQueue.empty() || !activeJobs.empty());

	outputFile.close();
	resultsFile.close();
	jobsFile.close();

	return 0;
}
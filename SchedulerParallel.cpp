// Xandra Best
// 1/23/15
// Scheduler Application

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "classes.h"
#include "classes.cpp"
using namespace std;

#define NOBODY -1

int factorial(int times, int empty) {
	int result;
    if (times == empty)
        return 1;
    else
        return times * factorial(times-1, empty);
}

int main(int argc, const char* argv[]) {
	/* Initializing Variables */
	// Arguments
	int threadct = 1; // Number of threads (default = 1)
	ifstream availabilityFile("StudentAvailability8.csv"); // input file

	// Initialize important custom objects
	TimeList catalog; // Holds available lesson times
	StudentList roster; // Holds students to be scheduled
	ScheduleGroup allPossible; // Holds all viable schedules
	Student nobody("Nobody"); // blank student "Nobody"

	// Set starting
	unsigned int numStudents; // Number of students to be scheduled
	unsigned int numTimeSlots; // Number of available lesson times
	unsigned int hourLongCount = 0; // Number of students requiring hour-long lessons
	unsigned long long numPermutations; // Number of schedule permutations
	vector<int> scheduleVec; // vector for handling schedule permutations
	unsigned int scheduleVecSize; // size of scheduleVec

	// variables for reading in from file
	unsigned int studRef, timeRef; // ints for referencing students and times
	string weekDaysS, timesS, studentLineS; //strings for holding a line of file input
	stringstream weekDaysSS, timesSS, studentLineSS; //stringstreams for holding a line of file input
	string junk, weekDayCheck, timeCheck, hourLongCheck; //strings for holding raw tokens of input
	string weekDay, name, availability;	//strings for holding modified tokens of input
	int hour, minute, delimiter, preference; //ints for holding modified tokens of input

	// iterators
	unsigned long long k; // for logic loop
	unsigned int i, h; // for vector sizes
	int j;

	// local variables for logic loop
	vector<int> localScheduleVec;
	// Schedule
	Schedule localSchedule;
	bool viable;
	float preferenceRank;
	// Assignment
	Assignment localAssignment1;
	Assignment localAssignment2;
	bool hourAssignment;


	/* Parse command-line args */
	if (argc > 1) {
		threadct = atoi(argv[1]);
	}


	/* Read in all info from file */
	// Read in time slots
	availabilityFile >> weekDaysS;
	weekDaysSS << weekDaysS << ",";
	getline(weekDaysSS, junk, ',');
	getline(weekDaysSS, junk, ',');

	availabilityFile >> timesS;
	timesSS << timesS << ",";
	getline(timesSS, junk, ',');
	getline(timesSS, junk, ',');

	while(getline(weekDaysSS, weekDayCheck, ',')) {
		if (weekDayCheck != "") {
			weekDay = weekDayCheck;
		}
		getline(timesSS, timeCheck, ',');
		delimiter = timeCheck.find(":");
		hour = stoi(timeCheck.substr(0,delimiter));
		minute = stoi(timeCheck.substr(delimiter+1,delimiter+3));
		catalog.add(Time(weekDay,hour,minute));
	}

	// Read in students
	studRef = 0;
	while (availabilityFile >> studentLineS) {
		timeRef = 0;
		studentLineSS << studentLineS << ",";
		getline(studentLineSS, hourLongCheck, ',');
		getline(studentLineSS, name, ',');
		roster.add(Student(name));
		if (hourLongCheck == "*") {
			roster.studentList[studRef].hourLong = true;
			hourLongCount++;
		}
		else {
			roster.studentList[studRef].hourLong = false;
		}
		while(getline(studentLineSS, availability, ',')) {
	    	if (availability != "0") {
		    	roster.studentList[studRef].availability.push_back(& catalog.timeList[timeRef]);
		    	preference = atoi(availability.c_str());
		    	roster.studentList[studRef].preference.push_back(preference);
		    }
	    	timeRef++;
		}	
		studentLineSS.clear();
		studRef++;
	}

	/* Prep for logic loop */
	// set variables necessary for logic
	numStudents = roster.studentList.size();
	numTimeSlots = catalog.timeList.size();
	numPermutations = factorial(numTimeSlots-hourLongCount, numTimeSlots-numStudents-hourLongCount);

	// Prep scheduleVec
	// Index for each student
	for (i = 0; i < numStudents; i++) {
		scheduleVec.push_back(i);
	}
	// Index -1 for each nobody
	for (i = 0; i < (numTimeSlots - numStudents - hourLongCount); i++) {
		scheduleVec.push_back(NOBODY);
	}
	sort(scheduleVec.begin(), scheduleVec.end());
	scheduleVecSize = scheduleVec.size();

	/* Logic Loop */
	// First run of loop, guaranteeing correct first permutation
	localScheduleVec = scheduleVec;
	/*
	cout << "[ ";
	for (h = 0; h < scheduleVecSize; h++) {
		cout << localScheduleVec[h];
		if (h != scheduleVecSize-1) {
			cout << ", ";
		}
	}
	cout << " ]" << endl;
	*/

	// Empty local schedule for new loop
	localSchedule.clear();

	// Reset local preference rank and iterator
	preferenceRank = 0;
	j = 0;

	// Loop through assignments represented by permutation
	for (i = 0; i < scheduleVecSize; i++) {
		//bool hourAssignment = false;
		hourAssignment = false;

		// Empty local assignments for new loop
		localAssignment1.clear();
		localAssignment2.clear();

		// If assigned to nobody, special args
		if (scheduleVec[i] == NOBODY) {
			localAssignment1.setArgs(catalog.timeList[j], nobody);
			localAssignment1.availability = true;
			localAssignment1.preferenceRank = 0;
		}
		// If assigned to somebody, set args to appropriate locations
		else {
			localAssignment1.setArgs(catalog.timeList[j], roster.studentList[scheduleVec[i]]);
			localAssignment1.checkAvailability();
			// Extra ssignment for hour long
			if (roster.studentList[scheduleVec[i]].hourLong) {
				j++;
				hourAssignment = true;
				localAssignment2.setArgs(catalog.timeList[j], roster.studentList[scheduleVec[i]]);
				localAssignment2.checkAvailability();
			}
		}

		// Check if assignment(s) is/are viable
		if (hourAssignment) {
			viable = (localAssignment1.availability && localAssignment2.availability);
			if (viable) {
				viable = checkHourSequence(localAssignment1, localAssignment2);
			}
		}
		else {
			viable = localAssignment1.availability;
		}

		// If not viable, move on to next schedule
		if (!viable) {
			break;
		}

		// If viable, add Assignment to schedule, move on to next assignment
		localSchedule.add (localAssignment1);

		// If hour long, add second assignmet and adjust preference.			
		if (hourAssignment) {
			localSchedule.add(localAssignment2);
			preferenceRank += (localAssignment1.preferenceRank / (float)2);
			preferenceRank += (localAssignment2.preferenceRank / (float)2);
		}
		// If not, add preference normally
		else {
			preferenceRank += localAssignment1.preferenceRank;
		}
		j++;
	} 
	
	// If all assignments were viable, calculate rank and add schedule 
	if (viable) {
		preferenceRank = preferenceRank / (float) numStudents;
		localSchedule.preferenceRank = preferenceRank;
		allPossible.add(localSchedule);
	}

	// loop proper
	#pragma omp parallel for num_threads(threadct) \
		private(k, j, i, h, localSchedule, preferenceRank, hourAssignment, localAssignment1, localAssignment2, viable, localScheduleVec)
	for (k = 1; k < numPermutations; k++) {

		// CRITICAL Move to next permutation and set localScheduleVec to match.
		#pragma omp critical
		{
			next_permutation(scheduleVec.begin(),scheduleVec.end());

			localScheduleVec = scheduleVec;
			/*
			cout << "[ ";
			for (h = 0; h < scheduleVecSize; h++) {
				cout << localScheduleVec[h];
				if (h != scheduleVecSize-1) {
					cout << ", ";
				}
			}
			cout << " ]" << endl;	
			*/
		}

		// Reset local variables for current SCHEDULE loop
		localSchedule.clear();
		preferenceRank = 0;
		j = 0;

		// Loop through assignments of this schedule represented by permutation
		for (i = 0; i < scheduleVecSize; i++) {
			// Reset local variables for current ASSIGNMENT loop
			hourAssignment = false;
			localAssignment1.clear();
			localAssignment2.clear();

			// If time assigned to nobody, special args, availability = true;
			if (localScheduleVec[i] == NOBODY) {
				localAssignment1.setArgs(catalog.timeList[j], nobody);
				localAssignment1.availability = true;
				localAssignment1.preferenceRank = 0;
			}
			// If time assigned to somebody, set args to appropriate locations, set availability
			else {
				localAssignment1.setArgs(catalog.timeList[j], roster.studentList[localScheduleVec[i]]);
				localAssignment1.checkAvailability();
				// Extra ssignment for hour long
				if (roster.studentList[localScheduleVec[i]].hourLong) {
					j++;
					hourAssignment = true;
					localAssignment2.setArgs(catalog.timeList[j], roster.studentList[localScheduleVec[i]]);
					localAssignment2.checkAvailability();
				}
			}

			// Check viability of assignment (or assignments if hour long)
			if (hourAssignment) {
				viable = (localAssignment1.availability && localAssignment2.availability);
				if (viable) {
					viable = checkHourSequence(localAssignment1, localAssignment2);
				}
			}
			else {
				viable = localAssignment1.availability;
			}

			// If not a viable ASSIGNMENT, move on to next SCHEDULE
			if (!viable) {
				break;
			}

			// If viable, add Assignment to schedule, move on to next assignment
			localSchedule.add (localAssignment1);

			// If hour long, add second assignment to schedule and add preference by half
			if (hourAssignment) {
				localSchedule.add(localAssignment2);
				preferenceRank += (localAssignment1.preferenceRank / (float)2);
				preferenceRank += (localAssignment2.preferenceRank / (float)2);
			}
			// If not, add preference normally
			else {
				preferenceRank += localAssignment1.preferenceRank;
			}
			j++;
		
		}
		
		// If all ASSIGNMETNTS in the SCHEDULE were viable, calculate rank and add SCHEDULE 
		if (viable) {
			preferenceRank = preferenceRank / (float) numStudents;
			localSchedule.preferenceRank = preferenceRank;
			#pragma omp critical
			{
				allPossible.add(localSchedule);
			}
		}
	}
	cout << endl;

	sort(allPossible.scheduleGroup.begin(),allPossible.scheduleGroup.end());

	// Display viable schedules
	allPossible.print(-2);
	return 0;
}

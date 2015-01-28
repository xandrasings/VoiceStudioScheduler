// Xandra Best
// 1/27/15
// Process of work

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Classes.h"
#include "Classes.cpp"
using namespace std;

#define NOBODY -1

// function for calculating number of permutations
int factorial(int times, int empty) {
	int result;
    if (times == empty)
        return 1;
    else
        return times * factorial(times-1, empty);
}


int process(TimeList catalog, StudentList roster, int threadct) {
	// Initialize important custom objects
	ScheduleGroup allPossible; // Holds all viable schedules
	Student nobody("Nobody"); // blank student "Nobody"

	// Set starting
	unsigned int numStudents = roster.studentList.size(); // Number of students to be scheduled
	unsigned int numTimeSlots = catalog.timeList.size(); // Number of available lesson times
	unsigned int hourLongCount = 0; // Number of students requiring hour-long lessons
	for (unsigned int i = 0; i < numStudents; i++) {
		if (roster.studentList[i].hourLong) {
			hourLongCount++;
		}
	}
	unsigned long long 	numPermutations = factorial(numTimeSlots-hourLongCount, numTimeSlots-numStudents-hourLongCount);
; // Number of schedule permutations
	vector<int> scheduleVec; // vector for handling schedule permutations
	unsigned int scheduleVecSize; // size of scheduleVec

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


	/* LOGIC LOOP */
	// First Time Loop 

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
			cout << ((float)k)/((float)numPermutations)*100 << "% - ";
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
		
		// If all ASSIGNMENTS in the SCHEDULE were viable, calculate rank and add SCHEDULE 
		if (viable) {
			preferenceRank = preferenceRank / (float) numStudents;
			localSchedule.preferenceRank = preferenceRank;
			#pragma omp critical
			{
				allPossible.add(localSchedule);
			}
		}
	}
	sort(allPossible.scheduleGroup.begin(),allPossible.scheduleGroup.end());
	allPossible.print(-2);

	//cin >> junk;

	return allPossible.scheduleGroup.size();
}

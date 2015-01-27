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
	TimeList catalog;
	StudentList roster;
	ScheduleGroup allPossible;
	int numStudents = 0;
	int numTimeSlots = 0;
	int hourLongCount = 0;
	int threadct = 1;
	unsigned long long numPermutations, doCount; //ints for tracking run-time progress
	string weekDaysS, timesS, studentLineS; //strings for holding a line of file input
	stringstream weekDaysSS, timesSS, studentLineSS; //stringstreams for holding a line of file input
	string junk, weekDayCheck, timeCheck, hourLongCheck; //strings for holding raw tokens of input
	string weekDay, name, availability;	//strings for holding modified tokens of input
	int hour, minute, delimiter, preference; //ints for holding modified tokens of input
	ifstream availabilityFile("StudentAvailability5.csv"); // input file

	/* Parse command-line args */
	if (argc > 1) {
		threadct = atoi(argv[1]);
	}

	/* Create student "Nobody" */
	Student nobody("Nobody");

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
		numTimeSlots++;
	}

	// Read in students
	int studRef = 0;
	while (availabilityFile >> studentLineS) {
		int timeRef = 0;
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
		numStudents++;		
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

	/* Terrifying process of doom */
	// Create vector for making all permutations.
	vector<int> scheduleVec;
	
	// Reference for each student
	for (int i = 0; i < numStudents; i++) {
		scheduleVec.push_back(i);
	}
	
	// Reference for each empty time
	for (int i = 0; i < (numTimeSlots - numStudents - hourLongCount); i++) {
		scheduleVec.push_back(NOBODY);
	}

	// Variables to be moved
	bool viable;
	float preferenceRank;
	int j;
	unsigned int i;
	bool hourAssignment;
	Schedule localSchedule;
	Assignment localAssignment1;
	Assignment localAssignment2;
	unsigned long long k;
	numPermutations = factorial(numTimeSlots-hourLongCount, numTimeSlots-numStudents-hourLongCount);
	

	sort(scheduleVec.begin(), scheduleVec.end());
	for (k = 0; k < numPermutations; k++) {
		// Empty local schedule for new loop
		localSchedule.clear();

		// Reset local preference rank and iterator
		preferenceRank = 0;
		j = 0;

		// Loop through assignments represented by permutation
		for (i = 0; i < scheduleVec.size(); i++) {
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
		next_permutation(scheduleVec.begin(),scheduleVec.end());
	}
	cout << endl;

	sort(allPossible.scheduleGroup.begin(),allPossible.scheduleGroup.end());

	// Display viable schedules
	allPossible.print(-2);
	return 0;
}

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

int main(int argc, const char* argv[]) {
	TimeList catalog;
	StudentList roster;
	ScheduleGroup allPossible;
	int numStudents = 0;
	int numTimeSlots = 0;
	string weekDaysS, timesS, studentLineS; //strings for holding a line
	stringstream weekDaysSS, timesSS, studentLineSS; //stringstreams for holding a line
	string junk, weekDayCheck, timeCheck; //strings for holding raw tokens
	string weekDay, name, availability;	//strings for holding modified token
	int hour, minute, delimiter; //ints for holding modified token
	ifstream availabilityFile("StudentAvailability.csv");

	/* Read in all info from file */
	// Read in time slots
	availabilityFile >> weekDaysS;
	weekDaysSS << weekDaysS << ",";
	getline(weekDaysSS, junk, ',');

	availabilityFile >> timesS;
	timesSS << timesS << ",";
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


	// Read in students from file
	int studRef = 0;
	while (availabilityFile >> studentLineS) {
		int timeRef = 0;
		studentLineSS << studentLineS << ",";
		getline(studentLineSS, name, ',');
		roster.add(Student(name));
		numStudents++;		
		while(getline(studentLineSS, availability, ',')) {
	    	if (availability == "y") {
		    	roster.studentList[studRef].availability.push_back(& catalog.timeList[timeRef]);
		    }
		    else if (availability != "n") {
		    	cout << "Error: One of your entries is neither 'y' or 'n'." << endl;
		    }
	    	timeRef++;
		}	
		studentLineSS.clear();
		studRef++;
	}	

	// Create student "Nobody"
	Student nobody("Nobody");
	
	// If you want to look at individual availability, uncomment this:
	// roster.printAvailability();
	
	// Create vector for making all permutations.
	vector<int> scheduleVec;
	
	// Reference for each student
	for (int i = 0; i < numStudents; i++) {
		scheduleVec.push_back(i);
	}
	
	// Reference for each empty time
	for (int i = 0; i < (numTimeSlots - numStudents); i++) {
		scheduleVec.push_back(NOBODY);
	}

	// Consider each possible schedule permutation
	sort(scheduleVec.begin(), scheduleVec.end());
	do {
		// Create a new empty schedule
		Schedule localSchedule;
		bool viable;
		for (unsigned int i = 0; i < scheduleVec.size(); i++) {
			// Create each assignment for schedule		
			Assignment localAssignment;
			if (scheduleVec[i] == NOBODY) {
				localAssignment.setArgs(catalog.timeList[i], nobody);
				localAssignment.availability = true;
			}
			else {
				localAssignment.setArgs(catalog.timeList[i], roster.studentList[scheduleVec[i]]);
				localAssignment.checkAvailability();
			}
			// Check if assignment is viable
			viable = localAssignment.availability;
			// If not, move on to next schedule
			if (!viable) {
				break;
			}
			// If viable, add Assignment to schedule, move on to next assignment
			localSchedule.add (localAssignment);
		}
		// If all assignments were viable, add schedule
		if (viable) {
			allPossible.add(localSchedule);
		}
	// Do this for every permutation
	} while (next_permutation(scheduleVec.begin(),scheduleVec.end()));

	// Give every viable result
	allPossible.print();	
	return 0;
}

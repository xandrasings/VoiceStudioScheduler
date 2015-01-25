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

	int numPermutations, doCount; //ints for tracking run-time progress
	string weekDaysS, timesS, studentLineS; //strings for holding a line of file input
	stringstream weekDaysSS, timesSS, studentLineSS; //stringstreams for holding a line of file input
	string junk, weekDayCheck, timeCheck, hourLongCheck; //strings for holding raw tokens of input
	string weekDay, name, availability;	//strings for holding modified tokens of input
	int hour, minute, delimiter, preference; //ints for holding modified tokens of input
	ifstream availabilityFile("StudentAvailability1.csv"); // input file

	// Create student "Nobody"
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
		    /*
		    else if (availability != "n") {
		    	cout << "Error: One of your entries is neither 'y' or 'n'." << endl;
		    }
		    */
	    	timeRef++;
		}	
		studentLineSS.clear();
		studRef++;
	}

	/* Optional Output */
	string response;
	// Individual student availability
	cout << "Would you like to look at students' responses?" << endl;
	cout << "Type 'y' for yes." << endl;
	cin >> response;
	if (response == "y") {
		roster.printAvailability();
	}

	// Progress on do while loop
	cout << "Would you like to see progress updates on the loop?" << endl;
	cin >> response;

	/* Terrifyling process of doom */
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

	// Consider each possible schedule permutation
	numPermutations = factorial(numTimeSlots-hourLongCount, numTimeSlots-numStudents-hourLongCount);
	sort(scheduleVec.begin(), scheduleVec.end());
	doCount = 1;
	do {
		if (response == "y") {
			cout << (doCount*100)/numPermutations << "% - doCount " << doCount << ": ";
			for (unsigned int i = 0; i < scheduleVec.size(); i++) {
				cout << scheduleVec[i] << ",";
			}
			cout << endl;
		}

		// Create a new empty schedule
		Schedule localSchedule;
		bool viable; // Viability of current schedule
		int j=0; // Separate iterator for referencing funky times
		float preferenceRank = 0;
		for (unsigned int i = 0; i < scheduleVec.size(); i++) {
			bool hourAssignment = false;

			// Create each assignment for schedule
			Assignment localAssignment1;
			Assignment localAssignment2;
			if (scheduleVec[i] == NOBODY) {
				localAssignment1.setArgs(catalog.timeList[j], nobody);
				localAssignment1.availability = true;
				localAssignment1.preferenceRank = 0;
			}
			else {
				localAssignment1.setArgs(catalog.timeList[j], roster.studentList[scheduleVec[i]]);
				localAssignment1.checkAvailability();
				if (roster.studentList[scheduleVec[i]].hourLong) {
					j++;
					hourAssignment = true;
					localAssignment2.setArgs(catalog.timeList[j], roster.studentList[scheduleVec[i]]);
					localAssignment2.checkAvailability();
				}
			}
			// Check if assignment is viable
			if (hourAssignment) {
				viable = (localAssignment1.availability && localAssignment2.availability);
			}
			else {
				viable = localAssignment1.availability;
			}
			// If not, move on to next schedule
			if (!viable) {
				break;
			}
			// If viable, add Assignment to schedule, move on to next assignment
			localSchedule.add (localAssignment1);
			
			if (hourAssignment) {
				localSchedule.add(localAssignment2);
				preferenceRank += (localAssignment2.preferenceRank / 2);
				preferenceRank += (localAssignment2.preferenceRank / 2);
			}
			else {
				preferenceRank += localAssignment1.preferenceRank;
			}
			j++;
		}
		localSchedule.preferenceRank = preferenceRank;
		// If all assignments were viable, add schedule
		if (viable) {
			allPossible.add(localSchedule);
		}
		doCount++;
	// Do this for every permutation
	} while (next_permutation(scheduleVec.begin(),scheduleVec.end()));
	cout << endl;

	sort(allPossible.scheduleGroup.begin(),allPossible.scheduleGroup.end());


	/* Optional Output */
	// Display viable schedules
	int numDisplay;
	cout << "How many schedules would you like to see?" << endl;
	cout << "Respond with a number or 'all'" << endl;
	cin >> response;
	if (response == "all") {
		numDisplay = -1;
	}
	else {
		numDisplay = atoi(response.c_str());
	}
	allPossible.print(numDisplay);

	return 0;
}

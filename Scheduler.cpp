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
	//Schedule testSchedule;
	ScheduleGroup allPossible;
	int numStudents = 0;
	int numTimeSlots = 0;

	// Read in available times from file
	ifstream catalogFile("times.txt");
	string newWeekDay, newTime;
	int delimiter, newHour, newMinute;
	while (catalogFile >> newWeekDay >> newTime) {
		delimiter = newTime.find(":");
		newHour = stoi(newTime.substr(0,delimiter));
		newMinute = stoi(newTime.substr(delimiter+1,delimiter+3));
		catalog.add(Time(newWeekDay, newHour, newMinute));
		numTimeSlots++;
	}

	// Read in students from file
	ifstream rosterFile("roster.txt");
	string newName;
	while (getline(rosterFile, newName)) {
		roster.add(Student(newName));
		cout << "newName is currently " << newName << endl;
		numStudents++;
	}

	Student nobody("Nobody");
	for (unsigned int i = 0; i < catalog.timeList.size(); i++) {
		//nobody.availability.
	}
	
	roster.checkAvailability(catalog);
	roster.printAvailability();
	
	//create vector that decides how to make 
	vector<int> scheduleVec;
	for (int i = 0; i < numStudents; i++) {
		scheduleVec.push_back(i);
	}
	for (int i = 0; i < (numTimeSlots - numStudents); i++) {
		scheduleVec.push_back(NOBODY);
	}

	sort(scheduleVec.begin(), scheduleVec.end());

	do {
		Schedule localSchedule;
		bool viable;
		for (unsigned int i = 0; i < scheduleVec.size(); i++) {
			Assignment localAssignment;
			if (scheduleVec[i] == NOBODY) {
				localAssignment.setArgs(catalog.timeList[i], nobody);
				localAssignment.availability = true;
			}
			else {
				localAssignment.setArgs(catalog.timeList[i], roster.studentList[scheduleVec[i]]);
				localAssignment.checkAvailability();
			}
			localSchedule.add (localAssignment);
			viable = localAssignment.availability;
			if (!viable) {
				break;
			}
		}
		if (viable) {
			allPossible.add(localSchedule);
		}
	} while (next_permutation(scheduleVec.begin(),scheduleVec.end()));

	allPossible.print();	
	return 0;
}

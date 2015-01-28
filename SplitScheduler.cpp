// Xandra Best
// 1/27/15
// Scheduler Application

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Classes.h"
#include "Classes.cpp"
#include "ScheduleAlgorithm.cpp"
using namespace std;


int main(int argc, const char* argv[]) {
	/* Initializing Variables */
	// Arguments
	int threadct = 1; // Number of threads (default = 1)
	string fileName = "StudentAvailability.csv";

	// Initialize important custom objects
	TimeList catalog; // Holds available lesson times
	StudentList roster; // Holds students to be scheduled
	ScheduleGroup allPossible; // Holds all viable schedules

	// Set starting
	//unsigned int numStudents; // Number of students to be scheduled
	unsigned int numTimeSlots; // Number of available lesson times
	unsigned int numDays;
	unsigned int hourLongCount = 0; // Number of students requiring hour-long lessons
	unsigned long long numPermutations; // Number of schedule permutations
	int nextDay;

	// variables for reading in from file
	unsigned int studRef, timeRef; // ints for referencing students and times
	string weekDaysS, timesS, studentLineS; //strings for holding a line of file input
	stringstream weekDaysSS, timesSS, studentLineSS; //stringstreams for holding a line of file input
	string junk, weekDayCheck, timeCheck, hourLongCheck; //strings for holding raw tokens of input
	string weekDay, name, availability;	//strings for holding modified tokens of input
	int hour, minute, delimiter, preference; //ints for holding modified tokens of input

	//vector<string> lessonDays;
	vector<StudentList> dailyRoster;
	vector<TimeList> dailyCatalog;
	Week fullWeek;

	/* Parse command-line args */
	if (argc == 1) {
		//cout << "NO ARGUMENTS " << endl;
		//return 0;
	}
	else {
		fileName = argv[1];
	}
	if (argc == 3) {
		threadct = atoi(argv[2]);		
	}
	ifstream availabilityFile(fileName);

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
			Weekday localWeekDay(weekDay);
			fullWeek.add(localWeekDay);
		}
		else {
			fullWeek.fullWeek.back().count++;
		}
		getline(timesSS, timeCheck, ',');
		delimiter = timeCheck.find(":");
		hour = stoi(timeCheck.substr(0,delimiter));
		minute = stoi(timeCheck.substr(delimiter+1,delimiter+3));
		catalog.add(Time(weekDay,hour,minute));
	}

	numTimeSlots = catalog.timeList.size();
	numDays = fullWeek.fullWeek.size();
	for (unsigned int i = 0; i < numDays; i++) {
		TimeList localTimeList;
		dailyCatalog.push_back(localTimeList);
		StudentList localStudentList;
		dailyRoster.push_back(localStudentList);
	}

	// Read in students
	studRef = 0;
	string previousDay = "";
	while (availabilityFile >> studentLineS) {
		timeRef = 0;
		studentLineSS << studentLineS << ",";
		getline(studentLineSS, hourLongCheck, ',');
		getline(studentLineSS, name, ',');
		roster.add(Student(name, numDays));
		if (hourLongCheck == "*") {
			roster.studentList[studRef].hourLong = true;
			hourLongCount++;
		}
		else {
			roster.studentList[studRef].hourLong = false;
		}
		for (int i = 0; i < numDays; i++) {
			for (unsigned int j = 0; j < fullWeek.fullWeek[i].count; j++) {
				getline(studentLineSS, availability, ',');
				preference = atoi(availability.c_str());
		    	if (availability != "0") {
			    	roster.studentList[studRef].availability.push_back(& catalog.timeList[timeRef]);
			    	roster.studentList[studRef].preference.push_back(preference);
			    	roster.studentList[studRef].preferenceByDay[i] += preference;
			    }
			    fullWeek.fullWeek[i].preferenceCount++;
			    fullWeek.fullWeek[i].preferenceTotal += preference;
		    	timeRef++;
			}	
		}
		studentLineSS.clear();
		for (int i = 0; i < numDays; i++ ) {
			roster.studentList[studRef].preferenceByDay[i] = roster.studentList[studRef].preferenceByDay[i] / (float) fullWeek.fullWeek[i].count;
		}
		studRef++;
	}
	fullWeek.calcPref();

	/* MAIN LOGIC */


	int result;
	bool retry = false;
	unsigned int runCount = fullWeek.fullWeek.size();
	while (runCount > 0) {

		// Skip all this if we're just retrying a day with fewer students
		if (!retry) {
			// Otherwise, find best students for next day.
			nextDay = fullWeek.nextDay(); // Find next least popular day
			roster.sortByDay(nextDay);
			int addStudents = fullWeek.fullWeek[nextDay].count;
			// Add students to this day
			while (addStudents > 0) {
				bool foundStudent = false;
				if (addStudents == 1) { // If it's the last space, must be half hour student
					for (unsigned int i = 0; i < roster.studentList.size(); i++) {
						if (!roster.studentList[i].assigned && !roster.studentList[i].hourLong) {
							foundStudent = true;
							roster.studentList[i].assigned = true;
							dailyRoster[nextDay].studentList.push_back(roster.studentList[i]);
							//roster.studentList[i].print();
							addStudents--;
							break;
						}
					}
				}
				else { // Add any good student
					for (unsigned int i = 0; i < roster.studentList.size(); i++) {
						if (!roster.studentList[i].assigned) {
							foundStudent = true;
							roster.studentList[i].assigned = true;
							dailyRoster[nextDay].studentList.push_back(roster.studentList[i]);
							//roster.studentList[i].print();
							addStudents--;
							if (roster.studentList[i].hourLong) {
								addStudents--;
							}
							break;
						}
					}
				}
				if (!foundStudent) { // No students left
					break;
				}
			}

			int scheduleRef = 0;
			for (int i = 0; i < nextDay; i++) {
				scheduleRef += fullWeek.fullWeek[i].count;
			}
			for (unsigned int i = 0; i < fullWeek.fullWeek[nextDay].count; i++) {
				dailyCatalog[nextDay].timeList.push_back(catalog.timeList[scheduleRef+i]);
			}

		}

		result = process(dailyCatalog[nextDay], dailyRoster[nextDay], threadct);

		if (result == 0) {
		// didn't work with those students.

			if (runCount == 1) {
				// strategy didn't work, can't schedule most popular day.
				cout << "SORRY, could not create complete schedule." << endl;
				break;
			}

			// We'll try one less student
			for (unsigned int i = 0; i < roster.studentList.size(); i++) {
				if (roster.studentList[i].name == dailyRoster[nextDay].studentList.back().name) {
					roster.studentList[i].assigned = false;
				}
			}
			dailyRoster[nextDay].studentList.pop_back();
			retry = true;
		}

		// Worked! Move on.
		else {
			cout << " of " << result << " viable results." << endl << endl << endl;
			fullWeek.fullWeek[nextDay].assigned = true;
			runCount--;
			retry = false;
		}
	}
	return 0;
}

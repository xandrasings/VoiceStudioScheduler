// Xandra Best
// 1/23/15
// Classes of Scheduler Application

#include <iostream>
#include <vector>
#include "classes.h"
using namespace std;

/* Time */
Time::Time () {
	weekDay = "Monday";
	hour = 12;
	minute = 30;
}

Time::Time (string w, int h, int m) {
	weekDay = w;
	hour = h;
	minute = m;
}

bool operator==(const Time& lhs, const Time& rhs) {
	return (
		lhs.weekDay == rhs.weekDay &&
		lhs.hour == rhs.hour &&
		lhs.minute == rhs.minute);
}

void Time::print () {
	cout << weekDay << " at " << hour << ":";
	if (minute == 0) {
		cout << "00";
	}
	else {
		cout << minute;
	}
	cout << endl;
}

/* TimeList */
void TimeList::add(Time newTime) {
	timeList.push_back(newTime);	
}

void TimeList::print() {
	cout << "All times: " << endl;
	for (unsigned int i = 0; i < timeList.size(); i++) {
		timeList[i].print();
	}
}

/* Student */
Student::Student (string n) {
	name = n;
}

void Student::checkAvailability (TimeList& myList) {
	int available = 0;
	cout << "Is " << name << " available at the following times?" << endl;
	cout << "(Type '1' for yes or '0' for no.)" << endl;
	for (unsigned int i = 0; i < myList.timeList.size(); i++) {
		myList.timeList[i].print();
		cin >> available;
		if (available != 0) {
			availability.push_back(& myList.timeList[i]);
		}
		//make a protective loop here
		available = 'n';
	}
}

void Student::printAvailability () {
	for (unsigned int i = 0; i < availability.size(); i++) {
		//cout << preference[i] << " ";
		availability[i]->print();
	}
}

/* Student List */
void StudentList::add (Student newStudent) {
	studentList.push_back(newStudent);
}

void StudentList::checkAvailability (TimeList& myList) {
	for (unsigned int i = 0; i < studentList.size(); i++) {
		studentList[i].checkAvailability(myList);
		cout << endl;
	}
}

void StudentList::printAvailability () {
	for (unsigned int i = 0; i < studentList.size(); i++) {
		cout << "Student " << studentList[i].name << " is ";
		if (!studentList[i].hourLong) {
			cout << "not ";
		}
		cout << "hourly, and is available at the following times: " << endl;
		studentList[i].printAvailability();
		cout << endl;
	}
}

/* Assignment */
Assignment::Assignment () {}

Assignment::Assignment (Time& t, Student& s) {
	time = &t;
	student = &s;
	availability = false;
}

void Assignment::setArgs (Time& t, Student& s) {
	time = &t;
	student = &s;
	availability = false;
}

void Assignment::checkAvailability () {
	bool availCheck = false;
	for (unsigned int i = 0; i < student->availability.size(); i++) {
		if (*time == *(student->availability[i])) {
			preferenceRank = student->preference[i];
			availCheck = true;
			break;
		}
	}
	availability = availCheck;
}

void Assignment::print () {
	//cout << preferenceRank << " ";
	cout << student->name << " on ";
	time->print();
	//cout << "Available: " << availability << endl;
}

/* Schedule */
void Schedule::add (Assignment newAssignment) {
	schedule.push_back(newAssignment);
}

void Schedule::print () {
	for (unsigned int i = 0; i < schedule.size(); i++) {
		schedule[i].print();
	}
}

/* ScheduleGroup */
void ScheduleGroup::add (Schedule newSchedule) {
	scheduleGroup.push_back(newSchedule);
}

void ScheduleGroup::print () {
	if (scheduleGroup.size() > 0) {
		cout << "All viable schedules: " << endl;
		for (unsigned int i = 0; i < scheduleGroup.size(); i++) {
			cout << "Ranking of " << scheduleGroup[i].preferenceRank << endl;
			scheduleGroup[i].print();
			cout << endl;
		}
	}
	else {
		cout << "No viable schedules :[" << endl;
	}
}
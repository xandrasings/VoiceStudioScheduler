// Xandra Best
// 1/23/15
// Header for Classes of Scheduler Application

#ifndef _CLASSES_
#define _CLASSES_
#include <vector>
using namespace std;

/* Time */
class Time {
public:
	string weekDay;
	int hour;
	int minute;
public:
	Time ();
	Time (string, int, int);
	void print ();
};

/* TimeList */
class TimeList {
public:
	vector<Time> timeList;
public:
	void add (Time);
	void print ();
};

/* Student */
class Student {
public:
	string name;
	vector<Time*> availability;
	vector<int> preference;
	bool hourLong;
public:
	Student (string);
	void checkAvailability (TimeList&);
	void printAvailability ();
};

/* StudentList */
class StudentList {
public:
	vector<Student> studentList;
public:
	void add (Student);
	void checkAvailability (TimeList&);
	void printAvailability ();
};

/* Assignment */
class Assignment {
public:
	Time* time;
	Student* student;
	int preferenceRank;
	bool availability;
public:
	Assignment ();
	Assignment (Time&, Student&);
	void setArgs (Time&, Student&);
	void checkAvailability ();
	void print ();
};

/* Schedule */
class Schedule {
public:
	vector<Assignment> schedule;
	int preferenceRank;
public:
	void add (Assignment);
	void print ();
};

/* ScheduleGroup */
class ScheduleGroup {
public:
	vector<Schedule> scheduleGroup;
public:
	void add (Schedule);
	void print ();
};

#endif // _CLASSES_

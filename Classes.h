// Xandra Best
// 1/23/15
// Header for Classes of Scheduler Application

#ifndef _CLASSES_
#define _CLASSES_
#include <vector>
using namespace std;

/* Weekday */
class Weekday {
public:
	string day;
	unsigned int count;
	unsigned int preferenceTotal;
	unsigned int preferenceCount;
	float avPreference;
	bool assigned;
public:
	Weekday (string);
	void print ();
	void calcPref ();
};

/* Week */
class Week {
public:
	vector<Weekday> fullWeek;
public:
	void add (Weekday);
	void calcPref ();
	void sortWeek ();
	void print ();
	int nextDay ();
};

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
	vector<float> preference;
	vector<float> preferenceByDay;
	float sortablePreference;
	bool hourLong;
	bool assigned;
public:
	Student (string);
	Student (string, int);
	void checkAvailability (TimeList&);
	void printAvailability ();
	void print ();
	void setPreference (int);
};

/* StudentList */
class StudentList {
public:
	vector<Student> studentList;
public:
	void add (Student);
	void checkAvailability (TimeList&);
	void printAvailability ();
	void setPreference (int);
	void sortByDay (int);
};

/* Assignment */
class Assignment {
public:
	Time* time;
	Student* student;
	float preferenceRank;
	bool availability;
public:
	Assignment ();
	Assignment (Time&, Student&);
	void setArgs (Time&, Student&);
	void checkAvailability ();
	void print ();
	void clear ();
};

/* Schedule */
class Schedule {
public:
	vector<Assignment> schedule;
	float preferenceRank;
public:
	void add (Assignment);
	void print ();
	void clear();
};

/* ScheduleGroup */
class ScheduleGroup {
public:
	vector<Schedule> scheduleGroup;
public:
	void add (Schedule);
	void print ();
	void print (int);
};

#endif // _CLASSES_
